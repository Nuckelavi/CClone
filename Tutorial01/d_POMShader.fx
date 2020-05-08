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
	matrix World;
	matrix View;
	matrix Projection;

	float fHeightScale;
	int nMinSamples;
	int nMaxSamples;
	int nEffectID;

	/*float fNearDepth;
	float fFarDepth;
	float2 pad0;*/
}

Texture2D txDiffuse[3] : register(t0);
SamplerState samLinear : register(s0);

#define MAX_LIGHTS 1
// Light types.
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct _Material
{
	float4  Emissive;       // 16 bytes
							//----------------------------------- (16 byte boundary)
	float4  Ambient;        // 16 bytes
							//------------------------------------(16 byte boundary)
	float4  Diffuse;        // 16 bytes
							//----------------------------------- (16 byte boundary)
	float4  Specular;       // 16 bytes
							//----------------------------------- (16 byte boundary)
	float   SpecularPower;  // 4 bytes
	bool    UseTexture;     // 4 bytes
	float2  Padding;        // 8 bytes
							//----------------------------------- (16 byte boundary)
};  // Total:               // 80 bytes ( 5 * 16 )

cbuffer MaterialProperties : register(b1)
{
	_Material Material;
};

struct Light
{
	float4      Position;               // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4      Direction;              // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4      Color;                  // 16 bytes
										//----------------------------------- (16 byte boundary)
	float       SpotAngle;              // 4 bytes
	float       ConstantAttenuation;    // 4 bytes
	float       LinearAttenuation;      // 4 bytes
	float       QuadraticAttenuation;   // 4 bytes
										//----------------------------------- (16 byte boundary)
	int         LightType;              // 4 bytes
	bool        Enabled;                // 4 bytes
	int2        Padding;                // 8 bytes
										//----------------------------------- (16 byte boundary)
};  // Total:                           // 80 bytes (5 * 16)

cbuffer LightProperties : register(b2)
{
	float4 EyePosition;                 // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4 CameraPosition;              // 16 bytes
										//----------------------------------- (16 byte boundary)
	float4 GlobalAmbient;               // 16 bytes
										//----------------------------------- (16 byte boundary)
	Light Lights[MAX_LIGHTS];           // 80 * 8 = 640 bytes
}; 

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 viewPos : POSITION1;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
};


float4 DoDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L));
	return light.Color * NdotL;
}

float4 DoSpecular(Light lightObject, float3 vertexToEye, float3 lightDirectionToVertex, float3 Normal)
{
	float4 lightDir = float4(normalize(-lightDirectionToVertex),1);
	vertexToEye = normalize(vertexToEye);

	float lightIntensity = saturate(dot(Normal, lightDir));
	float4 specular = float4(0, 0, 0, 0);
	if (lightIntensity > 0.0f)
	{
		float3  reflection = normalize(2 * lightIntensity * Normal - lightDir);
		specular = pow(saturate(dot(reflection, vertexToEye)), Material.SpecularPower); // 32 = specular power
	}

	return specular;
}

float DoAttenuation(Light light, float d)
{
	return 1.0f / (light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d);
}

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

LightingResult DoPointLight(Light light, float3 vertexToEye, float4 vertexPos, float3 N)
{
	LightingResult result;

	float3 LightDirectionToVertex = (vertexPos - light.Position).xyz;
	float distance = length(LightDirectionToVertex);
	LightDirectionToVertex = LightDirectionToVertex  / distance;

	float3 vertexToLight = (light.Position - vertexPos).xyz;
	distance = length(vertexToLight);
	vertexToLight = vertexToLight / distance;

	float attenuation = DoAttenuation(light, distance);
	attenuation = 1;


	result.Diffuse = DoDiffuse(light, vertexToLight, N) * attenuation;
	result.Specular = DoSpecular(light, vertexToEye, LightDirectionToVertex, N) * attenuation;

	return result;
}

LightingResult ComputeLighting(float4 vertexPos, float3 N)
{
	float3 vertexToEye = normalize(EyePosition - vertexPos).xyz;

	LightingResult totalResult = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

	[unroll]
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		LightingResult result = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

		if (!Lights[i].Enabled) 
			continue;
		
		result = DoPointLight(Lights[i], vertexToEye, vertexPos, N);
		
		totalResult.Diffuse += result.Diffuse;
		totalResult.Specular += result.Specular;
	}

	totalResult.Diffuse = saturate(totalResult.Diffuse);
	totalResult.Specular = saturate(totalResult.Specular);

	return totalResult;
}


