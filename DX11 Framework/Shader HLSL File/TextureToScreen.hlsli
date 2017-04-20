
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


float4 WeightedAveragefilterBlurring(float2 textCoord)
{
    float2 tex = textCoord;
    float2 minus = float2(0.99, 0.99);
    float2 plus = float2(1.01, 1.01);
    float4 color = 0.0f;

    color += 0.0625 * (gtxtDiffuse.Sample(gssDefault, float2(tex.x * minus.x, tex.y * minus.y)));
    color += 0.125 * (gtxtDiffuse.Sample(gssDefault, float2(tex.x, tex.y * minus.y)));
    color += 0.0625 * (gtxtDiffuse.Sample(gssDefault, float2(tex.x * plus.x, tex.y * minus.y)));
					  
    color += 0.125 * (gtxtDiffuse.Sample(gssDefault, float2(tex.x * minus.x, tex.y)));
    color += 0.25 * (gtxtDiffuse.Sample(gssDefault, float2(tex.x, tex.y)));
    color += 0.125 * (gtxtDiffuse.Sample(gssDefault, float2(tex.x * plus.x, tex.y)));
					   
    color += 0.0625 * (gtxtDiffuse.Sample(gssDefault, float2(tex.x * minus.x, tex.y * plus.y)));
    color += 0.125 * (gtxtDiffuse.Sample(gssDefault, float2(tex.x, tex.y * plus.y)));
    color += 0.0625 * (gtxtDiffuse.Sample(gssDefault, float2(tex.x * plus.x, tex.y * plus.y)));

    return color;
}

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
}