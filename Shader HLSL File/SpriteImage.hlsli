
//	fxc /E PSTexturedLightingColor /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo DeferredShading.hlsli
#include "GBufferStructure.hlsli"

cbuffer cbViewProjectionMatrix : register(b0) // VS Set
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

cbuffer cbWorldMatrix : register(b1) // VS Set
{
    matrix gmtxWorld : packoffset(c0);
};

cbuffer cbTestVariable : register(b3) // VS Set
{
    float gfFrame : packoffset(c0.x);
    float gfSizeX : packoffset(c0.y);
    float gfSizeY : packoffset(c0.z);
    float pad : packoffset(c0.w);
};

Texture2D gtxDiffuse : register(t0);
SamplerState gssDefault : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float2 texCoord : TEXCOORD;
};


//-------------------------------------------------------------------------------------------------------------------------------
VS_OUTPUT VSSpriteImage(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.texCoord = input.texCoord;

    int iX = (int) gfFrame % gfSizeX;
    int iY = (int) gfFrame / gfSizeX;

    float fX = output.texCoord.x / gfSizeX;
    float fY = output.texCoord.y / gfSizeY;

    output.texCoord.x = fX + (1.f / gfSizeX * iX);
    output.texCoord.y = fY + (1.f / gfSizeY * iY);
    
    return (output);
}

PS_GBUFFER_OUTPUT PSSpriteImage(VS_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output = (PS_GBUFFER_OUTPUT) 0;

    float3 DiffuseColor = gtxDiffuse.Sample(gssDefault, input.texCoord);

    output.DiffuseSpecInt = float4(DiffuseColor.rgb, g_SpecIntensity);
    output.Normal = 0;
    output.SpecPow = 0;

    return output;
}