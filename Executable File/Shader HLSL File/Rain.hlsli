#include "ParticleCommon.hlsli"

// fxc /E DrawGS /T gs_5_0 /Od /Zi /Fo CompiledVS.fxo Rain.hlsli
static const float3 gRainAccelW = { -1.0f, -15.0f, 0.0f };

PARTICLE_INPUT VSParticleStreamOut(PARTICLE_INPUT vin)
{
    return vin;
}

[maxvertexcount(6)]
void GSParticleStreamOut(point PARTICLE_INPUT input[1], inout PointStream<PARTICLE_INPUT> ptStream)
{
    input[0].age += gfTimeStep;
    if (input[0].type == PARTICLE_TYPE_EMITTER)
    {
        if (input[0].age > 0.001f)
        {
            for (int i = 0; i < 5; ++i)
            {
                float3 vRandom = float3(50.0f, 10.0f, 50.0f) * RandVec3((float) i / 5.0f);      // offset 찾아야 함

                PARTICLE_INPUT particle = (PARTICLE_INPUT) 0;
                particle.position = gvParticleEmitPosition.xyz + vRandom;
 
                particle.velocity = float3(0.0f, -20.0f, 0.0f);
                particle.size = float2(1.0f, 1.0f);
                particle.age = 0.0f;
                particle.type = PARTICLE_TYPE_FLARE;

                ptStream.Append(particle);
            }
            input[0].age = 0.0f;
        }
        ptStream.Append(input[0]);
    }
    else
    {
        if (input[0].age <= 4.0f)
            ptStream.Append(input[0]);
    }
}


struct VertexOut
{
    float3  PosW : POSITION;
    uint    Type : TYPE;
};

VertexOut VSParticleDraw(PARTICLE_INPUT vin)
{
    VertexOut vout;
    float t = vin.age;

    vout.PosW = 0.5f * t * t * gRainAccelW + t * vin.velocity + vin.position;
    vout.Type = vin.type;

    return vout;
}

struct GeoOut
{
    float4 PosH : SV_Position;
    float2 Tex : TEXCOORD;
};

[maxvertexcount(2)]
void GSParticleDraw(point VertexOut gin[1], inout LineStream<GeoOut> lineStream)
{
    if (gin[0].Type != PARTICLE_TYPE_EMITTER)
    {
        float3 p0 = gin[0].PosW;
        float3 p1 = gin[0].PosW + 0.04f * gRainAccelW;
		
        matrix mtxViewProjection = mul(gmtxView, gmtxProjection);
        GeoOut v0;
        v0.PosH = mul(float4(p0, 1.0f), mtxViewProjection);
        v0.Tex = float2(0.0f, 0.0f);
        lineStream.Append(v0);

        GeoOut v1;
        v1.PosH = mul(float4(p1, 1.0f), mtxViewProjection);
        v1.Tex = float2(1.0f, 1.0f);
        lineStream.Append(v1);
    }
}

float4 PSParticleDraw(GeoOut input) : SV_TARGET
{
    return gtxtParticleTextureArray.Sample(gLinearWarpSS, float3(input.Tex, 0));
}