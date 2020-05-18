#include "d_HeightmapGen.h"

#include <random>
#include <ctime>
#include <fstream>

void HeightmapGen::LoadHeightmap(std::vector<float>& heights, Heightmap& heightmap)
{
    //using namespace std;
    const int heigthmapLength = heightmap.width * heightmap.height;

    std::vector<unsigned char> in;
    in.resize(heigthmapLength, '0');

    std::ifstream inFile(heightmap.filename, std::ios_base::binary);

    if (inFile)
    {
        inFile.read((char*)&in[0], (std::streamsize)(in.size()));

        inFile.close();
    }

    int heightmapSize = heightmap.width * heightmap.height;
    heights.clear();
    heights.resize(heightmapSize, 0.0f);

    for (int i = 0; i < heigthmapLength; ++i)
    {
        heights[i] = (in[i] / 255.0f); //* heightmap.scale);
    }
}

void HeightmapGen::FaultFormation(std::vector<float>& heights, const int texDim, const int iterations, float scale, bool constHeightShift)
{
    const int texSize = texDim * texDim;
    heights.clear();
    heights.resize(texSize, 0.0f);

    std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
    std::uniform_int_distribution<> randTex{ 0, texSize - 1 };
    std::uniform_int_distribution<> randHeight{ 1, (int)iterations };


    float offset = 1.0f;
    if (constHeightShift)
    {
        offset = 2.0f * scale / (float)iterations;
    }

    for (int i = 0; i < iterations; ++i)
    {
        int A, B;
        //make sure B%texDim != A%texDim, because otherwise 
        //you're dividing with 0, when calculating the value of "m"
        do
        {
            A = randTex(mersenne);
            B = randTex(mersenne);
        } while (B % texDim == A % texDim);
        int ax = A % texDim;
        int ay = A / texDim;
        int bx = B % texDim;
        int by = B / texDim;

        //variables for "y = mx + c" line equation
        //hence the terrible names
        float m = (float)(by - ay) / (float)(bx - ax);
        float c = (float)ay - m * (float)ax;

        //if you don't want to have a constant shift in height,
        //just randomly offset variables, by no more than 0.5 though
        if (!constHeightShift)
        {
            offset = 0.5f * scale / (float)randHeight(mersenne);
        }

        for (int y = 0; y < texDim; ++y)
        {
            float cutoff = ((float)y - c) / m;

            for (int x = 0; x < texDim; ++x)
            {

                if (x > cutoff)
                {
                    heights[y * texDim + x] += offset;
                }
                else
                {
                    heights[y * texDim + x] -= offset;
                }
            }
        }

    }
}

