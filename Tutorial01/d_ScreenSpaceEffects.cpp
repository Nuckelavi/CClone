#include "d_ScreenSpaceEffects.h"

using namespace DirectX;

SSEffects::SSEffects() : 
	_pVertexLayout(nullptr),
	_pVertexShader(nullptr),
	_pPixelShader(nullptr),
	_pConstantBuffer(nullptr),
    _pCustomRenderTarget(nullptr),
    _pCustomRTV(nullptr),
    _pCustomSRV(nullptr)
{
}

SSEffects::~SSEffects()
{
	if (_pVertexShader) _pVertexShader->Release();
	if (_pPixelShader) _pPixelShader->Release();
	if (_pVertexLayout) _pVertexLayout->Release();
	if (_pConstantBuffer) _pConstantBuffer->Release();

    if (_pCustomRenderTarget) _pCustomRenderTarget->Release();
    if (_pCustomRTV) _pCustomRTV->Release();
    if (_pCustomSRV) _pCustomSRV->Release();
}

HRESULT SSEffects::SetupShader(ID3D11Device* pd3dDevice)
{
	HRESULT hr;

	hr = CreateShaders(pd3dDevice);
	if (FAILED(hr))
		return hr;

	hr = CreateBuffers(pd3dDevice);
	if (FAILED(hr))
		return hr;

	hr = CreateTextures(pd3dDevice);
	if (FAILED(hr))
		return hr;

	return hr;
}

HRESULT SSEffects::CreateShaders(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    //create quad VS shader
    ID3DBlob* pVSBlob = nullptr;
    hr = DX11::CompileShaderFromFile(L"d_SimpleSSEffects.fx", "VS", "vs_4_0", &pVSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The d_SimpleSSEffects.fx file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return hr;
    }

    D3D11_INPUT_ELEMENT_DESC layout[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT numElements2 = ARRAYSIZE(layout);

    hr = pd3dDevice->CreateInputLayout(layout, numElements2, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &_pVertexLayout);
    if (FAILED(hr))
        return hr;


    //set up quad pixel shader
    ID3DBlob* pPSBlob = nullptr;
    hr = DX11::CompileShaderFromFile(L"d_SimpleSSEffects.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr, L"The d_quadShader.fx file cannot be compiled. Pixel shader failed.", L"Error", MB_OK);
        return hr;
    }

    hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return hr;

    
    return hr;
}

HRESULT SSEffects::CreateBuffers(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    //set constant buffers
    D3D11_BUFFER_DESC bd = {};
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBufferQuad);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);
    if (FAILED(hr))
        return hr;

	return hr;
}

HRESULT SSEffects::CreateTextures(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    //Create render to target
    D3D11_TEXTURE2D_DESC renderToTexDesc = {};
    ZeroMemory(&renderToTexDesc, sizeof(renderToTexDesc));
    renderToTexDesc.Width = 640; //g_viewWidth;
    renderToTexDesc.Height = 480;//g_viewHeight;
    renderToTexDesc.MipLevels = 1;
    renderToTexDesc.ArraySize = 1;
    renderToTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    renderToTexDesc.SampleDesc.Count = 1;
    renderToTexDesc.SampleDesc.Quality = 0;
    renderToTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    renderToTexDesc.Usage = D3D11_USAGE_DEFAULT;
    renderToTexDesc.CPUAccessFlags = 0;
    renderToTexDesc.MiscFlags = 0;
    hr = pd3dDevice->CreateTexture2D(&renderToTexDesc, nullptr, &_pCustomRenderTarget);
    if (FAILED(hr))
        return hr;

    D3D11_RENDER_TARGET_VIEW_DESC customRtvDesc;
    customRtvDesc.Format = renderToTexDesc.Format;
    customRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    customRtvDesc.Texture2D.MipSlice = 0;
    hr = pd3dDevice->CreateRenderTargetView(_pCustomRenderTarget, &customRtvDesc, &_pCustomRTV);
    if (FAILED(hr))
        return hr;

    D3D11_SHADER_RESOURCE_VIEW_DESC customSrvDesc;
    customSrvDesc.Format = renderToTexDesc.Format;
    customSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    customSrvDesc.Texture2D.MostDetailedMip = 0;
    customSrvDesc.Texture2D.MipLevels = 1;
    hr = pd3dDevice->CreateShaderResourceView(_pCustomRenderTarget, &customSrvDesc, &_pCustomSRV);
    if (FAILED(hr))
        return hr;

    return hr;
}

void SSEffects::SetConstantBuffer(ID3D11DeviceContext* pContext, int screenW, int screenH, int effect)
{
	ConstantBufferQuad cbQuad;
	cbQuad.vScreenSize = XMFLOAT2((float)screenW, (float)screenH);
	cbQuad.nEffectID = effect;
	cbQuad.nBlur = 0;
	pContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cbQuad, 0, 0);

}

void SSEffects::Render(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
    //bind shader
    pContext->IASetInputLayout(_pVertexLayout);
    pContext->VSSetShader(_pVertexShader, nullptr, 0);
    pContext->PSSetShader(_pPixelShader, nullptr, 0);

    pContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
    pContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);

    pd3dDevice->CreateShaderResourceView(_pCustomRenderTarget, nullptr, &_pCustomSRV);
    pContext->PSSetShaderResources(0, 1, &_pCustomSRV);

}
