
cbuffer cbViewProjectionMatrix : register(b0) // VS Set
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

cbuffer cbWorldMatrix : register(b1) // VS Set
{
    matrix gmtxWorld : packoffset(c0);
};

struct VS_DIFFUSED_INPUT
{
    float3 position : POSITION;
};

struct VS_DIFFUSED_OUTPUT
{
    float4 position : SV_POSITION;
};

struct VS_DIFFUSED_INSTANCED_INPUT
{
    float3 position : POSITION;
    float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_DIFFUSED_INSTANCED_OUTPUT
{
    float4 position : SV_POSITION;
};


VS_DIFFUSED_OUTPUT VSBoundingBox(VS_DIFFUSED_INPUT input)
{
    VS_DIFFUSED_OUTPUT output = (VS_DIFFUSED_OUTPUT) 0;
    output.position = mul(float4(input.position, 1.0f), mul(mul(gmtxWorld, gmtxView), gmtxProjection));

    return (output);
}

VS_DIFFUSED_INSTANCED_OUTPUT VSInstancedBoundingBox(VS_DIFFUSED_INSTANCED_INPUT input)
{
    VS_DIFFUSED_INSTANCED_OUTPUT output = (VS_DIFFUSED_INSTANCED_OUTPUT) 0;
 
    float3 positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
    output.position = mul(mul(float4(positionW, 1.0f), gmtxView), gmtxProjection);

    return (output);
}

float4 PSBoundingBox(VS_DIFFUSED_OUTPUT input) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float4 PSBoundingBoxCollision(VS_DIFFUSED_OUTPUT input) : SV_Target
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
