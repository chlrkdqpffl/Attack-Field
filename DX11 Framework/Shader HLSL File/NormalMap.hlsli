#include "Light.hlsli"
#include "Normal.hlsli"
#include "Fog.hlsli"

//    fxc /E PS_TexturedLightingNormalMap /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo NormalMap.hlsli

cbuffer cbViewProjectionMatrix : register(b0)   // VS Set
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

cbuffer cbWorldMatrix : register(b1)            // VS Set
{
    matrix gmtxWorld : packoffset(c0);
};

cbuffer cbRenderOption : register(b5) // PS Set
{
    float4 gbRenderOption : packoffset(c0); // (x : Fog)
};

Texture2D gtxtDiffuse : register(t0);
SamplerState gssDefault : register(s0);

struct VS_TEXTURED_LIGHTING_NORMAL_INPUT
{
    float3 positionL : POSITION;
    float3 normalL : NORMAL;
    float3 tangentL : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_TEXTURED_LIGHTING_NORMAL_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float2 texCoord : TEXCOORD;
};

VS_TEXTURED_LIGHTING_NORMAL_OUTPUT VS_TexturedLightingNormalMap(VS_TEXTURED_LIGHTING_NORMAL_INPUT input)
{
    VS_TEXTURED_LIGHTING_NORMAL_OUTPUT output = (VS_TEXTURED_LIGHTING_NORMAL_OUTPUT) 0;
	
    output.positionW = mul(float4(input.positionL, 1.0f), gmtxWorld).xyz;
    output.normalW = mul(input.normalL, (float3x3) gmtxWorld);
    output.tangentW = mul(input.tangentL, (float3x3) gmtxWorld);

    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	
    output.texCoord = input.texCoord;

    return output;
}

float4 PS_TexturedLightingNormalMap(VS_TEXTURED_LIGHTING_NORMAL_OUTPUT input) : SV_Target
{
    float3 normalW = CalcNormal(input.normalW, input.tangentW, input.texCoord);
    float4 cIllumination = Lighting(input.positionW, normalW);
    float4 cColor = gtxtDiffuse.Sample(gssDefault, input.texCoord) * cIllumination;
  
    if (gbRenderOption.x == 1.0f)
        cColor = Fog(cColor, input.positionW);

    return cColor;
}