#pragma once

#include "d_Mesh.h"

class Grid : public Mesh
{
private:
	static constexpr int CELLWIDTH = 1.0f;
	static constexpr int CELLHEIGHT = 1.0f;

	int _width;
	int _height;

public:
	Grid();
	~Grid();

	void Setup(int width, int height);
	HRESULT InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext,
		std::vector<float> heights);

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
};