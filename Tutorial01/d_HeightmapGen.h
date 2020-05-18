#pragma once

#include <vector>

struct Heightmap
{
	std::string filename;
	int width;
	int height;
};

class HeightmapGen
{
public:
	void LoadHeightmap(std::vector<float>& heights, Heightmap& heightmap);

	void FaultFormation(std::vector<float>& heights, const int texDim, const int iterations,
		float scale, bool constHeightShift);

	void DiamondSquare(std::vector<float>& heights, int texPower, int gridDim, float heightScale, float randScale = 1.0f);

	void CircleHill(std::vector<float>& heights, const int texDimX, const int texDimY, const int iterations,
		float scale, int radMin, int radMax,
		bool shouldFlatten = false, int flattenPower = 2);
};