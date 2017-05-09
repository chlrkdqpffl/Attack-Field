
//     fxc /E HorzBlurCS /T cs_5_0 /Od /Zi /Fo CompiledShader.fxo Blurring.hlsli

Texture2D gtxInput : register(t0);
RWTexture2D<float4> gtxtRWOutput;

static float gfWeights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };
static const int gBlurRadius = 5;

#define ThreadNum 256
groupshared float4 gTextureCache[(ThreadNum + 2 * gBlurRadius)];

[numthreads(ThreadNum, 1, 1)]
void HorzBlurCS(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
    if (vGroupThreadID.x < gBlurRadius)
    {
        int x = max(vDispatchThreadID.x - gBlurRadius, 0);
        gTextureCache[vGroupThreadID.x] = gtxInput[int2(x, vDispatchThreadID.y)];
    }
    else if (vGroupThreadID.x >= ThreadNum - gBlurRadius)
    {
        int x = min(vDispatchThreadID.x + gBlurRadius, gtxInput.Length.x - 1);
        gTextureCache[vGroupThreadID.x + 2 * gBlurRadius] = gtxInput[int2(x, vDispatchThreadID.y)];

    }
    gTextureCache[vGroupThreadID.x + gBlurRadius] = gtxInput[min(vDispatchThreadID.xy, gtxInput.Length.xy - 1)];
   
    GroupMemoryBarrierWithGroupSync();

    float4 cBlurredColor = float4(0, 0, 0, 0);

    [unroll]
    for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
        int k = vGroupThreadID.x + gBlurRadius + i;
        cBlurredColor += gfWeights[i + gBlurRadius] * gTextureCache[k];
    }

    gtxtRWOutput[vDispatchThreadID.xy] = cBlurredColor;
}


[numthreads(1, ThreadNum, 1)]
void VertBlurCS(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
    if (vGroupThreadID.y < gBlurRadius)
    {
        int y = max(vDispatchThreadID.y - gBlurRadius, 0);
        gTextureCache[vGroupThreadID.y] = gtxInput[int2(vDispatchThreadID.x, y)];
    }
    else if (vGroupThreadID.y >= ThreadNum - gBlurRadius)
    {
        int y = min(vDispatchThreadID.y + 5, gtxInput.Length.y - 1);
        gTextureCache[vGroupThreadID.y + 2 * gBlurRadius] = gtxInput[int2(vDispatchThreadID.x, y)];
    }
    gTextureCache[vGroupThreadID.y + gBlurRadius] = gtxInput[min(vDispatchThreadID.xy, gtxInput.Length.xy - 1)];

    GroupMemoryBarrierWithGroupSync();

    float4 cBlurredColor = float4(0, 0, 0, 0);

    [unroll]
    for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
        int k = vGroupThreadID.y + gBlurRadius + i;
        cBlurredColor += gfWeights[i + gBlurRadius] * gTextureCache[k];
    }

    gtxtRWOutput[vDispatchThreadID.xy] = cBlurredColor;
}



// 상수버퍼를 이용하여 가변적인 가중치 블러링
/*

Texture2D gtxInput;
RWTexture2D<float4> gtxtRWOutput;

cbuffer cbWeights : register(b0) // CS Set
{
    float4 gfWeights[4] : packoffset(c0);
};

static const int gBlurRadius = 5;

#define ThreadNum 256
groupshared float4 gTextureCache[(ThreadNum + 2 * gBlurRadius)];

[numthreads(ThreadNum, 1, 1)]
void HorzBlurCS(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
    if (vGroupThreadID.x < gBlurRadius)
    {
        int x = max(vDispatchThreadID.x - gBlurRadius, 0);
        gTextureCache[vGroupThreadID.x] = gtxInput[int2(x, vDispatchThreadID.y)];
    }
    else if (vGroupThreadID.x >= ThreadNum - gBlurRadius)
    {
        int x = min(vDispatchThreadID.x + gBlurRadius, gtxInput.Length.x - 1);
        gTextureCache[vGroupThreadID.x + 2 * gBlurRadius] = gtxInput[int2(x, vDispatchThreadID.y)];

    }
    gTextureCache[vGroupThreadID.x + gBlurRadius] = gtxInput[min(vDispatchThreadID.xy, gtxInput.Length.xy - 1)];
   
    GroupMemoryBarrierWithGroupSync();

    float4 cBlurredColor = float4(0, 0, 0, 0);

    [unroll]
    for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
        int k = vGroupThreadID.x + gBlurRadius + i;
        if ((i + gBlurRadius) % 4 == 0 )
            cBlurredColor += gfWeights[(i + gBlurRadius) / 4].x * gTextureCache[k];
        else if ((i + gBlurRadius) % 4 == 1)
            cBlurredColor += gfWeights[(i + gBlurRadius) / 4].y * gTextureCache[k];
        else if ((i + gBlurRadius) % 4 == 2)
            cBlurredColor += gfWeights[(i + gBlurRadius) / 4].z * gTextureCache[k];
        else if ((i + gBlurRadius) % 4 == 3)
            cBlurredColor += gfWeights[(i + gBlurRadius) / 4].w * gTextureCache[k];
    }

    gtxtRWOutput[vDispatchThreadID.xy] = cBlurredColor;
}



[numthreads(1, ThreadNum, 1)]
void VertBlurCS(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
    if (vGroupThreadID.y < gBlurRadius)
    {
        int y = max(vDispatchThreadID.y - gBlurRadius, 0);
        gTextureCache[vGroupThreadID.y] = gtxInput[int2(vDispatchThreadID.x, y)];
    }
    else if (vGroupThreadID.y >= ThreadNum - gBlurRadius)
    {
        int y = min(vDispatchThreadID.y + 5, gtxInput.Length.y - 1);
        gTextureCache[vGroupThreadID.y + 2 * gBlurRadius] = gtxInput[int2(vDispatchThreadID.x, y)];
    }
    gTextureCache[vGroupThreadID.y + gBlurRadius] = gtxInput[min(vDispatchThreadID.xy, gtxInput.Length.xy - 1)];

    GroupMemoryBarrierWithGroupSync();

    float4 cBlurredColor = float4(0, 0, 0, 0);

    [unroll]
    for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
        int k = vGroupThreadID.y + gBlurRadius + i;
        if ((i + gBlurRadius) % 4 == 0)
            cBlurredColor += gfWeights[(i + gBlurRadius) / 4].x * gTextureCache[k];
        else if ((i + gBlurRadius) % 4 == 1)
            cBlurredColor += gfWeights[(i + gBlurRadius) / 4].y * gTextureCache[k];
        else if ((i + gBlurRadius) % 4 == 2)
            cBlurredColor += gfWeights[(i + gBlurRadius) / 4].z * gTextureCache[k];
        else if ((i + gBlurRadius) % 4 == 3)
            cBlurredColor += gfWeights[(i + gBlurRadius) / 4].w * gTextureCache[k];
    }

    gtxtRWOutput[vDispatchThreadID.xy] = cBlurredColor;
}
*/