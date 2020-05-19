#pragma once

#include "d_BaseFX.h"

class SurfaceDetailFX : public BaseFX
{
private:
	ID3D11Buffer* _pConstantBuffer;
	ID3D11Buffer* _pLightBuffer;
	ID3D11Buffer* _pMaterialBuffer;
	ID3D11SamplerState* _pSamplerLinear;

	static constexpr int TEX_NUM = 3;
	ID3D11ShaderResourceView** _pTextureRVs;

protected:
	HRESULT CreateShaders(ID3D11Device* pd3dDevice);
	HRESULT CreateBuffers(ID3D11Device* pd3dDevice);
	HRESULT CreateTextures(ID3D11Device* pd3dDevice);

public:
	SurfaceDetailFX();
	~SurfaceDetailFX();

	void SetConstantBuffers(ID3D11DeviceContext* pContext, 
		DirectX::XMMATRIX* world, DirectX::XMMATRIX* view, DirectX::XMMATRIX* proj, 
		const DirectX::XMFLOAT4& lightPos, const DirectX::XMFLOAT4& camPos, const Light& light, int effect);

	void Render(ID3D11DeviceContext* pContext);

};