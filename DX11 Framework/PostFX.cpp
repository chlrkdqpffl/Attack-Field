#include "stdafx.h"
#include "PostFX.h"

CPostFX::CPostFX() : 
	m_fMiddleGrey(0.0025f), m_fWhite(1.5f),
	m_pDownScale1DBuffer(NULL), m_pDownScale1DUAV(NULL), m_pDownScale1DSRV(NULL),
	m_pDownScaleCB(NULL), m_pFinalPassCB(NULL),
	m_pAvgLumBuffer(NULL), m_pAvgLumUAV(NULL), m_pAvgLumSRV(NULL),
	m_pDownScaleFirstPassCS(NULL), m_pDownScaleSecondPassCS(NULL), m_pFullScreenQuadVS(NULL), m_pFinalPassPS(NULL)
{
}

CPostFX::~CPostFX()
{

}

void CPostFX::Initialize(ID3D11Device* pDevice, UINT width, UINT height)
{
	Deinit();

	// Find the amount of thread groups needed for the downscale operation
	m_nWidth = width;
	m_nHeight = height;
	m_nDownScaleGroups = (UINT)ceil((float)(m_nWidth * m_nHeight / 16) / 1024.0f);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate down scaled luminance buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.StructureByteStride = sizeof(float);
	bufferDesc.ByteWidth = m_nDownScaleGroups * bufferDesc.StructureByteStride;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	HR(pDevice->CreateBuffer( &bufferDesc, NULL, &m_pDownScale1DBuffer ) );
	DXUT_SetDebugName( m_pDownScale1DBuffer, "PostFX - Down Scale 1D Buffer" );

	D3D11_UNORDERED_ACCESS_VIEW_DESC DescUAV;
	ZeroMemory( &DescUAV, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC) );
	DescUAV.Format = DXGI_FORMAT_UNKNOWN;
	DescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	DescUAV.Buffer.NumElements = m_nDownScaleGroups;
	HR(pDevice->CreateUnorderedAccessView( m_pDownScale1DBuffer, &DescUAV, &m_pDownScale1DUAV ) );
	DXUT_SetDebugName( m_pDownScale1DSRV, "PostFX - Luminance Down Scale 1D SRV" );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate average luminance buffer
	D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd;
	ZeroMemory(&dsrvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	dsrvd.Format = DXGI_FORMAT_UNKNOWN;
	dsrvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	dsrvd.Buffer.NumElements = m_nDownScaleGroups;
	HR(pDevice->CreateShaderResourceView( m_pDownScale1DBuffer, &dsrvd, &m_pDownScale1DSRV ) );
	DXUT_SetDebugName( m_pDownScale1DSRV, "PostFX - Down Scale 1D SRV" );

	bufferDesc.ByteWidth = sizeof(float);
	HR(pDevice->CreateBuffer( &bufferDesc, NULL, &m_pAvgLumBuffer ) );
	DXUT_SetDebugName( m_pAvgLumBuffer, "PostFX - Average Luminance Buffer" );
	
	DescUAV.Buffer.NumElements = 1;
	HR(pDevice->CreateUnorderedAccessView( m_pAvgLumBuffer, &DescUAV, &m_pAvgLumUAV ) );
	DXUT_SetDebugName( m_pAvgLumUAV, "PostFX - Average Luminance UAV" );

	dsrvd.Buffer.NumElements = 1;
	HR(pDevice->CreateShaderResourceView( m_pAvgLumBuffer, &dsrvd, &m_pAvgLumSRV ) );
	DXUT_SetDebugName( m_pAvgLumSRV, "PostFX - Average Luminance SRV" );

	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(TDownScaleCB);
	HR(pDevice->CreateBuffer( &bufferDesc, NULL, &m_pDownScaleCB ) );
	DXUT_SetDebugName( m_pDownScaleCB, "PostFX - Down Scale CB" );

	bufferDesc.ByteWidth = sizeof(TFinalPassCB);
	HR(pDevice->CreateBuffer( &bufferDesc, NULL, &m_pFinalPassCB ) );
	DXUT_SetDebugName( m_pFinalPassCB, "PostFX - Final Pass CB" );

	// Create ShaderFile
	wstring strFileName;
	strFileName = L"Shader HLSL File/PostDownScaleFX.hlsli";
	CShader::CreateComputeShaderFromFile(pDevice, strFileName, "DownScaleFirstPass", "cs_5_0", &m_pDownScaleFirstPassCS);
	DXUT_SetDebugName(m_pDownScaleFirstPassCS, "Post FX - Down Scale First Pass CS" );
	CShader::CreateComputeShaderFromFile(pDevice, strFileName, "DownScaleSecondPass", "cs_5_0", &m_pDownScaleSecondPassCS);
	DXUT_SetDebugName(m_pDownScaleFirstPassCS, "Post FX - Down Scale Second Pass CS");

	strFileName = L"Shader HLSL File/PostFX.hlsli";
	CShader::CreateVertexShaderFromFile(pDevice, strFileName, "FullScreenQuadVS", "vs_5_0", &m_pFullScreenQuadVS);
	DXUT_SetDebugName(m_pFullScreenQuadVS, "Post FX - Full Screen Quad VS");
	CShader::CreatePixelShaderFromFile(pDevice, strFileName, "FinalPassPS", "ps_5_0", &m_pFinalPassPS);
    DXUT_SetDebugName( m_pFinalPassPS, "Post FX - Final Pass PS" );
}

