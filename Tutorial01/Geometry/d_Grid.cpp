#include "d_Grid.h"

using namespace DirectX;

Grid::Grid() : 
	_width(0),
	_height(0)
{
}

Grid::~Grid()
{
}

void Grid::Setup(int width, int height)
{
	_width = width;
	_height = height;
	NUM_VERTICES = _width * _height;
	NUM_INDICES = (_width - 1) * (_height - 1) * 6;
	_world = XMMatrixIdentity();

	_translate = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	_rotate = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	_scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	Update(0.0f);
}

HRESULT Grid::InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext,
	std::vector<float> heights)
{
	_vertices.clear();
	_vertices.resize(NUM_VERTICES);

	for (int y = 0; y < _height; ++y)
	{
		for (int x = 0; x < _width; ++x)
		{
			XMFLOAT3 vertPos;
			vertPos.x = (float)x * CELLWIDTH + (-(float)_width * 0.5f);
			vertPos.y = 0.0f;
			vertPos.z = -((float)y * CELLHEIGHT) + ((float)_height * 0.5f);

			/*if (heights.size() > _vertices.size())
			{
				vertPos.y = heights[_vertices.size()];
			}*/

			XMFLOAT3 vertNorm = XMFLOAT3(0.0f, 1.0f, 0.0f);

			XMFLOAT2 vertTex;
			vertTex.x = (float)x * CELLWIDTH / (float)_width;
			vertTex.y = (float)y * CELLHEIGHT / (float)_height;

			_vertices[y * _width + x] = SimpleVertex{ vertPos, vertNorm, vertTex };
		}
	}

	for (int y = 0; y < _height - 1; ++y)
	{
		for (int x = 0; x < _width - 1; ++x)
		{
			_indices.push_back(x * _height + y);
			_indices.push_back(x * _height + y + 1);
			_indices.push_back((x + 1) * _height + y);

			_indices.push_back((x + 1) * _height + y);
			_indices.push_back(x * _height + y + 1);
			_indices.push_back((x + 1) * _height + y + 1);
		}
	}

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * NUM_VERTICES;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = _vertices.data();
	HRESULT hr = pd3dDevice->CreateBuffer(&bd, &initData, &_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	SetVertexBuffer(pContext);

	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * NUM_INDICES;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	initData = {};
	initData.pSysMem = _indices.data();
	hr = pd3dDevice->CreateBuffer(&bd, &initData, &_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	SetIndexBuffer(pContext);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	return hr;
}
