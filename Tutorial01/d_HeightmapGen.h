#pragma once

#include <vector>

class HeightmapGen
{
private:

public:
	void LoadHeightmap(std::vector<float>& heightmap);

	void FaultFormation(std::vector<float>& heightmap, const int texDim, const int iterations,
		float scale, bool constHeightShift);

	void DiamondSquare(std::vector<float>& scaledHeights, int texPower, int gridDim, float heightScale, float randScale = 1.0f);

	void CircleHill(std::vector<float>& heightmap, const int texDimX, const int texDimY, const int iterations,
		float scale, int radMin, int radMax,
		bool shouldFlatten = false, int flattenPower = 2);
};