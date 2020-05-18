#include "d_GridTerrain.h"

#include "DDSTextureLoader.h"

GridTerrain::GridTerrain() :
	_heightmap({ "", 0, 0, }),
	_hmScale(1.0f),
	_position(0.0f, 0.0f, 0.0f)

{
	_terrain = new Grid();
}

GridTerrain::~GridTerrain()
{
	delete _terrain;
	_terrain = nullptr;
}

void GridTerrain::AdjustHMToGrid()
{
	//consider doing this in hm gen
	//also make sure hm gen makes all the heights [0, 1] range
}

void GridTerrain::SetupTerrain(ID3D11Device* pd3dDevice, 
	ID3D11DeviceContext* pContext, float scale, bool smooth, int smoothAmount)
{
	//PREREQUISITES ----------------------------------
	//set heightmap constraints
	//load or generate heightmap values
	
	//ACTUAL FUNCTIONALITY ---------------------------
	//scale heightmap values
	_hmScale = scale;
	for (auto& height : _heights)
	{
		height *= _hmScale;
	}
	
	//smooth values
	if (smooth)
	{
		for (int i = 0; i < smoothAmount; ++i)
		{
			Smooth();
		}
	}

	//setup grid
	_terrain->Setup(_heightmap.width, _heightmap.height);
	_terrain->InitMesh(pd3dDevice, pContext, _heights);


}

void GridTerrain::Update(float t)
{
	_terrain->Update(t);
}

void GridTerrain::Draw(ID3D11DeviceContext* _pContext)
{
	_terrain->Draw(_pContext);
}

void GridTerrain::Smooth()
{
	std::vector<float> dest(_heights.size());
	for (UINT y = 0; y < _terrain->GetHeight(); ++y)
	{
		for (UINT x = 0; x < _terrain->GetWidth(); ++x)
		{
			dest[y * _terrain->GetWidth() + x] = AverageNeighbours(x, y);
		}
	}

	_heights = dest;
}

float GridTerrain::AverageNeighbours(int x, int y)
{
	float mean = 0.0f;
	float samples = 0.0f;

	for (int j = y - 1; j <= y + 1; ++j)
	{
		for (int i = x - 1; i <= x + 1; ++i)
		{
			if (IndexInBounds(i, j))
			{
				samples += 1.0f;
				mean += _heights[j * _terrain->GetWidth() + i];
			}
		}
	}

	return mean / samples;
}

bool GridTerrain::IndexInBounds(int i, int j)
{
	return ((i >= 0 && i < _terrain->GetWidth())
		&& (j >= 0 && j < _terrain->GetHeight()));
}

void GridTerrain::BuildHeightmapSRV(ID3D11Device* pd3dDevice)
{
	/*D3D11_TEXTURE2D_DESC hmd;
	hmd.Width = _heightmap.width;
	hmd.Height = _heightmap.height;
	hmd.MipLevels = 1;
	hmd.ArraySize = 1;
	hmd.Format = DXGI_FORMAT_R16_FLOAT;*/
}

void GridTerrain::LoadTextures(ID3D11Device* pd3dDevice)
{
	for (int i = 0; i < _textures.size(); ++i)
	{
		_textureSRVs.push_back(nullptr);
		DirectX::CreateDDSTextureFromFile(
			pd3dDevice, _textures[i], nullptr, &_textureSRVs[i]);
	}
}

void GridTerrain::AddTexture(const wchar_t* newTex)
{
	_textures.push_back(newTex);
}

void GridTerrain::SetHeightmap(int width, int height, std::string name)
{
	_heightmap.width = width;
	_heightmap.height = height;
	_heightmap.filename = name;
}
