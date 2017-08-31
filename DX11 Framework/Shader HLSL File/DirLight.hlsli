#include "common.hlsli"
//     fxc /E DirLightPS /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo DirLight.hlsli

cbuffer cbDirLight : register(b7)
{
    float3 AmbientDown : packoffset(c0);
    float3 AmbientRange : packoffset(c1);
    float3 DirToLight : packoffset(c2);
    float3 DirLightColor : packoffset(c3);
}

static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
	float2(1.0, 1.0),
	float2(-1.0, -1.0),
	float2(1.0, -1.0),
};

/////////////////////////////////////////////////////////////////////////////
// Vertex shader
/////////////////////////////////////////////////////////////////////////////
struct VS_OUTPUT
{
    float4 Position : SV_Position; // vertex position 
    float2 texCoord : TEXCOORD0;
};

VS_OUTPUT DirLightVS(uint VertexID : SV_VertexID)
{
    VS_OUTPUT Output;

    Output.Position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
    Output.texCoord = Output.Position.xy;

    return Output;
}

/////////////////////////////////////////////////////////////////////////////
// Pixel shaders
/////////////////////////////////////////////////////////////////////////////

// Ambient light calculation helper function
float3 CalcAmbient(float3 normal, float3 color)
{
	// Convert from [-1, 1] to [0, 1]
    float up = normal.y * 0.5 + 0.5;

	// Calculate the ambient value
    float3 ambient = AmbientDown + up * AmbientRange;

	// Apply the ambient value to the color
    return ambient * color;
}

// Directional light calculation helper function
float3 CalcDirectional(float3 position, Material material)
{
   // Phong diffuse
    float NDotL = dot(DirToLight, material.normal);
    float3 finalColor = DirLightColor.rgb * saturate(NDotL);
   
   // Blinn specular
    float3 ToEye = EyePosition - position;
    ToEye = normalize(ToEye);
    float3 HalfWay = normalize(ToEye + DirToLight);
    float NDotH = saturate(dot(HalfWay, material.normal));
    finalColor += DirLightColor.rgb * pow(NDotH, material.specPow) * material.specIntensity;

    return finalColor * material.diffuseColor.rgb;
}

float4 DirLightPS(VS_OUTPUT input) : SV_TARGET
{
    SURFACE_DATA GBufferData = UnpackGBuffer_Loc(input.Position.xy);
	
    Material mat;
    MaterialFromGBuffer(GBufferData, mat);

	// Reconstruct the world position
    float3 position = CalcWorldPos(input.texCoord, GBufferData.LinearDepth);

	// Calculate the ambient color
//    float3 finalColor = CalcAmbient(mat.normal, mat.diffuseColor.rgb); - 버그로 인하여 주석 처리

	// Calculate the directional light
 //   float3 finalColor = float3(g_f4Var.x, g_f4Var.x, g_f4Var.x);
    float3 finalColor = float3(0.15f, 0.15f, 0.15f);
    finalColor += CalcDirectional(position, mat);

	// Return the final color
    return float4(finalColor, 1.0);
}
