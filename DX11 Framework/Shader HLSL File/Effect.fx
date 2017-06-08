
//#define _WITH_SKYBOX_TEXTURE_ARRAY
#define _WITH_SKYBOX_TEXTURE_CUBE
//#define _WITH_TERRAIN_TEXTURE_ARRAY

//    fxc /E PSTexturedLightingTangent /T ps_5_0 /Od /Zi /Fo CompiledVS.fxo Effect.fx

//-------------------------------------------------------------------------------------------------------------------------------
// Constant Buffer Variables
//-------------------------------------------------------------------------------------------------------------------------------
cbuffer cbViewProjectionMatrix : register(b0)                // VS Set
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
};

cbuffer cbWorldMatrix : register(b1)                        // VS Set
{
	matrix		gmtxWorld : packoffset(c0);
};

cbuffer cbTextureMatrix : register(b2)                      // VS Set
{
	matrix		gmtxTexture : packoffset(c0);
};
/*
cbuffer cbCameraPosition : register(b2)                     // PS Set
{
    float4      gvCameraPosition : packoffset(c0);
};
*/
/*
cbuffer cbTerrain : register(b3)                            // PS Set
{
	int4		gvTerrainTextureIndex : packoffset(c0);
};

cbuffer cbSkyBox : register(b4)                             // PS Set
{
	int4		gvSkyBoxTextureIndex : packoffset(c0);
};
*/
cbuffer cbRenderOption : register(b5)                       // PS Set
{
    float4 gbRenderOption : packoffset(c0);                 // (x : Fog Render, y : BoundingBox Render,    z : UI Opacity )
};

cbuffer cbSkinned : register(b7)							// VS Set
{
	matrix gBoneTransform[60];
};

Texture2D gtxDiffuse			: register(t0);

Texture2D gtxTerrain			: register(t4);
Texture2D gtxTerrainDetail		: register(t5);

#ifdef _WITH_TERRAIN_TEXTURE_ARRAY
    Texture2D gtxtTerrainDetails[10] : register(t3);
#else
    Texture2D gtxtTerrainDetail : register(t3);
#endif

#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
    Texture2DArray gtxtSkyBox : register(t13);
#else
    #ifdef _WITH_SKYBOX_TEXTURE_CUBE
        TextureCube gtxtSkyBox : register(t13);
    #else
        Texture2D gtxtSkyBox : register(t13);
    #endif
#endif

SamplerState gssDefault         : register(s0);
SamplerState gssDefaultDetail   : register(s1);
SamplerState gssTerrain         : register(s2);
SamplerState gssTerrainDetail   : register(s3);
SamplerState gssSkyBox          : register(s4);

#include "Light.hlsli"
#include "Fog.hlsli"
#include "Normal.hlsli"

//-------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_SKYBOX_CUBEMAP_INPUT
{
	float3	position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VS_SKYBOX_CUBEMAP_OUTPUT
{
	float3	positionL : POSITION;
	float4	position : SV_POSITION;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_DETAIL_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

struct VS_DETAIL_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

//-------------------------------------------------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------------------------------------------------

struct VS_TEXTURED_LIGHTING_TANGENT_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD0;
};

struct VS_TEXTURED_LIGHTING_TANGENT_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float2 texCoord : TEXCOORD0;
};


//-------------------------------------------------------------------------------------------------------------------------------
struct VS_DETAIL_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

struct VS_DETAIL_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

struct VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_INSTANCED_DIFFUSED_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR0;
	float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_DIFFUSED_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_INSTANCED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
};

//-------------------------------------------------------------------------------------------------------------------------------
struct VS_INSTANCED_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

//-------------------------------------------------------------------------------------------------------------------------------
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

//	float2 texCoordShadow : TEXCOORD1;
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

struct VS_SKINNED_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
	float4 boneWeights : WEIGHTS;
	float4 boneIndices : BONEINDICES;
};

struct VS_SKINNED_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD0;
};

//-------------------------------------------------------------------------------------------------------------------------------
VS_DIFFUSED_OUTPUT VSDiffusedColor(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output = (VS_DIFFUSED_OUTPUT)0;
	//output.position = mul(float4(input.position, 1.0f), mul(mul(gmtxWorld, gmtxView), gmtxProjection));
    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.color = input.color;

	return(output);
}

