#include "common.hlsli"

//	fxc /E PSGBuffer /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo GBuffer.hlsli

static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
	float2(1.0, 1.0),
	float2(-1.0, -1.0),
	float2(1.0, -1.0),
};

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// --------------------------------------------------------------------------------------------------------- //

VS_OUTPUT VSGBuffer(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return (output);
}

float4 PSGBuffer_LinearDepth(VS_OUTPUT input) : SV_Target
{
    SURFACE_DATA gbd = UnpackGBuffer(input.texCoord.xy);

    return float4(1.0 - saturate(gbd.LinearDepth / 75.0), 1.0 - saturate(gbd.LinearDepth / 125.0), 1.0 - saturate(gbd.LinearDepth / 200.0), 0.0);
}

float4 PSGBuffer_Diffuse(VS_OUTPUT input) : SV_Target
{
    SURFACE_DATA gbd = UnpackGBuffer(input.texCoord.xy);

    return float4(gbd.Color.xyz, 0.0);
}

float4 PSGBuffer_Normal(VS_OUTPUT input) : SV_Target
{
    SURFACE_DATA gbd = UnpackGBuffer(input.texCoord.xy);

    return float4(gbd.Normal.xyz * 0.5 + 0.5, 0.0);
 //   return float4(gbd.Normal.xyz, 0.0);
}

float4 PSGBuffer_Spec(VS_OUTPUT input) : SV_Target
{
    SURFACE_DATA gbd = UnpackGBuffer(input.texCoord.xy);

    return float4(gbd.SpecIntensity, gbd.SpecPow, 0.0, 0.0);
}