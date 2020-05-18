#include "d_HeightmapGen.h"

void HeightmapGen::FaultFormation(std::vector<float>& heightmap, const int texDim, const int iterations, float scale, bool constHeightShift)
{
    const int texSize = texDim * texDim;
    heightmap.clear();
    heightmap.resize(texSize, 0.0f);

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
                    heightmap[y * texDim + x] += offset;
                }
                else
                {
                    heightmap[y * texDim + x] -= offset;
                }
            }
        }

    }
}

void HeightmapGen::DiamondSquare(std::vector<float>& scaledHeights, int texPower, int gridDim, float heightScale, float randScale)
{
}

void HeightmapGen::CircleHill(std::vector<float>& heightmap, const int texDimX, const int texDimY, const int iterations, float scale, int radMin, int radMax, bool shouldFlatten, int flattenPower)
{
}