float4 PSDiffusedColor(VS_DIFFUSED_OUTPUT input) : SV_Target
{
	return(input.color);
}

//-------------------------------------------------------------------------------------------------------------------------------
VS_LIGHTING_OUTPUT VSLightingColor(VS_LIGHTING_INPUT input)
{
	VS_LIGHTING_OUTPUT output = (VS_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	return(output);
}

float4 PSLightingColor(VS_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);

	return(cIllumination);
}

//-------------------------------------------------------------------------------------------------------------------------------
VS_TEXTURED_OUTPUT VSTexturedColor(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output = (VS_TEXTURED_OUTPUT)0;
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.texCoord = input.texCoord;

	return(output);
}

float4 PSTexturedColor(VS_TEXTURED_OUTPUT input) : SV_Target
{
	float4 cColor = gtxDiffuse.Sample(gssDefault, input.texCoord);

	return(cColor);
}

//-------------------------------------------------------------------------------------------------------------------------------
VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLightingColor(VS_TEXTURED_LIGHTING_INPUT input)
{
	VS_TEXTURED_LIGHTING_OUTPUT output = (VS_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.texCoord = input.texCoord;

	return(output);
}

float4 PSTexturedLightingColor(VS_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cColor = gtxDiffuse.Sample(gssDefault, input.texCoord) * cIllumination;
//    return float4(input.normalW, 1.0);
	return(cColor);
}

//-------------------------------------------------------------------------------------------------------------------------------

VS_TEXTURED_LIGHTING_TANGENT_OUTPUT VSTexturedLightingTangent(VS_TEXTURED_LIGHTING_TANGENT_INPUT input)
{
    VS_TEXTURED_LIGHTING_TANGENT_OUTPUT output = (VS_TEXTURED_LIGHTING_TANGENT_OUTPUT) 0;
    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.normalW = mul(input.normal, (float3x3) gmtxWorld);
    output.tangentW = mul(input.tangent, (float3x3) gmtxWorld);
    output.texCoord = input.texCoord;

    return (output);
}

float4 PSTexturedLightingTangent(VS_TEXTURED_LIGHTING_TANGENT_OUTPUT input) : SV_Target
{
    float3 normalW = CalcNormal(input.normalW, input.tangentW, input.texCoord);
   
    float4 cIllumination = Lighting(input.positionW, normalW);
    float4 cColor = gtxDiffuse.Sample(gssDefault, input.texCoord) * cIllumination;
//    return float4(input.normalW, 0.0f);

    return (cColor);
}

//-------------------------------------------------------------------------------------------------------------------------------
VS_DETAIL_TEXTURED_OUTPUT VSDetailTexturedColor(VS_DETAIL_TEXTURED_INPUT input)
{
	VS_DETAIL_TEXTURED_OUTPUT output = (VS_DETAIL_TEXTURED_OUTPUT)0;
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.texCoordBase = input.texCoordBase;
	output.texCoordDetail = input.texCoordDetail;

	return(output);
}

float4 PSDetailTexturedColor(VS_DETAIL_TEXTURED_OUTPUT input) : SV_Target
{
	float4 cBaseTexColor = gtxDiffuse.Sample(gssDefault, input.texCoordBase);
	float4 cDetailTexColor = gtxTerrainDetail.Sample(gssDefaultDetail, input.texCoordDetail);
	float4 cColor = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
	//    float4 cAlphaTexColor = gtxtTerrainAlphaTexture.Sample(gTerrainSamplerState, input.texcoord0);
	//    float4 cColor = cIllumination * lerp(cBaseTexColor, cDetailTexColor, cAlphaTexColor.r);
	return(cColor);
}

//-------------------------------------------------------------------------------------------------------------------------------
VS_DETAIL_TEXTURED_LIGHTING_OUTPUT VSDetailTexturedLightingColor(VS_DETAIL_TEXTURED_LIGHTING_INPUT input)
{
	VS_DETAIL_TEXTURED_LIGHTING_OUTPUT output = (VS_DETAIL_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.texCoordBase = input.texCoordBase;
	output.texCoordDetail = input.texCoordDetail;

	return(output);
}

VS_DETAIL_TEXTURED_LIGHTING_OUTPUT VSAnimatedDetailTexturedLightingColor(VS_DETAIL_TEXTURED_LIGHTING_INPUT input)
{
	VS_DETAIL_TEXTURED_LIGHTING_OUTPUT output = (VS_DETAIL_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.texCoordBase = input.texCoordBase;
	output.texCoordDetail = mul(float4(input.texCoordDetail, 0.0f, 1.0f), gmtxTexture).xy;

	return(output);
}

float4 PSDetailTexturedLightingColor(VS_DETAIL_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
		float4 cBaseTexColor = gtxDiffuse.Sample(gssDefault, input.texCoordBase);
		float4 cDetailTexColor = gtxTerrainDetail.Sample(gssDefaultDetail, input.texCoordDetail);
		float4 cColor = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
		//    float4 cAlphaTexColor = gtxtTerrainAlphaTexture.Sample(gTerrainSamplerState, input.texcoord0);
		//    float4 cColor = cIllumination * lerp(cBaseTexColor, cDetailTexColor, cAlphaTexColor.r);
		return(cColor*cIllumination);
}

VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT VSTerrainDetailTexturedLightingColor(VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_INPUT input)
{
	VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT output = (VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.texCoordBase = input.texCoordBase;
	output.texCoordDetail = input.texCoordDetail;

	return(output);
}

#ifdef _WITH_TERRAIN_TEXTURE_ARRAY
float4 PSTerrainDetailTexturedLightingColor(VS_DETAIL_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cBaseTexColor = gtxtTerrain.Sample(gssTerrain, input.texCoordBase);
	float4 cDetailTexColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//	cDetailTexColor = gtxtTerrainDetails[gvTerrainTextureIndex.a].Sample(gssTerrainDetail, input.texCoordDetail); //Error
	///*
	switch (gvTerrainTextureIndex.a)
	{
	case 0:
		cDetailTexColor = gtxtTerrainDetails[0].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 1:
		cDetailTexColor = gtxtTerrainDetails[1].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 2:
		cDetailTexColor = gtxtTerrainDetails[2].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 3:
		cDetailTexColor = gtxtTerrainDetails[3].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 4:
		cDetailTexColor = gtxtTerrainDetails[4].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 5:
		cDetailTexColor = gtxtTerrainDetails[5].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 6:
		cDetailTexColor = gtxtTerrainDetails[6].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 7:
		cDetailTexColor = gtxtTerrainDetails[7].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 8:
		cDetailTexColor = gtxtTerrainDetails[8].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	case 9:
		cDetailTexColor = gtxtTerrainDetails[9].Sample(gssTerrainDetail, input.texCoordDetail);
		break;
	}
	//*/
	float4 cColor = saturate((cIllumination * cBaseTexColor * 0.7f) + (cDetailTexColor * 0.3f));

		return(cColor);
}
#else
float4 PSTerrainDetailTexturedLightingColor(VS_TERRAIN_DETAIL_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cBaseTexColor = gtxTerrain.Sample(gssTerrain, input.texCoordBase);
	float4 cDetailTexColor = gtxtTerrainDetail.Sample(gssTerrainDetail, input.texCoordDetail);
	float4 cColor = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f)) * cIllumination;

    if (gbRenderOption.x == 1.0f)
        cColor = Fog(cColor, input.positionW);

	return(cColor);
}
#endif

//-------------------------------------------------------------------------------------------------------------------------------
#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
float4 PSSkyBoxTexturedColor(VS_TEXTURED_OUTPUT input) : SV_Target
{
	float3 uvw = float3(input.texCoord, gvSkyBoxTextureIndex.a);
	float4 cColor = gtxtSkyBox.Sample(gssSkyBox, uvw);
	return(cColor);
}
#else
#ifdef _WITH_SKYBOX_TEXTURE_CUBE
VS_SKYBOX_CUBEMAP_OUTPUT VSSkyBoxTexturedColor(VS_SKYBOX_CUBEMAP_INPUT input)
{
	VS_SKYBOX_CUBEMAP_OUTPUT output = (VS_SKYBOX_CUBEMAP_OUTPUT)0;
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.positionL = input.position;

	return(output);
}

float4 PSSkyBoxTexturedColor(VS_SKYBOX_CUBEMAP_OUTPUT input) : SV_Target
{
	float4 cColor = gtxtSkyBox.Sample(gssSkyBox, input.positionL);

	return(cColor);
}
#else
float4 PSSkyBoxTexturedColor(VS_TEXTURED_OUTPUT input) : SV_Target
{
	float4 cColor = gtxtSkyBox.Sample(gssSkyBox, input.texCoord);
	return(cColor);
}
#endif
#endif

// ====================================================================================================== //
// ============================================= Instancing ============================================= //
VS_INSTANCED_DIFFUSED_OUTPUT VSInstancedDiffusedColor(VS_INSTANCED_DIFFUSED_INPUT input)
{
	VS_INSTANCED_DIFFUSED_OUTPUT output = (VS_INSTANCED_DIFFUSED_OUTPUT)0;
	output.position = mul(mul(mul(float4(input.position, 1.0f), input.mtxTransform), gmtxView), gmtxProjection);
	output.color = input.color;
	return(output);
}

float4 PSInstancedDiffusedColor(VS_INSTANCED_DIFFUSED_OUTPUT input) : SV_Target
{
	return(input.color);
}

VS_INSTANCED_LIGHTING_OUTPUT VSInstancedLightingColor(VS_INSTANCED_LIGHTING_INPUT input)
{
	VS_INSTANCED_LIGHTING_OUTPUT output = (VS_INSTANCED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)input.mtxTransform);
	output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	return(output);
}

float4 PSInstancedLightingColor(VS_INSTANCED_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);

	return(cIllumination);
}

