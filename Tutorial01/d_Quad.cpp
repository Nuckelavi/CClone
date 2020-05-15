#include "d_Quad.h"

using namespace DirectX;

Quad::Quad()
{
	NUM_VERTICES = 4;
	NUM_INDICES = 6;
}

Quad::~Quad()
{
}

HRESULT Quad::InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	//_vertices.clear();
	//_vertices.resize(NUM_VERTICES);

	//_vertices[0] = { XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
	//_vertices[1] = { XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
	//_vertices[2] = { XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
	//_vertices[3] = { XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f) };

	//D3D11_BUFFER_DESC bd2 = {};
	//bd2.Usage = D3D11_USAGE_DEFAULT;
	//bd2.ByteWidth = sizeof(QuadVertex) * 4;
	//bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd2.CPUAccessFlags = 0;

	//D3D11_SUBRESOURCE_DATA InitData2 = {};

	//ZeroMemory(&InitData2, sizeof(InitData2));
	//InitData2.pSysMem = quadVertices;
	//hr = g_pd3dDevice->CreateBuffer(&bd2, &InitData2, &g_quadVertexBuffer);
	//if (FAILED(hr))
	//	return hr;

	//UINT stride2 = sizeof(QuadVertex);
	//g_pImmediateContext->IASetVertexBuffers(0, 1, &g_quadVertexBuffer, &stride2, &offset);

	//return E_NOTIMPL;

	////set quad index buffer
	//unsigned int quadIndices[] =
	//{
	//	0, 1, 2,
	//	2, 1, 3
	//	//0, 1, 2, 3
	//};
	//ZeroMemory(&bd2, sizeof(bd2));
	//bd2.Usage = D3D11_USAGE_DEFAULT;
	//bd2.ByteWidth = sizeof(unsigned int) * 6;
	//bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bd2.CPUAccessFlags = 0;

	//ZeroMemory(&InitData2, sizeof(InitData2));
	//InitData2.pSysMem = quadIndices;
	//hr = g_pd3dDevice->CreateBuffer(&bd2, &InitData2, &g_quadIndexBuffer);
	//if (FAILED(hr))
	//	return hr;
	return NULL;
}
