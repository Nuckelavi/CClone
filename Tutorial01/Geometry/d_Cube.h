#pragma once

#include "d_Mesh.h"

class Cube : public Mesh
{
private:

public:
	Cube();
	~Cube();

	HRESULT InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
};