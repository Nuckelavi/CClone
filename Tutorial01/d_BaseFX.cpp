#include "d_BaseFX.h"

BaseFX::BaseFX() :
	_pVertexLayout(nullptr),
	_pVertexShader(nullptr),
	_pPixelShader(nullptr)
{
}

BaseFX::~BaseFX()
{
	if (_pVertexShader) _pVertexShader->Release();
	if (_pPixelShader) _pPixelShader->Release();
	if (_pVertexLayout) _pVertexLayout->Release();
}

HRESULT BaseFX::SetupShader(ID3D11Device* pd3dDevice)
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