float3x3 computeTBNMatrix(float3 unitNormal, float3 tangent)
{
	float3 N = unitNormal;
	//make sure tangent is orthogonal after interpolation
	float3 T = normalize(tangent - dot(tangent, N) * N);
	float3 B = cross(T, N);

	//compute the tangent space matrix
	float3x3 TBN = float3x3(T, B, N);

	return TBN;
}

float3x3 computeTBNMatrixB(float3 unitNormal, float3 tangent, float3 binorm)
{
	//clone of function above, but it accepts the binormals as well
	//instead of calculating them on the fly, because they're already computed
	//in the DrawableGameObject.cpp
	//this function is used for the code taken from tutorial (cube binormals)
	float3 N = unitNormal;
	float3 T = normalize(tangent - dot(tangent, N) * N);
	//this time you have to make sure the binormal is orthogonal as well 
	float3 B = normalize(binorm - dot(binorm, tangent) * tangent);

	float3x3 TBN = float3x3(T, B, N);

	return TBN;
}

float3 NormalMapping(float2 texCoord, float3x3 TBN)
{
	float3 texNormal = txDiffuse[1].Sample(samLinear, texCoord).rgb;

	//decompress from [0,1] to [-1,1] range
	float3 texNorm = 2.0f * texNormal - 1.0f;

	//transform from tangent space to world space
	float3 bumpedNorm = mul(texNorm, TBN);

	return bumpedNorm;
}

float2 SimpleParallax(float2 texCoord, float3 toEye)
{
	float height = txDiffuse[2].Sample(samLinear, texCoord).r;
	//assumed that scaled height = -biased height -> h * s + b = h * s - s = s(h - 1)
	//because in presentation it states that reasonable scale value = 0.02, and bias = [-0.01, -0.02]
	float heightSB = fHeightScale * (height - 1.0);

	float2 parallax = toEye.xy * heightSB;

	return (texCoord + parallax);
}

float2 ParallaxOcclusion(float2 texCoord, float3 normal, float3 toEye)
{
	//set up toEye vector in tangent space
	float3 toEyeTS = -toEye;

	//calculate the maximum of parallax shift
	float2 parallaxLimit = fHeightScale * toEyeTS.xy;

	//calculate number of samples
	//normal = (0, 0, 1) in tangent space essentially, if dot product converges to 0, take more samples because it means view vec and normal are perpendicular
	int numSamples = (int)lerp(nMaxSamples, nMinSamples, abs(dot(toEyeTS, normal)));
	float zStep = 1.0f / (float)numSamples;

	float2 heightStep = zStep * parallaxLimit;

	float2 dx = ddx(texCoord);
	float2 dy = ddy(texCoord);

	//init loop variables
	int currSample = 0;
	float2 currParallax = float2(0, 0);
	float2 prevParallax = float2(0, 0);
	float2 finalParallax = float2(0, 0);
	float currZ = 1.0f - heightStep;
	float prevZ = 1.0f;
	float currHeight = 0.0f;
	float prevHeight = 0.0f;

	while (currSample < numSamples + 1)
	{
		currHeight = txDiffuse[2].SampleGrad(samLinear, texCoord + currParallax, dx, dy).r;

		if (currHeight > currZ)
		{
			float n = prevHeight - prevZ;
			float d = prevHeight - currHeight - prevZ + currZ;
			float ratio = n / d;

			finalParallax = prevParallax + ratio * heightStep;

			currSample = numSamples + 1;
		}
		else
		{
			++currSample;

			prevParallax = currParallax;
			prevZ = currZ;
			prevHeight = currHeight;

			currParallax += heightStep;

			currZ -= zStep;
		}
	}

	return (texCoord + finalParallax);
}