void CPostFX::Deinit()
{
	ReleaseCOM( m_pDownScale1DBuffer );
	ReleaseCOM( m_pDownScale1DUAV );
	ReleaseCOM( m_pDownScale1DSRV );
	ReleaseCOM( m_pDownScaleCB );
	ReleaseCOM( m_pFinalPassCB );
	ReleaseCOM( m_pAvgLumBuffer );
	ReleaseCOM( m_pAvgLumUAV );
	ReleaseCOM( m_pAvgLumSRV );
	ReleaseCOM( m_pDownScaleFirstPassCS );
	ReleaseCOM( m_pDownScaleSecondPassCS );
	ReleaseCOM( m_pFullScreenQuadVS );
	ReleaseCOM( m_pFinalPassPS );
}

void CPostFX::PostProcessing(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pHDRSRV, ID3D11RenderTargetView* pLDRRTV)
{
	// Down scale the HDR image
	ID3D11RenderTargetView* rt[1] = { NULL };
	pd3dImmediateContext->OMSetRenderTargets( 1, rt, NULL );
	DownScale(pd3dImmediateContext, pHDRSRV);

	// Do the final pass
	rt[0] = pLDRRTV;
	pd3dImmediateContext->OMSetRenderTargets( 1, rt, NULL );
	FinalPass(pd3dImmediateContext, pHDRSRV);
}

