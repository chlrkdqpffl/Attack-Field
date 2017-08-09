#include "ParticleCommon.hlsli"

// fxc /E GSParticleStreamOut /T gs_5_0 /Od /Zi /Fo CompiledVS.fxo Particle.fx



cbuffer cbFixed
{
	// Net constant acceleration used to accerlate the particles.
    float3 gFireAccelW = { 0.0f, 7.8f, 0.0f };
	
	// Texture coordinates used to stretch texture over quad 
	// when we expand point particle into a quad.
    float2 gQuadTexC[4] =
    {
        float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
    };
};
 

PARTICLE_INPUT VSParticleStreamOut(PARTICLE_INPUT input)
{
    return input;
}


PARTICLE_OUTPUT VSParticleDraw(PARTICLE_INPUT input)
{
    PARTICLE_OUTPUT output;

    float t = input.age;
    output.position = (0.5f * gFireAccelW * t * t) + (input.velocity * t) + input.position;

    float fOpacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
  //  float fOpacity = 1.0f - smoothstep(0.0f, 1.0f, t);

    output.color = float4(1.0f, 1.0f, 1.0f, fOpacity);      // 시간이 지날수록 투명해지도록 만듦
    output.size = input.size;
    output.type = input.type;

    return output;
}

[maxvertexcount(2)]
void GSParticleStreamOut(point PARTICLE_INPUT input[1], inout PointStream<PARTICLE_INPUT> pointStream)
{
    input[0].age += gfTimeStep;
    if (input[0].type == PARTICLE_TYPE_EMITTER)
    {
        if (input[0].age > 0.005f)
        {
        /*
            float3 vRandom = gtxtRandomTexture.SampleLevel(gParticleSamplerState, gfGameTime, 0).xyz;
            vRandom = normalize(vRandom);
            vRandom.x *= 0.5f;
            vRandom.z *= 0.5f;
            */
            float3 vRandom = RandUnitVec3(0.0f);
            vRandom.x *= 0.5f;
            vRandom.z *= 0.5f;

            PARTICLE_INPUT particle = (PARTICLE_INPUT) 0;
            particle.position = gvParticleEmitPosition.xyz;
            particle.velocity = 4.0f * vRandom;                 // 초기 스피드는 4, 즉 변경할 수 있다.
            particle.size = float2(3.0f, 3.0f);
           // particle.size = float2(1.0f, 1.0f);
            particle.age = 0.0f;
            particle.type = PARTICLE_TYPE_FLARE;                // 이 타입은 스스로 다른 파티클을 만들어 내는 것이 아닌 스스로 움직이기만 한다는 뜻
            pointStream.Append(particle);
            input[0].age = 0.0f;
       //     input[0].type = PARTICLE_TYPE_EMITTER; // 필요한가...
        }
        pointStream.Append(input[0]);
    }
    else if (input[0].age <= 1.0f)              // 1.0도 수명이므로 굳이 1초로 정하지 않아도 된다.
        pointStream.Append(input[0]);
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
        //float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(0.0f, 1.0f), float2(1.0f, 1.0f)

        float2(0.0f, 1.0f), float2(1.0f, 1.0f),	float2(0.0f, 0.0f),	float2(1.0f, 0.0f)
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
    float4 cColor = gtxtParticleTextureArray.Sample(gParticleSamplerState, input.texCoord);
    return (cColor * input.color);
}