float ParallaxSelfShadowing(float3 toLight, float2 texCoord, bool softShadow)
{
	float shadowFactor = 1;
	int minLayers = 15;
	int maxLayers = 30;

	float2 dx = ddx(texCoord);
	float2 dy = ddy(texCoord);
	float height = 1.0 - txDiffuse[2].SampleGrad(samLinear, texCoord, dx, dy).r;

	float parallaxScale = fHeightScale * (1.0 - height);

	if (dot(float3(0, 0, 1), toLight) > 0)
	{
		shadowFactor = 0;
		float numSamplesUnderSurface = 0;

		//no need for absolute value because dot is already > 0
		float numLayers = lerp(maxLayers, minLayers, dot(float3(0, 0, 1), toLight));

		float layerHeight = height / numLayers;
		float2 texStep = parallaxScale * toLight.xy / numLayers;

		float currLayerHeight = height - layerHeight;
		float2 currTexCoord = texCoord + texStep;
		float heightFromTex = 1.0 - txDiffuse[2].SampleGrad(samLinear, currTexCoord, dx, dy).r;
		int stepIndex = 1;
		int numIter = 0;

		while (currLayerHeight > 0)
		{
			if (heightFromTex < currLayerHeight)
			{
				numSamplesUnderSurface += 1;
				float newShadowFactor = (currLayerHeight - heightFromTex) * (1.0 - stepIndex / numLayers);
				shadowFactor = max(shadowFactor, newShadowFactor);
			}

			stepIndex += 1;
			currLayerHeight -= layerHeight;
			currTexCoord += texStep;
			heightFromTex = txDiffuse[2].SampleGrad(samLinear, currTexCoord, dx, dy).r;
		}

		if (numSamplesUnderSurface < 1)
		{
			shadowFactor = 1;
		}
		else
		{
			if (softShadow)
			{
				shadowFactor = 0.9 - shadowFactor;
			}
			else
			{
				shadowFactor = 0.1f;
			}
		}
	}

	return shadowFactor;
}


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
	output.worldPos = output.Pos;
    output.Pos = mul( output.Pos, View );
	output.viewPos = output.Pos;
    output.Pos = mul( output.Pos, Projection );

	// multiply the normal by the world transform (to go from model space to world space)
	output.Norm = mul(float4(input.Norm, 1), World).xyz;
	output.Tangent = mul(float4(input.Tangent, 1), World).xyz;
	output.Binormal = mul(float4(input.Binormal, 1), World).xyz;

	output.Tex = input.Tex;
    
    return output;
}


float4 NormalMapped(PS_INPUT IN)
{
	float3 vertexToLight = normalize(LightPosition - IN.worldPos).xyz;

	IN.Norm = normalize(IN.Norm);

	//!!! be careful about which TBN matrix function you're using. in most cases the regular one is fine,
	//but for the cube in DrawableGameObject.cpp, use computeTBNMatrixB
	//float3x3 TBN = computeTBNMatrix(IN.Norm, IN.Tangent);
	float3x3 TBN = computeTBNMatrixB(IN.Norm, IN.Tangent, IN.Binormal);



	float3 bumpNormal = NormalMapping(IN.Tex, TBN);

	LightingResult lit = ComputeLighting(IN.worldPos, normalize(bumpNormal), vertexToLight);

	float4 texColor = { 1, 1, 1, 1 };

	float4 emissive = Material.Emissive;
	float4 ambient = Material.Ambient * GlobalAmbient;
	float4 diffuse = Material.Diffuse * lit.Diffuse;
	float4 specular = Material.Specular * lit.Specular;

	if (Material.UseTexture)
	{
		texColor = txDiffuse[0].Sample(samLinear, IN.Tex);
	}

	float4 finalColor = (emissive + ambient + diffuse + specular) * texColor;
	return finalColor;
}

float4 SimpleParallaxMapped(PS_INPUT IN)
{
	float3 vertexToLight = normalize(LightPosition - IN.worldPos).xyz;
	float3 vertexToEye = normalize(CameraPosition - IN.worldPos).xyz;

	IN.Norm = normalize(IN.Norm);

	//!!! be careful about which TBN matrix function you're using. in most cases the regular one is fine,
	//but for the cube in DrawableGameObject.cpp, use computeTBNMatrixB
	//float3x3 TBN = computeTBNMatrix(IN.Norm, IN.Tangent);
	float3x3 TBN = computeTBNMatrixB(IN.Norm, IN.Tangent, IN.Binormal);

	float3 vertexToEyeTS = mul(vertexToEye, TBN);


	float2 parallaxTex = SimpleParallax(IN.Tex, vertexToEyeTS);

	//discard texels/pixels outside of the normal [0, 1] range - this gives you nice displaced edges (can look wonky tho on a static cube)
	if (parallaxTex.x > 1.0 || parallaxTex.y > 1.0 || parallaxTex.x < 0.0 || parallaxTex.y < 0.0)
		discard;

	float3 bumpNormal = NormalMapping(parallaxTex, TBN);

	//LIGHTING CALC -----------------------------------------------------------------------------------
	LightingResult lit = ComputeLighting(IN.worldPos, normalize(bumpNormal), vertexToLight);

	float4 texColor = { 1, 1, 1, 1 };

	float4 emissive = Material.Emissive;
	float4 ambient = Material.Ambient * GlobalAmbient;
	float4 diffuse = Material.Diffuse * lit.Diffuse;
	float4 specular = Material.Specular * lit.Specular;

	if (Material.UseTexture)
	{
		texColor = txDiffuse[0].Sample(samLinear, parallaxTex);
	}

	float4 finalColor = (emissive + ambient + diffuse + specular) * texColor;
	return finalColor;
}