void HeightmapGen::DiamondSquare(std::vector<float>& heights, int texPower, int gridDim, float heightScale, float randScale)
{
    const int n = 2;
    int texDim = pow(2, texPower) + 1;
    const int texSize = texDim * texDim;

    std::vector<float> heightmap;
    heightmap.resize(texSize, 0.0f);

    std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    //seed initial values
    float initVal = 1.0f;
    heightmap[0] = initVal;
    heightmap[texDim - 1] = initVal;
    heightmap[texDim * (texDim - 1)] = initVal;
    heightmap[texDim * texDim - 1] = initVal;

    //perform diamond square
    for (int i = 0; i < n; ++i)
    {
        int sideLength = pow(2, n - i);
        //std::uniform_real_distribution<float> randHeight(-pow(2, -i) * randScale, pow(2, -i) * randScale);
        std::uniform_real_distribution<float> randHeight(-pow(randScale, -i), pow(randScale, -i));

        for (int y = 0; y < (texDim - 1) / sideLength; ++y)
        {
            for (int x = 0; x < (texDim - 1) / sideLength; ++x)
            {
                //corner values:
                float topLeft = heightmap[y * sideLength + x];
                float topRight = heightmap[y * sideLength + x + sideLength];
                float bottomLeft = heightmap[(y * sideLength) * sideLength + x];
                float bottomRight = heightmap[(y * sideLength) * sideLength + x * sideLength];

                //for each square, average corners
                float cornerAvg = (topLeft + topRight + bottomLeft + bottomRight) / 4.0f;

                //do diamond step: add random value to corner average, assign to centre
                float centre = cornerAvg + randHeight(mersenne);
                heightmap[(y * sideLength + sideLength / 2) * texDim + (x * sideLength + (sideLength / 2))] = centre;

                //do square step: find diamond centres and average corner diamond values
                //LEFT = TL + BL + C
                float randDia = randHeight(mersenne);
                float leftDia = (topLeft + bottomLeft + centre) / 3.0f + randHeight(mersenne);
                //RIGHT = TR + BR + C
                float rightDia = (topRight + bottomRight + centre) / 3.0f + randHeight(mersenne);
                //TOP = TL + TR + C
                float topDia = (topLeft + topRight + centre) / 3.0f + randHeight(mersenne);
                //BOTTOM = BL + BR + C
                float bottomDia = (bottomLeft + bottomRight + centre) / 3.0f + randHeight(mersenne);

                //Tex[(y * sideLength + sideLength / 2) * texWidth + (x * sideLength + (sideLength / 2))];
                heightmap[(y * sideLength) * texDim + (x * sideLength + (sideLength / 2))] = topDia;
                heightmap[(y * sideLength + sideLength) * texDim + (x * sideLength + (sideLength / 2))] = bottomDia;
                heightmap[(y * sideLength + sideLength / 2) * texDim + (x * sideLength)] = leftDia;
                heightmap[(y * sideLength + sideLength / 2) * texDim + (x * sideLength + sideLength)] = rightDia;
            }
        }

        //you need to reduce the random range values
    }

    //normalize
    float minHeight = heightmap[0];
    float maxHeight = heightmap[0];
    for (int i = 0; i < heightmap.size(); ++i)
    {
        if (heightmap[i] < minHeight)
        {
            minHeight = heightmap[i];
        }
        else if (heightmap[i] > maxHeight)
        {
            maxHeight = heightmap[i];
        }
    }

    heights.clear();
    heights.resize(gridDim * gridDim, 0.0f);
    float maxHeightDiff = maxHeight - minHeight;
    int count = 0;
    for (int i = 0; i < heightmap.size(); ++i)
    {
        //heightmap[i] *= heightScale / maxHeightDiff;

        if (count >= heights.size())
            break;

        if (i % texDim < gridDim)
        {
            heights[count] = heightmap[i] * heightScale / maxHeightDiff;
            ++count;
        }

        /*
        if (i % _heightmap.width >= _terrainGrid.GetWidth())
        {
            continue;
        }

        _scaledHeights.push_back(_heightmapValues[i] * _heightmap.scale);
        */
    }
}

void HeightmapGen::CircleHill(std::vector<float>& heights, const int texDimX, const int texDimY, const int iterations, float scale, int radMin, int radMax, bool shouldFlatten, int flattenPower)
{
    const int texSize = texDimX * texDimY;
    heights.clear();
    heights.resize(texSize, 0.0f);

    std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
    std::uniform_int_distribution<> randPoint{ 0, texSize - 1 };
    std::uniform_int_distribution<> randPointX{ 0, texDimX - 1 };
    std::uniform_int_distribution<> randPointY{ 0, texDimY - 1 };
    std::uniform_int_distribution<> randRadius{ radMin, radMax };

    for (int i = 0; i < iterations; ++i)
    {
        int ox = randPointX(mersenne);
        int oy = randPointY(mersenne);

        int radius = randRadius(mersenne);

        for (int y = std::max(0, oy - radius); y <= std::min(texDimY - 1, oy + radius); ++y)
        {
            for (int x = std::max(0, ox - radius); x <= std::min(texDimX - 1, ox + radius); ++x)
            {
                float height = (float)(radius * radius -
                    (x - ox) * (x - ox) - (y - oy) * (y - oy));

                if (height < 0.0f)
                {
                    continue;
                }
                else
                {
                    heights[y * texDimX + x] += height;
                }
            }
        }

    }

    float maxHeight = -1.0f;
    for (int i = 0; i < heights.size(); ++i)
    {
        if (heights[i] > maxHeight)
        {
            maxHeight = heights[i];
        }
    }

    for (int i = 0; i < heights.size(); ++i)
    {
        if (maxHeight <= 0.0f)
        {
            break;
        }

        heights[i] /= maxHeight;

        //flatten
        if (shouldFlatten)
        {
            heights[i] = pow(heights[i], flattenPower);
        }

        heights[i] *= scale;
    }
}