#include "ParticleCommon.hlsli"

// fxc /E DrawGS /T gs_5_0 /Od /Zi /Fo CompiledVS.fxo Rain.hlsli

cbuffer cbFixed
{
    float3 gAccelW = { -1.0f, -9.8f, 0.0f };
};


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
        if (input[0].age > 0.002f)
        {
            for (int i = 0; i < 5; ++i)
            {
				// 카메라 위쪽에 빗방울들을 분포 시킨다. 
				// <- 분포도 조절 
           //     float3 vRandom = 300.0f * RandVec3((float) i / 5.0f);
            //    vRandom.y = 20.0f;

            /*
                float3 vRandom = gtxtRandomTexture.SampleLevel(gParticleSamplerState, gfGameTime, 0).xyz;
                vRandom = normalize(vRandom);
                vRandom.x *= 10;
                vRandom.z *= 10;
                vRandom.y = 20.0f;
                */
               // float3 vRandom = 10.0f * RandVec3((float) i / 5.0f);
               // vRandom.y = 5.0f;

                float3 vRandom = 5 * RandVec3((float) i / 5.0f);
                vRandom.y = 5.0f;


                PARTICLE_INPUT particle = (PARTICLE_INPUT) 0;
                particle.position = gvParticleEmitPosition.xyz + vRandom;
         //       particle.velocity= float3(0.0f, -1.0f, 0.0f);
                particle.velocity = float3(0.0f, 0.0f, 0.0f);
                particle.size = float2(1.0f, 1.0f);
                particle.age = 0.0f;
                particle.type = PARTICLE_TYPE_FLARE;

                ptStream.Append(particle);
            }

			// 방출할 시간을 재설정한다.
            input[0].age = 0.0f;
        }

		// 방출기 입자 하나는 항상 유지한다.
        ptStream.Append(input[0]);
    }
    else
    {
		// 여기서 입자를 유지할 조건들을 지정한다.
		// 구체적인 조건은 입자 시스템마다 다를 수 있다. 
        if (input[0].age <= 3.0f)
            ptStream.Append(input[0]);
    }
}


///
/// 렌더링 기법
///
struct VertexOut
{
    float3 PosW : POSITION;
    uint Type : TYPE;
};

VertexOut VSParticleDraw(PARTICLE_INPUT vin)
{
    VertexOut vout;

    float t = vin.age;

	// 상수 가속 공식.
    vout.PosW = vin.position + float3(0.0f, -3.0f * 3.0f * (t * 20), 0.0f); // 0.5f*t*t*gAccelW + t*vin.InitialVelW + vin.InitialPosW;
 //   vout.PosW = 0.5f * t * t * gAccelW + t * vin.velocity + vin.position;
	
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
	// 방출기 입자는 그리지 않는다. 
    if (gin[0].Type != PARTICLE_TYPE_EMITTER)
    {
		// 선분이 가속도 방향으로 기울어지게 한다.
        float3 p0 = gin[0].PosW;
        float3 p1 = gin[0].PosW + 0.08f * float3(0.0f, -9.8f * 3.0f, 0.0f);

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
    return gtxtParticleTextureArray.Sample(gParticleSamplerState, input.Tex);
}
