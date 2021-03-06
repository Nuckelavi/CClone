#pragma once

#include <Windows.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include <vector>

#include "..\structures.h"
#include "..\d_Utilities.h"



class Quad
{
private:
	/*static constexpr int NUM_VERTICES = 4;
	static constexpr int NUM_INDICES = 6;*/
	


	DirectX::XMMATRIX _world;
	DirectX::XMFLOAT3 _position;
	float pad;

	static constexpr int NUM_VERTICES = 4;
	static constexpr int NUM_INDICES = 6;
	

	ID3D11Buffer* _pVertexBuffer;
	ID3D11Buffer* _pIndexBuffer;

	std::vector<QuadVertex> _vertices;
	std::vector<UINT> _indices;
	
	

public:
	Quad();
	~Quad();

	HRESULT	InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	void Update(float t);
	void Draw(ID3D11DeviceContext* pContext);

	void SetPosition(DirectX::XMFLOAT3& position);
	void SetVertexBuffer(ID3D11DeviceContext* pContext);
	void SetIndexBuffer(ID3D11DeviceContext* pContext);

	DirectX::XMMATRIX* GetWorld() { return &_world; }
};