void CPostFX::DownScale(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pHDRSRV)
{
	// Output
	ID3D11UnorderedAccessView* arrUAVs[1] = { m_pDownScale1DUAV };
	pd3dImmediateContext->CSSetUnorderedAccessViews( 0, 1, arrUAVs, (UINT*)(&arrUAVs) );

	// Input
	ID3D11ShaderResourceView* arrViews[1] = {pHDRSRV};
	pd3dImmediateContext->CSSetShaderResources(0, 1, arrViews);

	// Constants
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pd3dImmediateContext->Map(m_pDownScaleCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	TDownScaleCB* pDownScale = (TDownScaleCB*)MappedResource.pData; 
	pDownScale->nWidth = m_nWidth  / 4;
	pDownScale->nHeight = m_nHeight / 4;
	pDownScale->nTotalPixels = pDownScale->nWidth * pDownScale->nHeight;
	pDownScale->nGroupSize = m_nDownScaleGroups;
	pd3dImmediateContext->Unmap(m_pDownScaleCB, 0);
	ID3D11Buffer* arrConstBuffers[1] = { m_pDownScaleCB };
	pd3dImmediateContext->CSSetConstantBuffers(CS_CB_SLOT_DownScale, 1, arrConstBuffers);


	// Shader
	pd3dImmediateContext->CSSetShader( m_pDownScaleFirstPassCS, NULL, 0 );

	// Execute the downscales first pass with enough groups to cover the entire full res HDR buffer
	pd3dImmediateContext->Dispatch(m_nDownScaleGroups, 1, 1);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Second pass - reduce to a single pixel

	// Outoput
	ZeroMemory(arrUAVs, sizeof(arrUAVs));
	arrUAVs[0] = m_pAvgLumUAV;
	pd3dImmediateContext->CSSetUnorderedAccessViews( 0, 1, arrUAVs, (UINT*)(&arrUAVs) );

	// Input
	arrViews[0] = m_pDownScale1DSRV;
	pd3dImmediateContext->CSSetShaderResources(0, 1, arrViews);

	// Constants
	pd3dImmediateContext->CSSetConstantBuffers(0, 1, arrConstBuffers);

	// Shader
	pd3dImmediateContext->CSSetShader( m_pDownScaleSecondPassCS, NULL, 0 );

	// Excute with a single group - this group has enough threads to process all the pixels
	pd3dImmediateContext->Dispatch(1, 1, 1);

	// Cleanup
	pd3dImmediateContext->CSSetShader( NULL, NULL, 0 );
	ZeroMemory(arrConstBuffers, sizeof(arrConstBuffers));
	pd3dImmediateContext->CSSetConstantBuffers(0, 1, arrConstBuffers);
	ZeroMemory(arrViews, sizeof(arrViews));
	pd3dImmediateContext->CSSetShaderResources(0, 1, arrViews);
	ZeroMemory(arrUAVs, sizeof(arrUAVs));
	pd3dImmediateContext->CSSetUnorderedAccessViews( 0, 1, arrUAVs, (UINT*)(&arrUAVs) );
}

void CPostFX::FinalPass(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pHDRSRV)
{
	ID3D11ShaderResourceView* arrViews[2] = {pHDRSRV, m_pAvgLumSRV};
	pd3dImmediateContext->PSSetShaderResources(0, 2, arrViews);

	// Constants
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pd3dImmediateContext->Map(m_pFinalPassCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	TFinalPassCB* pFinalPass = (TFinalPassCB*)MappedResource.pData; 
	pFinalPass->fMiddleGrey = m_fMiddleGrey;
	pFinalPass->fLumWhiteSqr = m_fWhite;
	pFinalPass->fLumWhiteSqr *= pFinalPass->fMiddleGrey; // Scale by the middle grey value
	pFinalPass->fLumWhiteSqr *= pFinalPass->fLumWhiteSqr; // Squre
	pd3dImmediateContext->Unmap(m_pFinalPassCB, 0);
	ID3D11Buffer* arrConstBuffers[1] = { m_pFinalPassCB };
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, arrConstBuffers);

	pd3dImmediateContext->IASetInputLayout( NULL );
	pd3dImmediateContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	pd3dImmediateContext->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
	pd3dImmediateContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	
	ID3D11SamplerState* arrSamplers[1] = { STATEOBJ_MGR->g_pPointWarpSS };
	pd3dImmediateContext->PSSetSamplers( 0, 1, arrSamplers );

	// Set the shaders
	pd3dImmediateContext->VSSetShader(m_pFullScreenQuadVS, NULL, 0);
	pd3dImmediateContext->PSSetShader(m_pFinalPassPS, NULL, 0);

	pd3dImmediateContext->Draw(4, 0);

	// Cleanup
	ZeroMemory(arrViews, sizeof(arrViews));
	pd3dImmediateContext->PSSetShaderResources(0, 2, arrViews);
	ZeroMemory(arrConstBuffers, sizeof(arrConstBuffers));
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, arrConstBuffers);
	pd3dImmediateContext->VSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->PSSetShader(NULL, NULL, 0);
}