#pragma once

#include "d_Mesh.h"

class Cube : public Mesh
{
private:
	/*static constexpr int NUM_VERTICES = 24;
	static constexpr int NUM_INDICES = 36;
	DirectX::XMFLOAT2 pad = { 0, 0 };*/
	 
public:
	Cube();
	~Cube();

	HRESULT InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
};