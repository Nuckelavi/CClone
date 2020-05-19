#pragma once

#include "d_Utilities.h"
#include "structures.h"

class GaussianBlurFX
{
private:
	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;
	ID3D11InputLayout* _pVertexLayout;
	ID3D11Buffer* _pConstantBuffer;

	static constexpr int RTTS = 2;
	ID3D11Texture2D* _pCustomRenderTarget[RTTS];
	ID3D11RenderTargetView* _pCustomRTV[RTTS];
	ID3D11ShaderResourceView* _pCustomSRV[RTTS];

public:
	GaussianBlurFX();
	~GaussianBlurFX();

	HRESULT SetupShader(ID3D11Device* pd3dDevice);
	HRESULT CreateShaders(ID3D11Device* pd3dDevice);
	HRESULT CreateBuffers(ID3D11Device* pd3dDevice);
	HRESULT CreateTextures(ID3D11Device* pd3dDevice);

	void SetConstantBuffer(ID3D11DeviceContext* pContext,
		int screenW, int screenH, int effect);
	void Render(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);

	ID3D11RenderTargetView* GetCustomRTV(int index);
	ID3D11RenderTargetView** ppGetCustomRTV();

};