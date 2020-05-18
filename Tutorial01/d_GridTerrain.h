#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>

#include "d_HeightmapGen.h"
#include "Geometry/d_Grid.h"

class GridTerrain
{
private:
	Heightmap _heightmap;
	HeightmapGen _hmGen;

	DirectX::XMFLOAT3 _position;
	float _hmScale;

	std::vector<ID3D11ShaderResourceView*> _textures;

public:

	void AdjustHMToGrid();
	void SetupTerrain();
	void Update(float t);
	void Draw();

	void Smooth();
	float AverageNeighbours(int x, int y);
	bool IndexInBounds(int i, int j);

	void BuildHeightmapSRV(ID3D11Device* pd3dDevice);
	void LoadTextures(ID3D11Device* pd3dDevice);
	void AddTexture(const wchar_t* newTex);

	//set
	void SetHeightmap(int width, int height, std::string name);

	//get
	const HeightmapGen& GetHmGen() const { return _hmGen; }
};