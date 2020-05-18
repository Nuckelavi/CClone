#include "d_GridTerrain.h"

void GridTerrain::SetHeightmap(int width, int height, std::string name)
{
	_heightmap.width = width;
	_heightmap.height = height;
	_heightmap.filename = name;
}
