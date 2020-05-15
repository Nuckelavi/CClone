#pragma once

#include "d_Mesh.h"

class Quad : public Mesh
{
private:

public:
	Quad();
	~Quad();

	HRESULT InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
};