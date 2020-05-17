#include "d_Quad.h"

using namespace DirectX;

Quad::Quad() :
	_world(XMMatrixIdentity()),
	_position(0, 0, 0),
	_pVertexBuffer(nullptr),
	_pIndexBuffer(nullptr)
{
}

Quad::~Quad()
{
	if (_pVertexBuffer)
		_pVertexBuffer->Release();

	if (_pIndexBuffer)
		_pIndexBuffer->Release();
}

HRESULT Quad::InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	_vertices.clear();
	_vertices.resize(NUM_VERTICES);

	_vertices[0] = { XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
	_vertices[1] = { XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
	_vertices[2] = { XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f) };
	_vertices[3] = { XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f) };

	HRESULT hr;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(QuadVertex) * NUM_VERTICES;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = _vertices.data();
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);
	if (FAILED(hr))
		return hr;


	SetVertexBuffer(pContext);


	//set quad index buffer
	_indices.clear();
	_indices.resize(NUM_INDICES, 0);

	_indices[0] = 0;
	_indices[1] = 1;
	_indices[2] = 2;
	_indices[3] = 2;
	_indices[4] = 1;
	_indices[5] = 3;
	/*_indices[0] = 0;
	_indices[1] = 3;
	_indices[2] = 2;
	_indices[3] = 1;
	_indices[4] = 3;
	_indices[5] = 0;*/

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * NUM_INDICES;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = _indices.data();
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	SetIndexBuffer(pContext);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Quad::Update(float t)
{
}

void Quad::Draw(ID3D11DeviceContext* pContext)
{
	pContext->DrawIndexed(NUM_INDICES, 0, 0);
}

void Quad::SetPosition(DirectX::XMFLOAT3& position)
{
	_position = position;
}

void Quad::SetVertexBuffer(ID3D11DeviceContext* pContext)
{
	UINT offset = 0;
	UINT stride = sizeof(QuadVertex);
	pContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
}

void Quad::SetIndexBuffer(ID3D11DeviceContext* pContext)
{
	pContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}
