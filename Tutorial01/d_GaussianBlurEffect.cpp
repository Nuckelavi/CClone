#include "d_GaussianBlurEffect.h"

GaussianBlurFX::GaussianBlurFX() : 
    _pVertexLayout(nullptr),
    _pVertexShader(nullptr),
    _pPixelShader(nullptr),
    _pConstantBuffer(nullptr)
{
    for (int i = 0; i < RTTS; ++i)
    {
        _pCustomRenderTarget[i] = nullptr;
        _pCustomRTV[i] = nullptr;
        _pCustomSRV[i] = nullptr;
    }
}

GaussianBlurFX::~GaussianBlurFX()
{
    if (_pVertexShader) _pVertexShader->Release();
    if (_pPixelShader) _pPixelShader->Release();
    if (_pVertexLayout) _pVertexLayout->Release();
    if (_pConstantBuffer) _pConstantBuffer->Release();

    for (int i = 0; i < RTTS; ++i)
    {
        if (_pCustomRenderTarget[i]) _pCustomRenderTarget[i]->Release();
        if (_pCustomRTV[i]) _pCustomRTV[i]->Release();
        if (_pCustomSRV[i]) _pCustomSRV[i]->Release();
    }
}

HRESULT GaussianBlurFX::SetupShader(ID3D11Device* pd3dDevice)
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

HRESULT GaussianBlurFX::CreateShaders(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    //create quad VS shader
    ID3DBlob* pVSBlob = nullptr;
    hr = DX11::CompileShaderFromFile(L"d_GaussianBlur.fx", "VS", "vs_4_0", &pVSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The d_GaussianBlur.fx file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
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
    UINT numElements = ARRAYSIZE(layout);

    hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &_pVertexLayout);
    if (FAILED(hr))
        return hr;


    //set up quad pixel shader
    ID3DBlob* pPSBlob = nullptr;
    hr = DX11::CompileShaderFromFile(L"d_GaussianBlur.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr, L"The d_GaussianBlur.fx file cannot be compiled. Pixel shader failed.", L"Error", MB_OK);
        return hr;
    }

    hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return hr;

    return hr;
}

HRESULT GaussianBlurFX::CreateBuffers(ID3D11Device* pd3dDevice)
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

HRESULT GaussianBlurFX::CreateTextures(ID3D11Device* pd3dDevice)
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
    for (int i = 0; i < RTTS; ++i)
    {
        hr = pd3dDevice->CreateTexture2D(&renderToTexDesc, nullptr, 
            &_pCustomRenderTarget[i]);
        if (FAILED(hr))
            return hr;
    }

    D3D11_RENDER_TARGET_VIEW_DESC customRtvDesc;
    customRtvDesc.Format = renderToTexDesc.Format;
    customRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    customRtvDesc.Texture2D.MipSlice = 0;
    for (int i = 0; i < RTTS; ++i)
    {
        hr = pd3dDevice->CreateRenderTargetView(_pCustomRenderTarget[i], 
            &customRtvDesc, &_pCustomRTV[i]);
        if (FAILED(hr))
            return hr;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC customSrvDesc;
    customSrvDesc.Format = renderToTexDesc.Format;
    customSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    customSrvDesc.Texture2D.MostDetailedMip = 0;
    customSrvDesc.Texture2D.MipLevels = 1;
    for (int i = 0; i < RTTS; ++i)
    {
        hr = pd3dDevice->CreateShaderResourceView(_pCustomRenderTarget[i],
            &customSrvDesc, &_pCustomSRV[i]);
        if (FAILED(hr))
            return hr;
    }

    return hr;
}

void GaussianBlurFX::SetConstantBuffer(ID3D11DeviceContext* pContext, int screenW, int screenH, int effect)
{
    ConstantBufferQuad cbQuad;
    cbQuad.vScreenSize = DirectX::XMFLOAT2((float)screenW, (float)screenH);
    cbQuad.nEffectID = effect;
    cbQuad.nBlur = 0;
    pContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cbQuad, 0, 0);
}

void GaussianBlurFX::Render(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
}


//OLD RTT BACKUP

/*
ConstantBufferQuad cbQuad;
cbQuad.vScreenSize = XMFLOAT2((float)g_viewWidth, (float)g_viewHeight);
cbQuad.nBlur = 1;
cbQuad.nEffectID = 0;
g_pImmediateContext->UpdateSubresource(g_pConstantBufferQuad, 0, nullptr, &cbQuad, 0, 0);
*/

