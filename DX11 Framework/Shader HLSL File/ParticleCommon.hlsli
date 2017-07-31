
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
    float3 gvAcceleration;
};

cbuffer cbViewMatrix : register(b2) // GS Buffer
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

Texture1D gtxtRandomTexture : register(t0);
Texture2D gtxtParticleTextureArray : register(t9);
SamplerState gParticleSamplerState : register(s0);


struct PARTICLE_INPUT
{
    float3 position : POSITION;
    float3 velocity : VELOCITY;
    float2 size : SIZE;
    uint type : TYPE;
    float age : AGE;
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
	// Use game time plus offset to sample random texture.
    float u = (gfGameTime + offset);
	
	// coordinates in [-1,1]
    float3 v = gtxtRandomTexture.SampleLevel(gParticleSamplerState, u, 0).xyz;
	
	// project onto unit sphere
    return normalize(v);
}

float3 RandVec3(float offset)
{
	// Use game time plus offset to sample random texture.
    float u = (gfGameTime + offset);
	
	// coordinates in [-1,1]
    float3 v = gtxtRandomTexture.SampleLevel(gParticleSamplerState, u, 0).xyz;
	
    return v;
}