#include "d_DepthOfFieldEffect.h"

DepthOfFieldFX::DepthOfFieldFX() : 
    _pConstantBuffer(nullptr)
{
    for (int i = 0; i < RTTS; ++i)
    {
        _pCustomRenderTarget[i] = nullptr;
        _pCustomRTV[i] = nullptr;
        _pCustomSRV[i] = nullptr;
    }
}

DepthOfFieldFX::~DepthOfFieldFX()
{
    if (_pConstantBuffer) _pConstantBuffer->Release();

    for (int i = 0; i < RTTS; ++i)
    {
        if (_pCustomRenderTarget[i]) _pCustomRenderTarget[i]->Release();
        if (_pCustomRTV[i]) _pCustomRTV[i]->Release();
        if (_pCustomSRV[i]) _pCustomSRV[i]->Release();
    }
}

HRESULT DepthOfFieldFX::CreateShaders(ID3D11Device* pd3dDevice)
{
    return E_NOTIMPL;
}

HRESULT DepthOfFieldFX::CreateBuffers(ID3D11Device* pd3dDevice)
{
    return E_NOTIMPL;
}

HRESULT DepthOfFieldFX::CreateTextures(ID3D11Device* pd3dDevice)
{
    return E_NOTIMPL;
}


void DepthOfFieldFX::ClearRenderTargets(ID3D11DeviceContext* pContext)
{
}

void DepthOfFieldFX::SetConstantBuffer(ID3D11DeviceContext* pContext, int screenW, int screenH, int blur)
{
}

void DepthOfFieldFX::Render(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, int srvIndex)
{
}