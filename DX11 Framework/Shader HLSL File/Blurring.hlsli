
//     fxc /E HorzBlurCS /T cs_5_0 /Od /Zi /Fo CompiledShader.fxo Blurring.hlsli

Texture2D gtxInput;
RWTexture2D<float4> gtxtRWOutput;

static float gfWeights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };
static const int gBlurRadius = 5;

#define ThreadNum 256
groupshared float4 gTextureCache[(ThreadNum + 2 * gBlurRadius)];


[numthreads(ThreadNum, 1, 1)]
void HorzBlurCS(int3 vGroupThreadID : SV_GroupThreadID, int3 vDispatchThreadID : SV_DispatchThreadID)
{
	// Fill local thread storage to reduce bandwidth.  To blur 
	// N pixels, we will need to load N + 2*BlurRadius pixels
	// due to the blur radius.
	
	// This thread group runs N threads.  To get the extra 2*BlurRadius pixels, 
	// have 2*BlurRadius threads sample an extra pixel.
    if (vGroupThreadID.x < gBlurRadius)
    {
    	// Clamp out of bound samples that occur at image borders.
        int x = max(vDispatchThreadID.x - gBlurRadius, 0);
        gTextureCache[vGroupThreadID.x] = gtxInput[int2(x, vDispatchThreadID.y)];
    }
    else if (vGroupThreadID.x >= ThreadNum - gBlurRadius)
    {
        // Clamp out of bound samples that occur at image borders.
        int x = min(vDispatchThreadID.x + gBlurRadius, gtxInput.Length.x - 1);
        gTextureCache[vGroupThreadID.x + 2 * gBlurRadius] = gtxInput[int2(x, vDispatchThreadID.y)];

    }
    // Clamp out of bound samples that occur at image borders.
    gTextureCache[vGroupThreadID.x + gBlurRadius] = gtxInput[min(vDispatchThreadID.xy, gtxInput.Length.xy - 1)];
   
   // Wait for all threads to finish.
    GroupMemoryBarrierWithGroupSync();

    // Now blur each pixel.
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