/*
case Scene::GAUSSIAN:
    cbQuad.nEffectID = 2;
    g_pImmediateContext->UpdateSubresource(g_pConstantBufferQuad, 0, nullptr, &cbQuad, 0, 0);
    RenderQuadEffects();
    break;*/

//----------------------------------------------------------
//THOU SHALLT BE NUKED 
//----------------------------------------------------------
/*

void RenderQuadEffects()
{
    //ID3D11RenderTargetView* nullRTV = nullptr;
    //g_pImmediateContext->OMSetRenderTargets(1, &nullRTV, nullptr);


    g_pImmediateContext->OMSetRenderTargets(1, &g_customRenderTargetView, g_pDepthStencilView);

    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::Coral);
    g_pImmediateContext->ClearRenderTargetView(g_customRenderTargetView, Colors::SeaGreen);
    //g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    //g_pImmediateContext->ClearDepthStencilView(g_depthBlurDsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);


    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    g_CubeTest.SetVertexBuffer(g_pImmediateContext);
    g_CubeTest.SetIndexBuffer(g_pImmediateContext);

    g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
    g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);

    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(1, 1, &g_pMaterialConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pLightConstantBuffer);


    //for cube
    ID3D11ShaderResourceView* tempsrv = (g_TextureManager.TexturesAt(TextureGroup::STONE));
    g_pImmediateContext->PSSetShaderResources(0, 1, &tempsrv);
    //if(tempsrv) tempsrv->Release();
    //for graphics cube
    //g_pImmediateContext->PSSetShaderResources(0, 3, g_pTextureRVs);




    g_CubeTest.Draw(g_pImmediateContext);



    //RTT------------------
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);//nullptr);




    //bind shader
    g_pImmediateContext->IASetInputLayout(g_pVertexLayoutQuad);
    g_pImmediateContext->VSSetShader(g_pVertexShaderQuad, nullptr, 0);
    g_pImmediateContext->PSSetShader(g_pPixelShaderQuad, nullptr, 0);

    g_QuadTest->SetVertexBuffer(g_pImmediateContext);
    g_QuadTest->SetIndexBuffer(g_pImmediateContext);

    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBufferQuad);
    g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBufferQuad);

    g_pd3dDevice->CreateShaderResourceView(g_renderToTexture, nullptr, &g_customShaderResourceView);
    g_pImmediateContext->PSSetShaderResources(0, 1, &g_customShaderResourceView);

    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

    g_QuadTest->Draw(g_pImmediateContext);

    ID3D11ShaderResourceView* const pSRV[1] = { NULL };
    g_pImmediateContext->PSSetShaderResources(0, 1, pSRV);
}
*/
//----------------------------------------------------------

/*
g_pImmediateContext->OMSetRenderTargets(1, &g_customRenderTargetView, g_pDepthStencilView);

g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::MidnightBlue);
g_pImmediateContext->ClearRenderTargetView(g_customRenderTargetView, Colors::SeaGreen);
g_pImmediateContext->ClearRenderTargetView(g_customRenderTargetView2, Colors::SeaGreen);
g_pImmediateContext->ClearRenderTargetView(g_customRenderTargetView3, Colors::SeaGreen);
g_pImmediateContext->ClearRenderTargetView(g_customRenderTargetView4, Colors::SeaGreen);
g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
g_pImmediateContext->ClearDepthStencilView(g_depthBlurDsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

g_View = XMLoadFloat4x4(&currentCam.GetView());
g_Projection = XMLoadFloat4x4(&currentCam.GetProjection());

g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

UINT cubeStride = sizeof(SimpleVertex);
UINT offset = 0;
g_pImmediateContext->IASetVertexBuffers(0, 1, g_GameObject.getPpVertexBuffer(), &cubeStride, &offset);

g_pImmediateContext->IASetIndexBuffer(g_GameObject.getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
g_pImmediateContext->PSSetConstantBuffers(1, 1, &g_pMaterialConstantBuffer);
g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pLightConstantBuffer);

g_pImmediateContext->PSSetShaderResources(0, g_texNum, &*g_pTextureRVs);
g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

g_pImmediateContext->DrawIndexed(36, 0, 0);
*/

