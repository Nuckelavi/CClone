#pragma once

#include "d_BaseFX.h"

class GaussianBlurFX : public BaseFX
{
private:
	ID3D11Buffer* _pConstantBuffer;

	static constexpr int RTTS = 2;
	ID3D11Texture2D* _pCustomRenderTarget[RTTS];
	ID3D11RenderTargetView* _pCustomRTV[RTTS];
	ID3D11ShaderResourceView* _pCustomSRV[RTTS];


protected:
	HRESULT CreateShaders(ID3D11Device* pd3dDevice);
	HRESULT CreateBuffers(ID3D11Device* pd3dDevice);
	HRESULT CreateTextures(ID3D11Device* pd3dDevice);

public:
	GaussianBlurFX();
	~GaussianBlurFX();

	void ClearRenderTargets(ID3D11DeviceContext* pContext);
	void SetConstantBuffer(ID3D11DeviceContext* pContext,
		int screenW, int screenH, int blur);
	void Render(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, int srvIndex);

	ID3D11RenderTargetView* GetCustomRTV(int index);
	ID3D11RenderTargetView** ppGetCustomRTV() { return _pCustomRTV; }
	ID3D11ShaderResourceView** ppGetCustomSRV() { return _pCustomSRV; }
};