#include "d_SurfaceDetailEffect.h"
#include "DDSTextureLoader.h"

using namespace DirectX;

SurfaceDetailFX::SurfaceDetailFX() :
    _pVertexShader(nullptr),
    _pPixelShader(nullptr),
    _pVertexLayout(nullptr),
    _pConstantBuffer(nullptr),
    _pLightBuffer(nullptr),
    _pMaterialBuffer(nullptr),
    _pSamplerLinear(nullptr)
{
    _pTextureRVs = new ID3D11ShaderResourceView* [3];
}

SurfaceDetailFX::~SurfaceDetailFX()
{
    if (_pVertexShader) _pVertexShader->Release();
    if (_pPixelShader) _pPixelShader->Release();
    if (_pVertexLayout) _pVertexLayout->Release();
    if (_pConstantBuffer) _pConstantBuffer->Release();
    if (_pLightBuffer) _pLightBuffer->Release();
    if (_pMaterialBuffer) _pMaterialBuffer->Release();
    if (_pSamplerLinear) _pSamplerLinear->Release();

    for (int i = 0; i < 3; ++i)
    {
        if (_pTextureRVs[i]) _pTextureRVs[i]->Release();
    }
    delete[] _pTextureRVs;
}

HRESULT SurfaceDetailFX::SetupShader(ID3D11Device* pd3dDevice)
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

HRESULT SurfaceDetailFX::CreateShaders(ID3D11Device* pd3dDevice)
{
    ID3DBlob* pVSBlob2 = nullptr;
    HRESULT hr = DX11::CompileShaderFromFile(L"d_POMShader.fx", "VS", "vs_4_0", &pVSBlob2);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The POM Vertex Shader FX file cannot be compiled.", L"Error", MB_OK);
        return hr;
    }

    // Create the vertex shader
    hr = pd3dDevice->CreateVertexShader(pVSBlob2->GetBufferPointer(), pVSBlob2->GetBufferSize(), nullptr, &_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob2->Release();
        return hr;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob2->GetBufferPointer(),
        pVSBlob2->GetBufferSize(), &_pVertexLayout);
    pVSBlob2->Release();
    if (FAILED(hr))
        return hr;

    // Set the input layout
    //g_pImmediateContext->IASetInputLayout(g_pVertexLayoutPOM);

    // Compile the pixel shader
    ID3DBlob* pPSBlob2 = nullptr;
    hr = DX11::CompileShaderFromFile(L"d_POMShader.fx", "PS", "ps_4_0", &pPSBlob2);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The POM Pixel Shader FX file cannot be compiled.", L"Error", MB_OK);
        return hr;
    }

    // Create the pixel shader
    hr = pd3dDevice->CreatePixelShader(pPSBlob2->GetBufferPointer(), pPSBlob2->GetBufferSize(), nullptr, &_pPixelShader);
    pPSBlob2->Release();
    if (FAILED(hr))
        return hr;


	return hr;
}

HRESULT SurfaceDetailFX::CreateBuffers(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBufferPOM);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);
    if (FAILED(hr))
        return hr;

    bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(MaterialPropertiesConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = pd3dDevice->CreateBuffer(&bd, nullptr, &_pMaterialBuffer);
    if (FAILED(hr))
        return hr;

    // Create the light constant buffer
    bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(LightPropertiesConstantBuffer2);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = pd3dDevice->CreateBuffer(&bd, nullptr, &_pLightBuffer);
    if (FAILED(hr))
        return hr;


    return hr;
}

HRESULT SurfaceDetailFX::CreateTextures(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    int texCount = 0;
    const wchar_t* texFilePaths[] =
    {
        //L"Resources\\stone.dds", //0
        ////L"Resources\\conenormal.dds",
        //L"Resources\\stone_texture.dds", //1
        //L"Resources\\stone_normalMap.dds",
        //L"Resources\\stone_heightMap.dds",
        L"Resources\\bricks_TEX.dds", //4
        L"Resources\\bricks_NORM.dds",
        L"Resources\\bricks_DISP.dds",
        //L"Resources\\toybox_TEX.dds", //7
        //L"Resources\\toybox_NORM.dds",
        //L"Resources\\toybox_DISP.dds"
    };

    for (int i = 0; i < 3; ++i)
    {
        hr = CreateDDSTextureFromFile(pd3dDevice, texFilePaths[i], nullptr, &_pTextureRVs[texCount]);//&g_pTextureRV);
        if (FAILED(hr))
            return hr;
        ++texCount;
    }

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = pd3dDevice->CreateSamplerState(&sampDesc, &_pSamplerLinear);

    return hr;
}

void SurfaceDetailFX::SetConstantBuffers(ID3D11DeviceContext* pContext, XMMATRIX* world, 
    XMMATRIX* view, XMMATRIX* proj, const XMFLOAT4& lightPos, const XMFLOAT4& camPos, const Light& light, int effect)
{
    ConstantBufferPOM cbPOM;
    cbPOM.mWorld = XMMatrixTranspose(*world);
    cbPOM.mView = XMMatrixTranspose(*view);
    cbPOM.mProjection = XMMatrixTranspose(*proj);
    cbPOM.fHeightScale = 0.15f;//0.05f;
    cbPOM.nMinSamples = 8;
    cbPOM.nMaxSamples = 32;
    cbPOM.nEffectID = effect;
    pContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cbPOM, 0, 0);

    MaterialPropertiesConstantBuffer redPlasticMaterial;
    redPlasticMaterial.Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    redPlasticMaterial.Material.Specular = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);
    redPlasticMaterial.Material.SpecularPower = 32.0f;
    redPlasticMaterial.Material.UseTexture = true;
    pContext->UpdateSubresource(_pMaterialBuffer, 0, nullptr, &redPlasticMaterial, 0, 0);

    LightPropertiesConstantBuffer2 lightProp;
    lightProp.EyePosition = lightPos;
    lightProp.CameraPosition = camPos;
    lightProp.Lights[0] = light;
    pContext->UpdateSubresource(_pLightBuffer, 0, nullptr, &lightProp, 0, 0);
}

void SurfaceDetailFX::Render(ID3D11DeviceContext* pContext)
{
    pContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
    pContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
    pContext->PSSetConstantBuffers(1, 1, &_pMaterialBuffer);
    pContext->PSSetConstantBuffers(2, 1, &_pLightBuffer);

    pContext->IASetInputLayout(_pVertexLayout);
    pContext->VSSetShader(_pVertexShader, nullptr, 0);
    pContext->PSSetShader(_pPixelShader, nullptr, 0);

    pContext->PSSetShaderResources(0, 3, _pTextureRVs);
    pContext->PSSetSamplers(0, 1, &_pSamplerLinear);
}
