
#include "Normal.hlsli"
#include "GBufferStructure.hlsli"
//     fxc /E PSSkinnedTexturedBumpedLightingColor /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo DeferredCharacterSkinnedNormal.hlsli

cbuffer cbViewProjectionMatrix : register(b0)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
}

cbuffer cbWorldMatrix : register(b1)
{
    matrix gmtxWorld : packoffset(c0);
}

cbuffer cbSkinned : register(b7)
{
    matrix gBoneTransform[60];
};

Texture2D gtxtDiffuse : register(t0);
SamplerState gSamplerState : register(s0);

struct VS_SKINNED_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD0;
    float4 boneWeights : WEIGHTS;
    float4 boneIndices : BONEINDICES;
};

struct VS_SKINNED_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float2 texCoord : TEXCOORD0;
};

VS_SKINNED_OUTPUT VSSkinnedTexturedBumpedLightingColor(VS_SKINNED_INPUT input)
{
    VS_SKINNED_OUTPUT output = (VS_SKINNED_OUTPUT) 0;

    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = input.boneWeights.x;
    weights[1] = input.boneWeights.y;
    weights[2] = input.boneWeights.z;
    weights[3] = input.boneWeights.w;

   // weights[3] = 1 - weights[0] - weights[1] - weights[2];

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    float3 tangentL = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++)
    {
        posL += weights[i] * mul(float4(input.position, 1.0f), gBoneTransform[input.boneIndices[i]]).xyz;
        normalL += weights[i] * mul(input.normal, (float3x3) gBoneTransform[input.boneIndices[i]]);
        tangentL += weights[i] * mul(input.tangent, (float3x3) gBoneTransform[input.boneIndices[i]]);
    }

    output.positionW = mul(float4(posL, 1.0f), gmtxWorld).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.normalW = mul(normalL, (float3x3) gmtxWorld);
    output.tangentW = mul(tangentL, (float3x3) gmtxWorld);
    output.texCoord = input.texCoord;

    return output;
}

PS_GBUFFER_OUTPUT PSSkinnedTexturedBumpedLightingColor(VS_SKINNED_OUTPUT input)
{
    float3 normalW = CalcNormal(input.normalW, input.tangentW, input.texCoord);
  
    // Normalize
    float SpecPowerNorm = max(0.0001, (g_SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y);
    input.normalW = normalize(input.normalW);

    float3 DiffuseColor = gtxtDiffuse.Sample(gSamplerState, input.texCoord);
    DiffuseColor *= DiffuseColor; // 어떤 차이인지 확인해보기
   
    PS_GBUFFER_OUTPUT output = (PS_GBUFFER_OUTPUT) 0;

    output.DiffuseSpecInt = float4(DiffuseColor, g_SpecIntensity);
    output.Normal = float4(normalW * 0.5 + 0.5, 0.0);
   // output.Normal = float4(1,1,1, 0.0);
    output.SpecPow = float4(SpecPowerNorm, 0.0, 0.0, 0.0);

    return output;
}