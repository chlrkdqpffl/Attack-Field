
#define LINEAR_FOG 	        1.0f
#define EXP_FOG	            2.0f
#define EXP2_FOG	        3.0f
#define E					2.71828182846

static const float4 gcFogColor = { 0.75f, 0.75f, 0.75f, 1.0f };
static const float4 gvFogParameter = { LINEAR_FOG, 2.0f, 175.0f, 0.8f };

cbuffer cbCamera : register(cb2)
{
    float4 gvCameraPosition;
};

/*
cbuffer cbFog : register(cb3)
{
    float4 gcFogColor;
    float4 gvFogParameter; //(Mode, Start, End, Density)
};
*/

float4 Fog(float4 cColor, float3 vPosition)
{
    float3 vCameraPosition = gvCameraPosition.xyz;
    float3 vPositionToCamera = vCameraPosition– vPosition;
    float fDistanceToCamera = length(vPositionToCamera);
    float fFogFactor = 0.0f;
    if (gvFogParameter.x == LINEAR_FOG)
    {
        float fFogRange = gvFogParameter.z - gvFogParameter.y;
        fFogFactor = saturate(fDistanceToCamera - gvFogParameter.y) / fFogRange;
    }
    float4 cColorByFog = lerp(cColor, gcFogColor, fFogFactor);
    return (cColorByFog);
}
