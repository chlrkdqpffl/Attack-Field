
#define PARTICLE_TYPE_EMITTER	0
#define PARTICLE_TYPE_FLARE		1

// fxc /E GSParticleDraw /T gs_5_0 /Od /Zi /Fo CompiledVS.fxo Particle.fx


cbuffer cbWorldMatrix : register(b1) // VS Buffer
{
    matrix gmtxWorld : packoffset(c0);
};

cbuffer cbCameraPosition : register(b10) // PS Set
{
    float4 gvCameraPosition : packoffset(c0);
};

cbuffer cbParticleInfo : register(b1) // GS Buffer
{
    float3 gvParticleEmitPosition;
    float gfGameTime;
    float3 gvParticleEmitDirection;
    float gfTimeStep;
};

cbuffer cbViewMatrix : register(b2) // GS Buffer
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

Texture1D gtxtRandomTexture : register(t0);
Texture2DArray gtxtParticleTextureArray : register(t9);
SamplerState gLinearWarpSS: register(s0);


struct PARTICLE_INPUT
{
    float3 position : POSITION;
    float3 velocity : VELOCITY;
    float2 size : SIZE;
    float age : AGE;
    uint type : TYPE;
};

struct PARTICLE_OUTPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    float4 color : COLOR;
    uint type : TYPE;
};

struct GS_PARTICLE_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};


float3 RandUnitVec3(float offset)
{
    float u = (gfGameTime + offset);
    float3 v = gtxtRandomTexture.SampleLevel(gLinearWarpSS, u, 0).xyz;
	
    return normalize(v);
}

float3 RandVec3(float offset)
{
    float u = (gfGameTime + offset);
    float3 v = gtxtRandomTexture.SampleLevel(gLinearWarpSS, u, 0).xyz;
	
    return v;
}