#pragma once

#include "d_Utilities.h"
#include "structures.h"

class BaseFX
{
protected:
	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;
	ID3D11InputLayout* _pVertexLayout;

protected:
	virtual HRESULT CreateShaders(ID3D11Device* pd3dDevice) = 0;
	virtual HRESULT CreateBuffers(ID3D11Device* pd3dDevice) = 0;
	virtual HRESULT CreateTextures(ID3D11Device* pd3dDevice) = 0;

public:
	BaseFX();
	~BaseFX();

	HRESULT SetupShader(ID3D11Device* pd3dDevice);

};