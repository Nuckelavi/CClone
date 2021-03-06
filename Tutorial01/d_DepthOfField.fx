
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix mInvProj;
}

Texture2D txDiffuse : register(t0);
Texture2D txDepth : register(t1);
Texture2D txNoBlur : register(t2);
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
	float2 Depth : TEXCOORD0;
};
//--------------------------------------------------------------------------------------


float4 DepthOfField(float2 texCoord, float zDepth)
{
	float4 finCol = { 0, 0, 0, 1 };

	float4 noBlurTex = txNoBlur.Sample(samLinear, texCoord);
	float4 blurTex = txDiffuse.Sample(samLinear, texCoord);

	zDepth = saturate(zDepth);

	finCol = zDepth * blurTex + (1.0f - zDepth) * noBlurTex;

	return finCol;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;

	output.Pos = input.Pos;
	output.Tex = input.Tex;

	//depth -----
	float4 viewPos = mul(input.Pos, mInvProj);
	output.Depth.x = viewPos.z / 100.0f;
	output.Depth.y = 0.0f;
	//-----------

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS(PS_INPUT IN) : SV_TARGET
{
	float4 texColor = { 1, 1, 1, 1 };
	texColor = txDiffuse.Sample(samLinear, IN.Tex);

	float4 depth = { 0, 0, 0, 1 };
	depth = txDepth.Sample(samLinear, IN.Tex);

	texColor = DepthOfField(IN.Tex, depth.z);

	return texColor;
}
