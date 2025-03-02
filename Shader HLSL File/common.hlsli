/////////////////////////////////////////////////////////////////////////////
// GBuffer textures and Samplers
/////////////////////////////////////////////////////////////////////////////
Texture2D<float>    DepthTexture            : register(t19);
Texture2D<float4>   DiffuseSpecIntTexture   : register(t20);
Texture2D<float3>   NormalTexture           : register(t21);
Texture2D<float4>   SpecPowTexture          : register(t22);

SamplerState        PointSampler            : register(s0);

/////////////////////////////////////////////////////////////////////////////
// constants
/////////////////////////////////////////////////////////////////////////////
cbuffer cbGBufferUnpack : register(b6)
{
    float4 PerspectiveValues : packoffset(c0);
    float4x4 ViewInv         : packoffset(c1);
}

cbuffer cbFog : register(b2)
{
    float3 FogColor : packoffset(c0);
    float FogStartDepth : packoffset(c0.w);
    float3 FogHighlightColor : packoffset(c1);
    float FogGlobalDensity : packoffset(c1.w);
    float3 FogSunDir : packoffset(c2);
    float FogStartHeight : packoffset(c2.w);
}

#define EyePosition (ViewInv[3].xyz)

static const float2 g_SpecPowerRange = { 10.0, 250.0 };

cbuffer cbTestVariable : register(b11) // VS, GS, PS Buffer
{
    float4 g_f4Var : packoffset(c0);
};


struct SURFACE_DATA
{
    float LinearDepth;
    float3 Color;
    float3 Normal;
    float SpecPow;
    float SpecIntensity;
};

float3 DecodeNormal(float2 encodedNormal)
{
    float4 decodedNormal = encodedNormal.xyyy * float4(2, 2, 0, 0) + float4(-1, -1, 1, -1);
    decodedNormal.z = dot(decodedNormal.xyz, -decodedNormal.xyw);
    decodedNormal.xy *= sqrt(decodedNormal.z);
    return decodedNormal.xyz * 2.0 + float3(0.0, 0.0, -1.0);
}

float ConvertZToLinearDepth(float depth)
{
    float linearDepth = PerspectiveValues.z / (depth + PerspectiveValues.w);
    return linearDepth;
}

float3 CalcWorldPos(float2 csPos, float depth)
{
    float4 position;

    position.xy = csPos.xy * PerspectiveValues.xy * depth;
    position.z = depth;
    position.w = 1.0;
	
    return mul(position, ViewInv).xyz;
}

SURFACE_DATA UnpackGBuffer(float2 UV)
{
    SURFACE_DATA Out;

    float depth = DepthTexture.Sample(PointSampler, UV.xy).x;
    Out.LinearDepth = ConvertZToLinearDepth(depth);
    float4 baseColorSpecInt = DiffuseSpecIntTexture.Sample(PointSampler, UV.xy);
    Out.Color = baseColorSpecInt.xyz;
    Out.SpecIntensity = baseColorSpecInt.w;
    Out.Normal = NormalTexture.Sample(PointSampler, UV.xy).xyz;
    Out.Normal = normalize(Out.Normal * 2.0 - 1.0);
    Out.SpecPow = SpecPowTexture.Sample(PointSampler, UV.xy).x;

    return Out;
}

SURFACE_DATA UnpackGBuffer_Loc(int2 location)
{
    SURFACE_DATA Out;
    int3 location3 = int3(location, 0);

    float depth = DepthTexture.Load(location3).x;
    Out.LinearDepth = ConvertZToLinearDepth(depth);
    float4 baseColorSpecInt = DiffuseSpecIntTexture.Load(location3);
    Out.Color = baseColorSpecInt.xyz;
    Out.SpecIntensity = baseColorSpecInt.w;
    Out.Normal = NormalTexture.Load(location3).xyz;
    Out.Normal = normalize(Out.Normal * 2.0 - 1.0);
    Out.SpecPow = SpecPowTexture.Load(location3).x;

    return Out;
}

struct Material
{
    float3 normal;
    float4 diffuseColor;
    float specPow;
    float specIntensity;
};

void MaterialFromGBuffer(SURFACE_DATA gbd, inout Material mat)
{
    mat.normal = gbd.Normal;
    mat.diffuseColor.xyz = gbd.Color;
    mat.diffuseColor.w = 1.0; // Fully opaque
    mat.specPow = g_SpecPowerRange.x + g_SpecPowerRange.y * gbd.SpecPow;
  
    mat.specIntensity = gbd.SpecIntensity;
}

float4 DebugLightPS() : SV_TARGET
{
    return float4(1.0, 1.0, 1.0, 1.0);
}