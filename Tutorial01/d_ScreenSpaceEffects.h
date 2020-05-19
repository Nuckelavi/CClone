#pragma once

#include "d_BaseFX.h"

class SSEffects : public BaseFX
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
	SSEffects();
	~SSEffects();

	void SetConstantBuffer(ID3D11DeviceContext* pContext, 
	int screenW, int screenH, int effect);
	void Render(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);

	ID3D11RenderTargetView* GetCustomRTV() { return _pCustomRTV; }
	ID3D11RenderTargetView** ppGetCustomRTV() { return &_pCustomRTV; }
};