VS_INSTANCED_TEXTURED_OUTPUT VSInstancedTexturedColor(VS_INSTANCED_TEXTURED_INPUT input)
{
	VS_INSTANCED_TEXTURED_OUTPUT output = (VS_INSTANCED_TEXTURED_OUTPUT)0;
	output.position = mul(mul(mul(float4(input.position, 1.0f), input.mtxTransform), gmtxView), gmtxProjection);
	output.texCoord = input.texCoord;

	return(output);
}

float4 PSInstancedTexturedColor(VS_INSTANCED_TEXTURED_OUTPUT input) : SV_Target
{
	float4 cColor = gtxDiffuse.Sample(gssDefault, input.texCoord);

	return(cColor);
}

VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT VSInstancedTexturedLightingColor(VS_INSTANCED_TEXTURED_LIGHTING_INPUT input)
{
	VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT output = (VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)input.mtxTransform);
	output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.texCoord = input.texCoord;
	return(output);
}

float4 PSInstancedTexturedLightingColor(VS_INSTANCED_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cColor = gtxDiffuse.Sample(gssDefault, input.texCoord) * cIllumination;

    if (gbRenderOption.x == 1.0f)
        cColor = Fog(cColor, input.positionW);

	return(cColor);
}

VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_OUTPUT VSInstancedTexturedTangentLighting(VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_INPUT input)
{
    VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_OUTPUT output = (VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_OUTPUT) 0;
    output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    output.normalW = mul(input.normal, (float3x3) input.mtxTransform);
    output.tangentW = mul(input.tangent, (float3x3) input.mtxTransform);
    output.texCoord = input.texCoord;
    return (output);
}

float4 PSInstancedTexturedTangentLighting(VS_INSTANCED_TEXTURED_TANGENT_LIGHTING_OUTPUT input) : SV_Target
{
    float3 normalW = CalcNormal(input.normalW, input.tangentW, input.texCoord);
    float4 cIllumination = Lighting(input.positionW, normalW);

//    input.normalW = normalize(input.normalW);
 //   float4 cIllumination = Lighting(input.positionW, input.normalW);
    float4 cColor = gtxDiffuse.Sample(gssDefault, input.texCoord) * cIllumination;

    return cColor;
}