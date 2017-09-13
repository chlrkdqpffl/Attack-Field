
static const float2 g_SpecPowerRange = { 10.0, 100.0 };
static const float g_SpecPower = 10.f;
static const float g_SpecIntensity = 0.1f;

struct PS_GBUFFER_OUTPUT
{
    float4 DiffuseSpecInt   : SV_TARGET0;
    float4 Normal           : SV_TARGET1;
    float4 SpecPow          : SV_TARGET2;
};