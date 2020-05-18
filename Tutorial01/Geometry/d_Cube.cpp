#include "d_Cube.h"
#include "../DDSTextureLoader.h"

using namespace DirectX;

Cube::Cube()
{
	NUM_VERTICES = 24;
	NUM_INDICES = 36;
}

Cube::~Cube()
{
}

HRESULT Cube::InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	_vertices.clear();
	_vertices.resize(NUM_VERTICES);
	
	_vertices[0] = { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };
	_vertices[1] = { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
	_vertices[2] = { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	_vertices[3] = { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };

	_vertices[4] = { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
	_vertices[5] = { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };
	_vertices[6] = { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };
	_vertices[7] = { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };

	_vertices[8] = { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	_vertices[9] = { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };
	_vertices[10] = { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };
	_vertices[11] = { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };

	_vertices[12] = { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };
	_vertices[13] = { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	_vertices[14] = { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
	_vertices[15] = { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };

	_vertices[16] = { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) };
	_vertices[17] = { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) , XMFLOAT2(1.0f, 1.0f) };
	_vertices[18] = { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) };
	_vertices[19] = { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) };

	_vertices[20] = { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) };
	_vertices[21] = { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) };
	_vertices[22] = { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
	_vertices[23] = { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) };


	_indices.clear();
	_indices.resize(NUM_INDICES);

	_indices[0] = 3;
	_indices[1] = 1;
	_indices[2] = 0;
	_indices[3] = 2;
	_indices[4] = 1;
	_indices[5] = 3;

	_indices[6] = 6;
	_indices[7] = 4;
	_indices[8] = 5;
	_indices[9] = 7;
	_indices[10] = 4;
	_indices[11] = 6;

	_indices[12] = 11;
	_indices[13] = 9;
	_indices[14] = 8;
	_indices[15] = 10;
	_indices[16] = 9;
	_indices[17] = 11;

	_indices[18] = 14;
	_indices[19] = 12;
	_indices[20] = 13;
	_indices[21] = 15;
	_indices[22] = 12;
	_indices[23] = 14;

	_indices[24] = 19;
	_indices[25] = 17;
	_indices[26] = 16;
	_indices[27] = 18;
	_indices[28] = 17;
	_indices[29] = 19;

	_indices[30] = 22;
	_indices[31] = 20;
	_indices[32] = 21;
	_indices[33] = 23;
	_indices[34] = 20;
	_indices[35] = 22;


	//SimpleVertex* pVertices = _vertices.data();
	//UINT* pIndices = _indices.data();

	HRESULT hr;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * NUM_VERTICES;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = _vertices.data();//pVertices;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	SetVertexBuffer(pContext);

	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * NUM_INDICES;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData = {};
	InitData.pSysMem = _indices.data();//pIndices;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	SetIndexBuffer(pContext);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/*hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\stone.dds", nullptr, &_pTextureResourceView);
	if (FAILED(hr))
		return hr;*/


	return hr;
}
