
#include "Light.hlsli"

//	fxc /E PSTextureToScreen /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo TextureToScreen.hlsli
cbuffer cbViewProjectionMatrix : register(b0) // VS Set
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

cbuffer cbWorldMatrix : register(b1) // VS Set
{
    matrix gmtxWorld : packoffset(c0);
};

cbuffer cbRenderOption : register(b5) // PS Set
{
    float4 gbRenderOption : packoffset(c0); // (x : Fog Render, y : BoundingBox Render )
};

cbuffer cbSkinned : register(b7) // VS Set
{
    matrix gBoneTransform[80];
};

Texture2D gtxDiffuse : register(t0);
SamplerState gssDefault : register(s0);


struct GBUFFER
{
    float4 DiffuseSpecInt   : SV_TARGET0;
    float4 Normal           : SV_TARGET1;
    float4 SpecPow          : SV_TARGET2;
};



struct VS_TEXTURED_LIGHTING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct VS_TEXTURED_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
};

VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLightingColor(VS_TEXTURED_LIGHTING_INPUT input)
{
    VS_TEXTURED_LIGHTING_OUTPUT output = (VS_TEXTURED_LIGHTING_OUTPUT) 0;
    output.normalW = mul(input.normal, (float3x3) gmtxWorld); 
    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.texCoord = input.texCoord;

    return (output);
}

float4 PSTexturedLightingColor(VS_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
    input.normalW = normalize(input.normalW);
    float4 cIllumination = Lighting(input.positionW, input.normalW);
    float4 cColor = gtxDiffuse.Sample(gssDefault, input.texCoord) * cIllumination;
//    return float4(input.normalW, 1.0);
    return (cColor);
}
