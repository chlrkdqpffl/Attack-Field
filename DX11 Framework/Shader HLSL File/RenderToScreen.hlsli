
//	fxc /E PSTextureToScreen /T ps_5_0 /Od /Zi /Fo CompiledShader.fxo TextureToScreen.hlsli

struct VS_INPUT_Diffuse
{
    float3 position : POSITION;
    float4 color    : COLOR0;
};

struct VS_OUTPUT_Diffuse
{
    float4 position : SV_POSITION;
    float4 color    : COLOR0;
};


struct VS_INPUT_Texture
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT_Texture
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};


cbuffer cbRenderOption : register(b5) // PS Set
{
    float4 gbRenderOption : packoffset(c0); // (x : Fog Render, y : BoundingBox Render, z : UI Opacity)
};

SamplerState gssDefault : register(s0);
Texture2D gtxtDiffuse : register(t0);

// --------------------------------------------------------------------------------------------------------- //

VS_OUTPUT_Diffuse VSDiffusedColor(VS_INPUT_Diffuse input)
{
    VS_OUTPUT_Diffuse output = (VS_OUTPUT_Diffuse) 0;
	
    output.position = float4(input.position, 1.0f);
    output.color = input.color;

    return (output);
}

float4 PSDiffusedColor(VS_OUTPUT_Diffuse input) : SV_Target
{
    return (input.color);
}


VS_OUTPUT_Texture VSTextureToScreen(VS_INPUT_Texture input)
{
    VS_OUTPUT_Texture output = (VS_OUTPUT_Texture) 0;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return (output);
}

float4 PSTextureToScreen(VS_OUTPUT_Texture input) : SV_Target
{
    float4 cColor = gtxtDiffuse.Sample(gssDefault, input.texCoord);
    cColor = cColor * gbRenderOption.w;

    return (cColor);
}