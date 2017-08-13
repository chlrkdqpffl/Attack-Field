#include "ParticleCommon.hlsli"

// fxc /E GSParticleStreamOut /T gs_5_0 /Od /Zi /Fo CompiledVS.fxo Particle.fx

//static const float3 gFireAccelW = { 0.0f, 7.8f, 0.0f };
static const float3 gFireAccelW = { 0.0f, 8.0f, 0.0f };

PARTICLE_INPUT VSParticleStreamOut(PARTICLE_INPUT input)
{
    return input;
}

[maxvertexcount(2)]
void GSParticleStreamOut(point PARTICLE_INPUT input[1], inout PointStream<PARTICLE_INPUT> pointStream)
{
 //   input[0].age += gfTimeStep * g_f4Var.x;
    input[0].age += gfTimeStep * 0.4f;
    if (input[0].type == PARTICLE_TYPE_EMITTER)
    {
        if (input[0].age > 0.005f)
        {
            float3 vRandom = RandUnitVec3(0.0f);
            vRandom.x *= 0.5f;
            vRandom.z *= 0.5f;

            PARTICLE_INPUT particle = (PARTICLE_INPUT) 0;
            particle.position = gvParticleEmitPosition.xyz;
            particle.velocity = 3.0f * vRandom;
  //          particle.velocity = g_f4Var.z * vRandom;
            particle.size = float2(3.0f, 3.0f);
           // particle.size = float2(1.0f, 1.0f);
  //          particle.size = float2(g_f4Var.y, g_f4Var.y);
            particle.age = 0.0f;
            particle.type = PARTICLE_TYPE_FLARE;

            pointStream.Append(particle);
            input[0].age = 0.0f;
        }
        pointStream.Append(input[0]);
    }
    else
    {
        if (input[0].age <= 1.0f)
            pointStream.Append(input[0]);
    }
}



PARTICLE_OUTPUT VSParticleDraw(PARTICLE_INPUT input)
{
    PARTICLE_OUTPUT output;
   
//    float3 accel = { 0, g_f4Var.y, 0 };
    float t = input.age;
    output.position = (0.5f * t * t * gFireAccelW) + (t * input.velocity) + input.position;
//    output.position = (0.5f * t * t * accel) + (t * input.velocity) + input.position;
    

    float fOpacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
  //  float fOpacity = 1.0f - smoothstep(0.0f, 1.0f, t);

    output.color = float4(1.0f, 1.0f, 1.0f, fOpacity);
    output.size = input.size;
    output.type = input.type;

    return output;
}


[maxvertexcount(4)]
void GSParticleDraw(point PARTICLE_OUTPUT input[1], inout TriangleStream<GS_PARTICLE_OUT> triStream)
{
    if (input[0].type == PARTICLE_TYPE_EMITTER)
        return;

    float3 vLook = normalize(gvCameraPosition.xyz - input[0].position);
    float3 vRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), vLook));
    float3 vUp = cross(vLook, vRight);

    float fHalfWidth = 0.5f * input[0].size.x, fHalfHeight = 0.5f * input[0].size.y;
    float4 vQuad[4];
    vQuad[0] = float4(input[0].position + fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
    vQuad[1] = float4(input[0].position + fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);
    vQuad[2] = float4(input[0].position - fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
    vQuad[3] = float4(input[0].position - fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);

    matrix mtxViewProjection = mul(gmtxView, gmtxProjection);
    GS_PARTICLE_OUT output = (GS_PARTICLE_OUT) 0;

    float2 TexCoord[4] =
    {
        float2(0.0f, 1.0f), float2(1.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 0.0f)
    };

    [unroll]
    for (int i = 0; i < 4; i++)
    {
        output.position = mul(vQuad[i], mtxViewProjection);
        output.texCoord = TexCoord[i];
        output.color = input[0].color;
        triStream.Append(output);
    }
}


float4 PSParticleDraw(GS_PARTICLE_OUT input) : SV_Target
{
//    float4 cColor = gtxtParticleTextureArray.Sample(gLinearWarpSS, input.texCoord);
//    return (cColor * input.color);
    return gtxtParticleTextureArray.Sample(gLinearWarpSS, float3(input.texCoord, 0)) * input.color;
}