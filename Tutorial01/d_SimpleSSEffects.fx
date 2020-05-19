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
	float2 vScreenSize;
	int nEffectID;
	int nBlur;
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

float4 Grayscale(float4 col)
{
	float gray = dot(float3(col.r, col.g, col.b), float3(0.3f, 0.59f, 0.11f));
	return float4(gray, gray, gray, col.a);
}

float4 BlueTint(float4 col)
{
	col.b *= 5.0f;
	if (col.b >= 1.0f) { col.b = 1.0f; }
	return col;
}

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
	
	//blue tint
	//texColor = txDiffuse.Sample(samLinear, IN.Tex);
	//return BlueTint(texColor);

	if (nEffectID == 0)
	{
		//grayscale
		texColor = txDiffuse.Sample(samLinear, IN.Tex);
		return Grayscale(texColor);
	}
	
	if (nEffectID == 1)
	{
		return BoxBlur(IN.Tex);
	}

	return texColor;
}
