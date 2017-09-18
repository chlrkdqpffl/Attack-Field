
//	fxc /E PSTexturedLightingColor /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo DeferredShading.hlsli
#include "Normal.hlsli"
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

cbuffer cbTextureOffset : register(b2) // VS Set
{
    float2 g_TextureOffset: packoffset(c0);
    float2 pad : packoffset(c2);
};

Texture2D gtxDiffuse                : register(t0);
SamplerState gssAnisotropicWrap     : register(s6);

struct VS_DIFFUSED_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR0;
};

struct VS_DIFFUSED_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

struct VS_LIGHTING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
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
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct VS_INSTANCED_TEXTURED_LIGHTING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
    float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD0;
    float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float2 texCoord : TEXCOORD0;
};


//-------------------------------------------------------------------------------------------------------------------------------
VS_DIFFUSED_OUTPUT VSDiffusedColor(VS_DIFFUSED_INPUT input)
{
    VS_DIFFUSED_OUTPUT output = (VS_DIFFUSED_OUTPUT) 0;
	//output.position = mul(float4(input.position, 1.0f), mul(mul(gmtxWorld, gmtxView), gmtxProjection));
    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
    output.color = input.color;

    return (output);
}

PS_GBUFFER_OUTPUT PSDiffusedColor(VS_DIFFUSED_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output = (PS_GBUFFER_OUTPUT) 0;

    output.DiffuseSpecInt = float4(input.color.rgb, g_SpecIntensity);
    output.Normal = 0;
    output.SpecPow = 0;

    return output;
}

//-------------------------------------------------------------------------------------------------------------------------------
VS_LIGHTING_OUTPUT VSLightingColor(VS_LIGHTING_INPUT input)
{
    VS_LIGHTING_OUTPUT output = (VS_LIGHTING_OUTPUT) 0;
    output.normalW = mul(input.normal, (float3x3) gmtxWorld);
    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

    return (output);
}

PS_GBUFFER_OUTPUT PSLightingColor(VS_LIGHTING_OUTPUT input)
{
    // Normalize
    float SpecPowerNorm = max(0.0001, (g_SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y);
    input.normalW = normalize(input.normalW);

    PS_GBUFFER_OUTPUT output = (PS_GBUFFER_OUTPUT) 0;

    output.DiffuseSpecInt = float4(0.8f, 0.8f, 0.8f, g_SpecIntensity);
    output.Normal = float4(input.normalW * 0.5 + 0.5, 0.0);
    output.SpecPow = float4(SpecPowerNorm, 0.0, 0.0, 0.0);

    return output;
}

//-------------------------------------------------------------------------------------------------------------------------------
VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLightingColor(VS_TEXTURED_LIGHTING_INPUT input)
{
    VS_TEXTURED_LIGHTING_OUTPUT output = (VS_TEXTURED_LIGHTING_OUTPUT) 0;
    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
    output.normalW = mul(input.normal, (float3x3) gmtxWorld); 
    output.texCoord = input.texCoord * g_TextureOffset;

    return (output);
}

PS_GBUFFER_OUTPUT PSTexturedLightingColor(VS_TEXTURED_LIGHTING_OUTPUT input)
{
    // Normalize
    float SpecPowerNorm = max(0.0001, (g_SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y);
    input.normalW = normalize(input.normalW);

    float3 DiffuseColor = gtxDiffuse.Sample(gssAnisotropicWrap, input.texCoord);
    DiffuseColor *= DiffuseColor;       // 어떤 차이인지 확인해보기
   
    PS_GBUFFER_OUTPUT output = (PS_GBUFFER_OUTPUT) 0;

    output.DiffuseSpecInt = float4(DiffuseColor.rgb, g_SpecIntensity);
    output.Normal = float4(input.normalW * 0.5 + 0.5, 0.0);
    output.SpecPow = float4(SpecPowerNorm, 0.0, 0.0, 0.0);

    return output;
}

//-------------------------------------------------------------------------------------------------------------------------------
VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT VSInstancedTexturedLightingColor(VS_INSTANCED_TEXTURED_LIGHTING_INPUT input)
{
    VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT output = (VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT) 0;
    output.normalW = mul(input.normal, (float3x3) input.mtxTransform);
    output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.texCoord = input.texCoord * g_TextureOffset;
    return (output);
}

PS_GBUFFER_OUTPUT PSInstancedTexturedLightingColor(VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT input)
{
     // Normalize
    float SpecPowerNorm = max(0.0001, (g_SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y);
    input.normalW = normalize(input.normalW);

    float3 DiffuseColor = gtxDiffuse.Sample(gssAnisotropicWrap, input.texCoord);
    DiffuseColor *= DiffuseColor; // 어떤 차이인지 확인해보기
   
    PS_GBUFFER_OUTPUT output = (PS_GBUFFER_OUTPUT) 0;

    output.DiffuseSpecInt = float4(DiffuseColor.rgb, g_SpecIntensity);
    output.Normal = float4(input.normalW * 0.5 + 0.5, 0.0);
    output.SpecPow = float4(SpecPowerNorm, 0.0, 0.0, 0.0);

    return output;
}

VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_OUTPUT VSInstancedTexturedTangentLighting(VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_INPUT input)
{
    VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_OUTPUT output = (VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_OUTPUT) 0;
    output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.normalW = mul(input.normal, (float3x3) input.mtxTransform);
    output.tangentW = mul(input.tangent, (float3x3) input.mtxTransform);
    output.texCoord = input.texCoord * g_TextureOffset;
    return (output);
}

PS_GBUFFER_OUTPUT PSInstancedTexturedTangentLighting(VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_OUTPUT input)
{
    float3 normalW = CalcNormal(input.normalW, input.tangentW, input.texCoord);
  
    // Normalize
    float SpecPowerNorm = max(0.0001, (g_SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y);
    input.normalW = normalize(input.normalW);

    float3 DiffuseColor = gtxDiffuse.Sample(gssAnisotropicWrap, input.texCoord);
    DiffuseColor *= DiffuseColor; // 어떤 차이인지 확인해보기
   
    PS_GBUFFER_OUTPUT output = (PS_GBUFFER_OUTPUT) 0;

    output.DiffuseSpecInt = float4(DiffuseColor.rgb, g_SpecIntensity);
    output.Normal = float4(normalW * 0.5 + 0.5, 0.0);
    output.SpecPow = float4(SpecPowerNorm, 0.0, 0.0, 0.0);

    return output;
}