float4 ParallaxOcclusionMapped(PS_INPUT IN)
{
	float3 vertexToLight = normalize(LightPosition - IN.worldPos).xyz;
	float3 vertexToEye = normalize(CameraPosition - IN.worldPos).xyz;

	IN.Norm = normalize(IN.Norm);

	//!!! be careful about which TBN matrix function you're using. in most cases the regular one is fine,
	//but for the cube in DrawableGameObject.cpp, use computeTBNMatrixB
	//float3x3 TBN = computeTBNMatrix(IN.Norm, IN.Tangent);
	float3x3 TBN = computeTBNMatrixB(IN.Norm, IN.Tangent, IN.Binormal);

	float3 vertexToEyeTS = mul(vertexToEye, TBN);


	float2 parallaxTex = ParallaxOcclusion(IN.Tex, IN.Norm, vertexToEyeTS);

	if (parallaxTex.x > 1.0 || parallaxTex.y > 1.0 || parallaxTex.x < 0.0 || parallaxTex.y < 0.0)
		discard;

	float3 bumpNormal = NormalMapping(parallaxTex, TBN);

	LightingResult lit = ComputeLighting(IN.worldPos, normalize(bumpNormal), vertexToLight);

	float4 texColor = { 1, 1, 1, 1 };

	float4 emissive = Material.Emissive;
	float4 ambient = Material.Ambient * GlobalAmbient;
	float4 diffuse = Material.Diffuse * lit.Diffuse;
	float4 specular = Material.Specular * lit.Specular;

	if (Material.UseTexture)
	{
		texColor = txDiffuse[0].Sample(samLinear, parallaxTex);
	}


	float4 finalColor = (emissive + ambient + diffuse + specular) * texColor;

	return finalColor;
}

float4 SelfShadowed(PS_INPUT IN)
{
	float3 vertexToLight = normalize(LightPosition - IN.worldPos).xyz;
	float3 vertexToEye = normalize(CameraPosition - IN.worldPos).xyz;

	IN.Norm = normalize(IN.Norm);

	//!!! be careful about which TBN matrix function you're using. in most cases the regular one is fine,
	//but for the cube in DrawableGameObject.cpp, use computeTBNMatrixB
	//float3x3 TBN = computeTBNMatrix(IN.Norm, IN.Tangent);
	float3x3 TBN = computeTBNMatrixB(IN.Norm, IN.Tangent, IN.Binormal);

	float3 vertexToLightTS = mul(vertexToLight, TBN);
	float3 vertexToEyeTS = mul(vertexToEye, TBN);


	bool softShadow = false;
	if (iEffectID == 4)
	{
		softShadow = true;
	}


	float2 parallaxTex = ParallaxOcclusion(IN.Tex, IN.Norm, vertexToEyeTS);

	if (parallaxTex.x > 1.0 || parallaxTex.y > 1.0 || parallaxTex.x < 0.0 || parallaxTex.y < 0.0)
		discard;

	float3 bumpNormal = NormalMapping(parallaxTex, TBN);

	LightingResult lit = ComputeLighting(IN.worldPos, normalize(bumpNormal), vertexToLight);

	float4 texColor = { 1, 1, 1, 1 };

	float4 emissive = Material.Emissive;
	float4 ambient = Material.Ambient * GlobalAmbient;
	float4 diffuse = Material.Diffuse * lit.Diffuse;
	float4 specular = Material.Specular * lit.Specular;

	if (Material.UseTexture)
	{
		texColor = txDiffuse[0].Sample(samLinear, parallaxTex);
	}

	float shadowFactor = ParallaxSelfShadowing(vertexToLightTS, parallaxTex, softShadow);
	float4 finalColor = (emissive + ambient + diffuse * shadowFactor + specular * shadowFactor) * texColor;


	return finalColor;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS(PS_INPUT IN) : SV_TARGET
{
	if (iEffectID == 0 || iEffectID == 10)
	{
		return NormalMapped(IN);
	}
	if (iEffectID == 1)
	{
		return ParallaxMapped(IN);
	}
	if (iEffectID == 2)
	{
		return ParallaxOcclusionMapped(IN);
	}
	if (iEffectID > 2)
	{
		return SelfShadowed(IN);
	}


	return { 1, 1, 1, 1 };
}

//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid(PS_INPUT input) : SV_Target
{
	return vOutputColor;
}
