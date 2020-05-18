#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>

#include "d_HeightmapGen.h"
#include "Geometry/d_Grid.h"

class GridTerrain
{


private:
	Grid* _terrain;
	Heightmap _heightmap;
	HeightmapGen _hmGen;

	DirectX::XMFLOAT3 _position;
	float _hmScale;

	float _gridRatio;
	
	std::vector < const wchar_t*> _textures;
	std::vector<ID3D11ShaderResourceView*> _textureSRVs;

public:
	std::vector<float> _heights;




public:
	GridTerrain();
	~GridTerrain();

	void AdjustHMToGrid(bool shrinkHM);
	void SetupTerrain(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext,
		float scale, bool shrinkHM = false, bool smooth = false, int smoothAmount = 0);
	void Update(float t);
	void Draw(ID3D11DeviceContext* _pContext);

	void Smooth();
	float AverageNeighbours(int x, int y);
	bool IndexInBounds(int i, int j);

	void BuildHeightmapSRV(ID3D11Device* pd3dDevice);
	void LoadTextures(ID3D11Device* pd3dDevice);
	void AddTexture(const wchar_t* newTex);

	//set
	void SetHeightmap(int width, int height, std::string name);
	void SetGridRatio(float gridRatio);

	//get
	HeightmapGen& GetHmGen() { return _hmGen; }
	Heightmap& GetHMStruct() { return _heightmap; }
	Grid* GetTerrainGrid() { return _terrain; }
	float GetHMScale() { return _hmScale; }
	std::vector<float>& GetHeightValues() { return _heights; }


};