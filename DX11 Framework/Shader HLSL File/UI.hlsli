
Texture2D gtxtDefault : register(t0);
SamplerState gssDefault : register(s0);

//-------------------------------------------------------------------------------------------------------------------------------

struct VS_UI_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VS_UI_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

VS_UI_OUTPUT VS_UI(VS_UI_INPUT input)
{
    VS_UI_OUTPUT output = (VS_UI_OUTPUT) 0;
    output.position = float4(input.position, 1.0f);

    output.texCoord = input.texCoord;
    return (output);
}

float4 PS_UI(VS_UI_OUTPUT input) : SV_Target
{
    return gtxtDefault.Sample(gssDefault, input.texCoord);
}