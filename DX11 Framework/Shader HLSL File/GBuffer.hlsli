
//	fxc /E PSTextureToScreen /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo TextureToScreen.hlsli

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

SamplerState gssDefault : register(s0);
Texture2D gtxtDiffuse : register(t0);


// --------------------------------------------------------------------------------------------------------- //

VS_OUTPUT VSTextureToScreen(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return (output);
}

float4 PSTextureToScreen(VS_OUTPUT input) : SV_Target
{
    return gtxtDiffuse.Sample(gssDefault, input.texCoord);
    //return WeightedAveragefilterBlurring(input.texCoord);
}