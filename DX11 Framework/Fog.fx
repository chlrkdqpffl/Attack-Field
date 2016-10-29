
#define LINEAR_FOG 	        1.0f
#define EXP_FOG	            2.0f
#define EXP2_FOG	        3.0f
#define E					2.71828182846

// fxc /E Fog /T vs_5_0 /Od /Zi /Fo CompiledVS.fxo Fog.fx

cbuffer cbCamera : register(b2)
{
    float4 gvCameraPosition;
};

static const float4 gcFogColor = { 0.75f, 0.75f, 0.75f, 1.0f };
static const float4 gvFogParameter = { LINEAR_FOG, 100.0f, 300.0f, 0.6f };

/*
cbuffer cbFog : register(cb3)
{
    float4 gcFogColor;
    float4 gvFogParameter; //(Mode, Start, End, Density)
};
*/

float4 Fog(float4 cColor, float3 vPosition)
{
    float fDistanceToCamera = length(gvCameraPosition.xyz - vPosition);
    float fFogFactor = 0.0f;

    if (gvFogParameter.x == LINEAR_FOG)
    {
        fFogFactor = saturate((fDistanceToCamera - gvFogParameter.y) / gvFogParameter.z);
    }
    /*
    else if (gvFogParameter.x == EXPONENTIAL_FOG)
    {
        //fFogFactor = 1.0f / exp( fDistanceToCamera * gvFogParameter.z);
        fFogFactor = 1 / pow(2.71828182846, float((mul(fDistanceToCamera, gvFogParameter.z))));
    }
    else if (gvFogParameter.x == EXPONENTIAL_SQUARED_FOG)
    {
        fFogFactor = 1.0f / exp(pow(fDistanceToCamera * gvFogParameter.z, 2));
    }
    */
    return (lerp(cColor, gcFogColor, fFogFactor));
}
