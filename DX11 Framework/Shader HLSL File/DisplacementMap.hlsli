#include "Light.hlsli"
#include "Fog.hlsli"

//    fxc /E PS_NormalDisplace /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo DisplacementMap.hlsli

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
    float4 gbRenderOption : packoffset(c0); // (x : Fog)
};

Texture2D gtxtDiffuseMap : register(t18);
Texture2D gtxtNormalMap : register(t19);
SamplerState gssDefault : register(s0);

static const float gfMinDistance        = 10;
static const float gfMaxDistance        = 500;
static const float gfMinTessFactor      = 1;
static const float gfMaxTessFactor      = 6;
static const float gfMipLevelInterval   = 20;
static const float gfMaxMipLevel        = 6.0f;
static const float gfHeightScale        = 10.0f;

struct VS_NormalDisplace_Input
{
    float3 positionL : POSITION;
    float3 normalL   : NORMAL;
    float3 tangentL  : TANGENT;
    float2 texCoord  : TEXCOORD;
};

struct VS_NormalDisplace_Output
{
    float3 positionW : POSITION;
    float3 normalW   : NORMAL;  
    float3 tangentW  : TANGENT;
    float2 texCoord  : TEXCOORD;
    float fTessFactor : TESSFACTOR;
};


VS_NormalDisplace_Output VS_NormalDisplace(VS_NormalDisplace_Input input)
{
    VS_NormalDisplace_Output output = (VS_NormalDisplace_Output) 0;

    output.positionW = mul(float4(input.positionL, 1.0f), gmtxWorld).xyz;
    output.normalW = mul(input.normalL, (float3x3) gmtxWorld);
    output.tangentW = mul(input.tangentL, (float3x3) gmtxWorld);
    output.texCoord = input.texCoord;

    float fDistToCamera = distance(output.positionW, gvCameraPosition);
    float fTessFactor = saturate((fDistToCamera - gfMinDistance) / (gfMaxDistance - gfMinDistance));

    output.fTessFactor = gfMinTessFactor + fTessFactor * (gfMaxTessFactor - gfMinTessFactor);
 //   output.fTessFactor = 1;
    return output;
}
/*
VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
	// Transform to world space space.
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3) gWorldInvTranspose);
    vout.TangentW = mul(vin.TangentL, (float3x3) gWorld);

	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	
    float d = distance(vout.PosW, gEyePosW);

	// Normalized tessellation factor. 
	// The tessellation is 
	//   0 if d >= gMinTessDistance and
	//   1 if d <= gMaxTessDistance.  
    float tess = saturate((gMinTessDistance - d) / (gMinTessDistance - gMaxTessDistance));
	
	// Rescale [0,1] --> [gMinTessFactor, gMaxTessFactor].
    vout.TessFactor = gMinTessFactor + tess * (gMaxTessFactor - gMinTessFactor);

    return vout;
}
*/

struct HSC_Output
{
    float fTessEdges[3] : SV_TessFactor;
    float fTessInsides : SV_InsideTessFactor;
};

HSC_Output ConstantHS(InputPatch<VS_NormalDisplace_Output, 3> patch, uint nPatchID : SV_PrimitiveID)
{
    HSC_Output output = (HSC_Output) 0;

    output.fTessEdges[0] = 0.5f * (patch[1].fTessFactor + patch[2].fTessFactor);
    output.fTessEdges[1] = 0.5f * (patch[2].fTessFactor + patch[0].fTessFactor);
    output.fTessEdges[2] = 0.5f * (patch[0].fTessFactor + patch[1].fTessFactor);
    output.fTessInsides = output.fTessEdges[0];

    return output;
}

struct HS_Output
{
    float3 positionW : POSITION;
    float3 normalW   : NORMAL;
    float3 tangentW  : TANGENT;
    float2 texCoord  : TEXCOORD;
};

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HS_Output HS_NormalDisplace(InputPatch<VS_NormalDisplace_Output, 3> input, uint i : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
    HS_Output output = (HS_Output) 0;

    output.positionW = input[i].positionW;
    output.normalW = input[i].normalW;
    output.tangentW = input[i].tangentW;
    output.texCoord = input[i].texCoord;

    return output;
}

struct DS_Output
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float2 texCoord : TEXCOORD;
};

[domain("tri")]
DS_Output DS_NormalDisplace(HSC_Output input, float3 uv : SV_DomainLocation, OutputPatch<HS_Output, 3> tri)
{
    DS_Output output = (DS_Output) 0;

    output.positionW = uv.x * tri[0].positionW + uv.y * tri[1].positionW + uv.z * tri[2].positionW;
    output.normalW = uv.x * tri[0].normalW + uv.y * tri[1].normalW + uv.z * tri[2].normalW;
    output.tangentW = uv.x * tri[0].tangentW + uv.y * tri[1].tangentW + uv.z * tri[2].tangentW;
    output.texCoord = uv.x * tri[0].texCoord + uv.y * tri[1].texCoord + uv.z * tri[2].texCoord;
    output.normalW = normalize(output.normalW);

    float fDistToCamera = distance(output.positionW, gvCameraPosition);
    float fMipLevel = clamp(((fDistToCamera - gfMipLevelInterval) / gfMipLevelInterval), 0.0f, gfMaxMipLevel);
    float fHeight = gtxtNormalMap.SampleLevel(gssDefault, output.texCoord, fMipLevel).a;

    output.positionW += (gfHeightScale * (fHeight - 1.0f)) * output.normalW;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

    return output;
}

float4 PS_NormalDisplace(DS_Output input) : SV_Target
{
    float3 N = normalize(input.normalW);
    float3 T = normalize(input.tangentW - dot(input.tangentW, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    float3 normalTextureRGB = gtxtNormalMap.Sample(gssDefault, input.texCoord).rgb;
    float3 normalW = (2.0f * normalTextureRGB - 1.0f);
    normalW = mul(normalW, TBN);

    float4 cIllumination = Lighting(input.positionW, normalW);
    float4 cColor = gtxtDiffuseMap.Sample(gssDefault, input.texCoord) * cIllumination;

    if (gbRenderOption.x == 1.0f)
        cColor = Fog(cColor, input.positionW);

    return cColor;
}