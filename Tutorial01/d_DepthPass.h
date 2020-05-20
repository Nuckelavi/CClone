#pragma once

#include "d_BaseFX.h"

class DepthPass : public BaseFX
{
private:
	ID3D11Buffer* _pConstantBuffer;
	ID3D11Texture2D* _pCustomRenderTarget;
	ID3D11RenderTargetView* _pCustomRTV;
	ID3D11ShaderResourceView* _pCustomSRV;

protected:
	HRESULT CreateShaders(ID3D11Device* pd3dDevice);
	HRESULT CreateBuffers(ID3D11Device* pd3dDevice);
	HRESULT CreateTextures(ID3D11Device* pd3dDevice);

public:
	DepthPass();
	~DepthPass();

	void SetConstantBuffer(ID3D11DeviceContext* pContext,
		DirectX::XMMATRIX* world, DirectX::XMMATRIX* view, DirectX::XMMATRIX* proj,
		float nearDepth, float farDepth);
	void Render(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	void CreateShaderResource(ID3D11Device* pd3dDevice);
	void SetPSResource(ID3D11DeviceContext* pContext);

	ID3D11RenderTargetView* GetCustomRTV() { return _pCustomRTV; }
	ID3D11RenderTargetView** ppGetCustomRTV() { return &_pCustomRTV; }
	ID3D11ShaderResourceView* GetCustomSRV() { return _pCustomSRV; }
};