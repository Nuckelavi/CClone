//--------------------------------------------------------------------------------------
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

// the lighting equations in this code have been taken from https://www.3dgep.com/texturing-lighting-directx-11/
// with some modifications by David White

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix mInvWVP;
	matrix mPrevVP;
	matrix mInvProj;

	float2 vScreenSize;
	int blur;
	int iEffectID;

	float fDepth;
	int iPassIndex;
	float2 pad0;
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

/*
float4 BoxBlur(float2 texCoord)
{
	//I know to get more blurred effect, you're meant to use multiple iterations, however... 
	//just dividing screensize by 2 gives similar effects
	float screenReduce = 2.0f;
	float xOffset = 1.0f / (vScreenSize.x / screenReduce);
	float yOffset = 1.0f / (vScreenSize.y / screenReduce);

	//9 surrounding pixels
	float4 topLeft = txDiffuse.Sample(samLinear, float2(texCoord.x - xOffset, texCoord.y - yOffset));
	float4 topMid = txDiffuse.Sample(samLinear, float2(texCoord.x, texCoord.y - 1.0f));
	float4 topRight = txDiffuse.Sample(samLinear, float2(texCoord.x + xOffset, texCoord.y - yOffset));

	float4 midLeft = txDiffuse.Sample(samLinear, float2(texCoord.x - xOffset, texCoord.y));
	float4 midMid = txDiffuse.Sample(samLinear, texCoord);
	float4 midRight = txDiffuse.Sample(samLinear, float2(texCoord.x + xOffset, texCoord.y));

	float4 bottomLeft = txDiffuse.Sample(samLinear, float2(texCoord.x - xOffset, texCoord.y + yOffset));
	float4 bottomMid = txDiffuse.Sample(samLinear, float2(texCoord.x, texCoord.y + yOffset));
	float4 bottomRight = txDiffuse.Sample(samLinear, float2(texCoord.x + xOffset, texCoord.y + yOffset));

	//final colour
	float4 col = (topLeft + topMid + topRight + midLeft + midMid + midRight + bottomLeft + bottomMid + bottomRight) / 9.0f;

	return col;
}

float4 GaussianBlur(float2 texCoord, bool horizontal)
{
	float weight[] = { 0.9f, 0.55f, 0.18f, 0.1f };
	float sum = 0.0f;
	for (int i = 0; i < 4; ++i)
	{
		sum += weight[i];
	}
	sum += 1.0f;
	for (int i = 0; i < 4; ++i)
	{
		weight[i] /= sum;
	}

	float2 texOffset = 1.0f / (vScreenSize / 2.0f);

	float4 centrePixel = txDiffuse.Sample(samLinear, texCoord) * weight[0];

	for (int i = 1; i < 4; ++i)
	{
		float2 offset = float2(0.0f, 0.0f);

		if (horizontal)
		{
			offset = float2(texOffset.x * i, 0.0f);
		}
		else
		{
			offset = float2(0.0f, texOffset.y * i);
		}

		centrePixel += txDiffuse.Sample(samLinear, texCoord - offset) * weight[i];
		centrePixel += txDiffuse.Sample(samLinear, texCoord + offset) * weight[i];
	}

	return centrePixel;
}
*/


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

	output.Pos = input.Pos;
	output.Tex = input.Tex;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS(PS_INPUT IN) : SV_TARGET
{
	float4 texColor = {1, 1, 1, 1};
	//texColor = txDiffuse.Sample(samLinear, IN.Tex);

	//blue tint
	/*texColor.b *= 5.0f;
	if (texColor.b >= 1.0f) { texColor.b = 1.0f; }*/

	return texColor;
}
