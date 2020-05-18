#pragma once

#include <vector>
#include <string>

struct Heightmap
{
	std::string filename;
	int width;
	int height;
};

class HeightmapGen
{
private:

public:
	void LoadHeightmap(std::vector<float>& heights, Heightmap& heightmap);

	void FaultFormation(std::vector<float>& heights, const int texDim, const int iterations, 
		bool constHeightShift, bool randomSide = false);

	void DiamondSquare(std::vector<float>& heights, int texPower, int gridDim, float randScale = 1.0f);

	void CircleHill(std::vector<float>& heights, const int texDimX, const int texDimY, const int iterations,
		int radMin, int radMax, bool shouldFlatten = false, int flattenPower = 2);
};