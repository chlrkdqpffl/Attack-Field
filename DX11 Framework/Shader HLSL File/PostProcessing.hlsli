#include "common.hlsli"
#include "Light.hlsli"
//	fxc /E PSGBuffer /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo GBuffer.hlsli

static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
	float2(1.0, 1.0),
	float2(-1.0, -1.0),
	float2(1.0, -1.0),
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// --------------------------------------------------------------------------------------------------------- //

VS_OUTPUT VSPostProcessing(uint VertexID : SV_VertexID)
{
    VS_OUTPUT Output;

    Output.Position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
    Output.texCoord = Output.Position.xy;

    return Output;
}

float4 PSPostProcessing(VS_OUTPUT input) : SV_Target
{
    SURFACE_DATA GBufferData = UnpackGBuffer_Loc(input.Position.xy);

    float3 position = CalcWorldPos(input.texCoord, GBufferData.LinearDepth);
    float4 cIllumination = Lighting(position, GBufferData.Normal * 2 - 1);

    //return float4((GBufferData.Color), 1) * cIllumination;
    return float4((GBufferData.Color), 1);
}