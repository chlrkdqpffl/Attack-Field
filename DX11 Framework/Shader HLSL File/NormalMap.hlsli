#include "Light.hlsli"
#include "Fog.hlsli"

//    fxc /E VS_TexturedLightingNormalMap /T vs_5_0 /Od /Zi /Fo CompiledShader.fxo NormalMap.hlsli

cbuffer cbViewProjectionMatrix : register(b0)   // VS Set
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

cbuffer cbWorldMatrix : register(b1)            // VS Set
{
    matrix gmtxWorld : packoffset(c0);
};

Texture2D gtxtDiffuseMap : register(t18);
Texture2D gtxtNormalMap : register(t19);
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
    float3 N = normalize(input.normalW);
    float3 T = normalize(input.tangentW - dot(input.tangentW, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    float3 normalTextureRGB = gtxtNormalMap.Sample(gssDefault, input.texCoord).rgb;
    float3 normal = 2.0f * normalTextureRGB - 1.0f;
    float3 normalW = mul(normal, TBN);

    float4 cIllumination = Lighting(input.positionW, normalW);
    return cIllumination;
    float4 cColor = gtxtDiffuseMap.Sample(gssDefault, input.texCoord) * cIllumination;

    return cColor;
}