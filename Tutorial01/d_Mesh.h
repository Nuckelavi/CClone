#pragma once

#include <Windows.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include <vector>

#include "structures.h"

class Mesh
{
protected:
	int NUM_VERTICES = 0;
	int NUM_INDICES = 0;
	/*const int NUM_VERTICES = 0;
	const int  NUM_INDICES = 0;*/
	DirectX::XMFLOAT2 pad = { 0, 0 };

	DirectX::XMMATRIX _world;
	DirectX::XMMATRIX _translate;
	DirectX::XMMATRIX _rotate;
	DirectX::XMMATRIX _scale;
	DirectX::XMFLOAT3 _position;
	float pad0;

	ID3D11Buffer* _pVertexBuffer;
	ID3D11Buffer* _pIndexBuffer;
	ID3D11ShaderResourceView* _pTextureResourceView;
	/*ID3D11SamplerState* _pSamplerState;
	MaterialPropertiesConstantBuffer _material;*/

	std::vector<SimpleVertex> _vertices;
	std::vector<UINT> _indices;

public:
	Mesh();
	~Mesh();

	HRESULT InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	void VertexNormalAveraging();
	void Update(float t);
	void Draw(ID3D11DeviceContext* pContext);

	//set
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetTranslation(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	void SetVertexBuffer(ID3D11DeviceContext* pContext);
	void SetIndexBuffer(ID3D11DeviceContext* pContext);

	//get
	DirectX::XMMATRIX* GetWorld() { return &_world; }

};