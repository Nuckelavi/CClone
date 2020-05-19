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
//--------------------------------------------------------------------------------------


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
	texColor = txDiffuse.Sample(samLinear, IN.Tex);

	if (nBlur == 0)
	{
		return GaussianBlur(IN.Tex, true);
	}
	if(nBlur == 1)
	{
		return GaussianBlur(IN.Tex, false);
	}

	return texColor;
}
