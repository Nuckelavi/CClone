
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix mWorld;
	matrix mView;
	matrix mProjection;

	float fNearDepth;
	float fFarDepth;
	float2 pad0;
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float4 viewPos : POSITION;
};
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, mWorld);
	output.Pos = mul(output.Pos, mView);
	output.viewPos = output.Pos;
	output.Pos = mul(output.Pos, mProjection);

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS(PS_INPUT IN) : SV_TARGET
{
	float depth = (IN.viewPos.z - fNearDepth) / fFarDepth;

	//scaled the depth to make DOF more visible
	depth *= 10.0f;

	return float4(depth, depth, depth, 1.0f);
}