#include "d_Mesh.h"

using namespace DirectX;

Mesh::Mesh() : 
	_world(XMMatrixIdentity()),
	_translate(XMMatrixIdentity()),
	_rotate(XMMatrixIdentity()),
	_position(0.0f, 0.0f, 0.0f), 
	pad0(0.0f),
	_pVertexBuffer(nullptr),
	_pIndexBuffer(nullptr)
{
	_scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
}

Mesh::~Mesh()
{
	if (_pVertexBuffer)
		_pVertexBuffer->Release();

	if (_pIndexBuffer)
		_pIndexBuffer->Release();

	if (_pTextureResourceView)
		_pTextureResourceView->Release();
}

HRESULT Mesh::InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	return E_NOTIMPL;
}

void Mesh::VertexNormalAveraging()
{
}

void Mesh::Update(float t)
{
	_world = _scale * _rotate * _translate;
}

void Mesh::Draw(ID3D11DeviceContext* pContext)
{
	pContext->DrawIndexed(NUM_INDICES, 0, 0);
}

void Mesh::SetPosition(DirectX::XMFLOAT3& position)
{
	_position = position;
}

void Mesh::SetTranslation(float x, float y, float z)
{
	_translate = XMMatrixTranslation(x, y, z);
}

void Mesh::SetRotation(float x, float y, float z)
{
	_rotate = XMMatrixRotationRollPitchYaw(x, y, z);
}

void Mesh::SetScale(float x, float y, float z)
{
	_scale = XMMatrixScaling(x, y, z);
}

void Mesh::SetVertexBuffer(ID3D11DeviceContext* pContext)
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
}

void Mesh::SetIndexBuffer(ID3D11DeviceContext* pContext)
{
	pContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}
