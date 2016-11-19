
//     fxc /E PS_Terrain /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo Terrain.hlsli

static const float gMaxDist = 500.0f;
static const float gMinDist = 10.0f;

Texture2D gtxTerrainDiffuse : register(t2);
Texture2D gtxTerrainDetail : register(t3);
Texture2D gtxTerrainNormal : register(t4);
Texture2D gtxTerrainHeight : register(t5);

SamplerState gssTerrain : register(s2);
SamplerState gssTerrainDetail : register(s3);

cbuffer cbViewProjectionMatrix : register(b0) // VS Set
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

cbuffer cbCameraPosition : register(b2) // PS Set
{
    float4 gvCameraPosition : packoffset(c0);
};

struct VS_Terrain_Input
{
    float3 posL : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float2 detailTex : TEXCOORD1;
};

struct VS_Terrain_Output
{
    float3 posW : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float2 detailTex : TEXCOORD1;
};

VS_Terrain_Output VS_Terrain(VS_Terrain_Input input)
{
    VS_Terrain_Output output = (VS_Terrain_Output) 0;
	
    output.posW = input.posL;
    output.tex = input.tex;
    output.detailTex = input.detailTex;

    return output;
}
 
float CalcTessFactor(float3 p)
{
    float fDistToCamera = distance(p, gvCameraPosition.xyz);
    float s = saturate((fDistToCamera - gMinDist) / (gMaxDist - gMinDist));
	
    return pow(2, (lerp(64.0f, 1.0f, s)));
}

struct HSC_Terrain_Output
{
    float fTessEdges[4] : SV_TessFactor;
    float fTessInsides[2] : SV_InsideTessFactor;
};

HSC_Terrain_Output ConstantHS_Terrain(InputPatch<VS_Terrain_Output, 4> input, uint patchID : SV_PrimitiveID)
{
    HSC_Terrain_Output output;
	
    float3 edge0 = 0.5f * (input[0].posW + input[2].posW);
    float3 edge1 = 0.5f * (input[0].posW + input[1].posW);
    float3 edge2 = 0.5f * (input[1].posW + input[3].posW);
    float3 edge3 = 0.5f * (input[2].posW + input[3].posW);
    
    float3 center = 0.25f * (input[0].posW + input[1].posW + input[2].posW + input[3].posW);

//  output.fTessEdges[0] = CalcTessFactor(edge0);
//  output.fTessEdges[1] = CalcTessFactor(edge1);
//  output.fTessEdges[2] = CalcTessFactor(edge2);
//  output.fTessEdges[3] = CalcTessFactor(edge3);
//  output.fTessInsides[0] = CalcTessFactor(center);
//  output.fTessInsides[1] = output.fTessInsides[0];
    output.fTessEdges[0] = 4;
    output.fTessEdges[1] = 4;
    output.fTessEdges[2] = 4;
    output.fTessEdges[3] = 4;
    output.fTessInsides[0] = 4;
    output.fTessInsides[1] = 4;

    return output;
}

struct HS_Terrain_Output
{
    float3 posW : POSITION;
    float2 tex : TEXCOORD0;
    float2 detailTex : TEXCOORD1;
};

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS_Terrain")]
[maxtessfactor(64.0f)]
HS_Terrain_Output HS_Terrain(InputPatch<VS_Terrain_Output, 4> input, uint i : SV_OutputControlPointID)
{
    HS_Terrain_Output output;

    output.posW = input[i].posW;
    output.tex = input[i].tex;
    output.detailTex = input[i].detailTex;

    return output;
}

struct DS_Terrain_Output
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float2 tex : TEXCOORD0;
    float2 detailTex : TEXCOORD1;
};

[domain("quad")]
DS_Terrain_Output DS_Terrain(HSC_Terrain_Output input, float2 uv : SV_DomainLocation, const OutputPatch<HS_Terrain_Output, 4> quad)
{
    DS_Terrain_Output output;
	
    output.posW = lerp(lerp(quad[0].posW, quad[1].posW, uv.x), lerp(quad[2].posW, quad[3].posW, uv.x), uv.y);
    output.tex = lerp(lerp(quad[0].tex, quad[1].tex, uv.x), lerp(quad[2].tex, quad[3].tex, uv.x), uv.y);
    output.detailTex = lerp(lerp(quad[0].detailTex, quad[1].detailTex, uv.x), lerp(quad[2].detailTex, quad[3].detailTex, uv.x), uv.y);

    output.posW.y = gtxTerrainHeight.SampleLevel(gssTerrain, output.tex, 0).r;
    output.posH = mul(mul(float4(output.posW, 1.0f), gmtxView), gmtxProjection);

    return output;
}

float4 PS_Terrain(DS_Terrain_Output input) : SV_Target
{
    float4 cColor = gtxTerrainDiffuse.Sample(gssTerrain, input.tex);

 //   output.color = Lighting(position, vNormal);

    return cColor;
}
