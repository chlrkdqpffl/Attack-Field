
//	fxc /E PSDeferrdNormalTexture /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo DeferredShading.hlsli


cbuffer cbViewProjectionMatrix : register(b0) // VS Set
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

cbuffer cbWorldMatrix : register(b1) // VS Set
{
    matrix gmtxWorld : packoffset(c0);
};

Texture2D gtxDiffuse : register(t0);
SamplerState gssDefault : register(s0);


static const float2 g_SpecPowerRange = { 10.0, 250.0 };
static const float g_SpecPower = 250.f;
static const float g_SpecIntensity = 0.25f;


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

struct PS_GBUFFER_OUTPUT
{
    float4 DiffuseSpecInt : SV_TARGET0;
    float4 Normal         : SV_TARGET1;
    float4 SpecPow        : SV_TARGET2;
};


VS_TEXTURED_LIGHTING_OUTPUT VSDeferrdNormalTexture(VS_TEXTURED_LIGHTING_INPUT input)
{
    VS_TEXTURED_LIGHTING_OUTPUT output = (VS_TEXTURED_LIGHTING_OUTPUT) 0;
    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
    output.normalW = mul(input.normal, (float3x3) gmtxWorld); 
    output.texCoord = input.texCoord;

    return (output);
}

PS_GBUFFER_OUTPUT PSDeferrdNormalTexture(VS_TEXTURED_LIGHTING_OUTPUT input)
{
    // Normalize
    float SpecPowerNorm = max(0.0001, (g_SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y);
    input.normalW = normalize(input.normalW);

    float3 DiffuseColor = gtxDiffuse.Sample(gssDefault, input.texCoord);
    DiffuseColor *= DiffuseColor;       // 어떤 차이인지 확인해보기
   
    PS_GBUFFER_OUTPUT output = (PS_GBUFFER_OUTPUT) 0;

    output.DiffuseSpecInt = float4(DiffuseColor.rgb, g_SpecIntensity);
    output.Normal = float4(input.normalW * 0.5 + 0.5, 0.0);
    output.SpecPow = float4(SpecPowerNorm, 0.0, 0.0, 0.0);

    return output;
}