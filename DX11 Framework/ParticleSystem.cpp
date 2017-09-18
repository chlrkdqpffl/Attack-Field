#pragma once
#include "stdafx.h"
#include "ParticleSystem.h"

float R(float a, float b) {	return(a + (float)(rand() / (float)RAND_MAX) * (b - a));}

CParticleSystem::CParticleSystem()
{
}

CParticleSystem::~CParticleSystem()
{
	// Buffer
	ReleaseCOM(m_pd3dInitialVertexBuffer);
	ReleaseCOM(m_pd3dStreamOutVertexBuffer);
	ReleaseCOM(m_pd3dDrawVertexBuffer);
	ReleaseCOM(m_pCBParticleInfo);

	// Shader
	ReleaseCOM(m_pd3dVertexLayout);
	ReleaseCOM(m_pd3dVertexShader);
	ReleaseCOM(m_pd3dGeometryShader);
	ReleaseCOM(m_pd3dPixelShader);
	ReleaseCOM(m_pd3dSOVertexShader);
	ReleaseCOM(m_pd3dSOGeometryShader);

	// SRV
	ReleaseCOM(m_pd3dsrvRandomTexture);
//	ReleaseCOM(m_pd3dsrvTextureArray);
}

void CParticleSystem::Initialize
	(ID3D11Device *pd3dDevice, ID3D11ShaderResourceView* pd3dsrvTexArray, ID3D11ShaderResourceView* pd3dsrvRandomTexture, UINT nMaxParticles, ID3D11BlendState *blendState, float maxAge)
{
	m_fMaxAge = maxAge;
	m_pd3dsrvTextureArray = pd3dsrvTexArray;
	m_pd3dsrvRandomTexture = pd3dsrvRandomTexture;
	m_pd3dBlendState = blendState;
	m_nMaxParticles = nMaxParticles;

	CreateBuffer(pd3dDevice);

	/*
	// Blend State
	D3D11_BLEND_DESC d3dBlendStateDesc;
	ZeroMemory(&d3dBlendStateDesc, sizeof(D3D11_BLEND_DESC));
	d3dBlendStateDesc.IndependentBlendEnable = false;
	ZeroMemory(&d3dBlendStateDesc.RenderTarget[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	d3dBlendStateDesc.AlphaToCoverageEnable = false;
	d3dBlendStateDesc.RenderTarget[0].BlendEnable = true;
	d3dBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	d3dBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pd3dDevice->CreateBlendState(&d3dBlendStateDesc, &m_pd3dBlendState);
	
	D3D11_BLEND_DESC d3dBlendStateDesc;
	ZeroMemory(&d3dBlendStateDesc, sizeof(D3D11_BLEND_DESC));
	d3dBlendStateDesc.IndependentBlendEnable = false;
	ZeroMemory(&d3dBlendStateDesc.RenderTarget[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	d3dBlendStateDesc.AlphaToCoverageEnable = false;
	d3dBlendStateDesc.RenderTarget[0].BlendEnable = true;
	
	d3dBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	d3dBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	// 불 파티클 아래 옵션이 더 자연스러움
	d3dBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	
	d3dBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pd3dDevice->CreateBlendState(&d3dBlendStateDesc, &m_pd3dBlendState);

	DXUT_SetDebugName(m_pd3dBlendState, "ParticleBS");
	*/
}

void CParticleSystem::CreateShader(ID3D11Device *pd3dDevice, const wstring& wstring)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE",		0, DXGI_FORMAT_R32G32_FLOAT,	0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "AGE",		0, DXGI_FORMAT_R32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE",		0, DXGI_FORMAT_R32_UINT,		0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CShader::CreateVertexShaderFromFile(pd3dDevice, wstring, "VSParticleStreamOut", "vs_5_0", &m_pd3dSOVertexShader, d3dInputLayout, 5, &m_pd3dVertexLayout);
	CShader::CreateVertexShaderFromFile(pd3dDevice, wstring, "VSParticleDraw", "vs_5_0", &m_pd3dVertexShader, d3dInputLayout, 5, &m_pd3dVertexLayout);
	CShader::CreateGeometryShaderFromFile(pd3dDevice, wstring, "GSParticleDraw", "gs_5_0", &m_pd3dGeometryShader);
	CreateSOGeometryShaderFromFile(pd3dDevice, wstring, "GSParticleStreamOut", "gs_5_0", &m_pd3dSOGeometryShader);
	CShader::CreatePixelShaderFromFile(pd3dDevice, wstring, "PSParticleDraw", "ps_5_0", &m_pd3dPixelShader);
}

void CParticleSystem::Update(float fDeltaTime)
{
	m_fTimeStep = fDeltaTime;
	m_fGameTime += fDeltaTime;
	m_fAge += m_fTimeStep;

	if (m_fMaxAge <= m_fAge)
		m_bIsActive = false;
}

void CParticleSystem::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	/*
	// Rain을 제외한 모든 파티클 절두체 컬링 - 미룸
	BoundingBox bcBoundingCube;
	bcBoundingCube.Extents = XMFLOAT3(TWBAR_MGR->g_nSelect, TWBAR_MGR->g_nSelect, TWBAR_MGR->g_nSelect);

	XMMATRIX mtx;
	mtx = XMMatrixIdentity();
	XMMatrixTranslation(m_f3EmitPosition.x, m_f3EmitPosition.y, m_f3EmitPosition.z);

	bcBoundingCube.Transform(bcBoundingCube, mtx);
	bool bIsVisible = SCENE_MGR->g_pCamera->IsInFrustum(&bcBoundingCube);

	if (!bIsVisible)
		return;
	*/

	UpdateConstantBuffer(pd3dDeviceContext);
	
	UINT stride = sizeof(CParticle);
	UINT offset = 0;

	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);

	pd3dDeviceContext->VSSetShader(m_pd3dSOVertexShader, NULL, 0);//
	pd3dDeviceContext->GSSetShader(m_pd3dSOGeometryShader, NULL, 0);//
	pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pNoCullRS);
	
	pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

	ID3D11DepthStencilState* prevDepthStencilState; UINT prevStencil;
	pd3dDeviceContext->OMGetDepthStencilState(&prevDepthStencilState, &prevStencil);

	pd3dDeviceContext->OMSetDepthStencilState(STATEOBJ_MGR->g_pDisableDepthDSS, 0);
	pd3dDeviceContext->GSSetSamplers(0, 1, &STATEOBJ_MGR->g_pLinearWrapSS);
	pd3dDeviceContext->GSSetShaderResources(GS_TEXTURE_SLOT_RANDOM, 1, &m_pd3dsrvRandomTexture);
	pd3dDeviceContext->SOSetTargets(1, &m_pd3dStreamOutVertexBuffer, &offset);

	if (m_bInitializeParticle)
	{
		pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dInitialVertexBuffer, &stride, &offset);
		pd3dDeviceContext->Draw(1, 0);
		m_bInitializeParticle = false;
	}
	else
	{
		pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dDrawVertexBuffer, &stride, &offset);
		pd3dDeviceContext->DrawAuto();
	}
	swap(m_pd3dDrawVertexBuffer, m_pd3dStreamOutVertexBuffer);

	ID3D11Buffer *pd3dBuffers[1] = { nullptr };
	UINT pStreamOffSets[1] = { 0 };
	pd3dDeviceContext->SOSetTargets(1, pd3dBuffers, pStreamOffSets);
	
	// Draw
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, nullptr, 0);
	pd3dDeviceContext->GSSetShader(m_pd3dGeometryShader, nullptr, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, nullptr, 0);

	pd3dDeviceContext->OMSetDepthStencilState(STATEOBJ_MGR->g_pNoDepthWritesDSS, 0);
	pd3dDeviceContext->OMSetBlendState(m_pd3dBlendState, nullptr, 0xffffffff);

	pd3dDeviceContext->PSSetSamplers(0, 1, &STATEOBJ_MGR->g_pLinearWrapSS);
	pd3dDeviceContext->PSSetShaderResources(PS_CB_SLOT_PARTICLE, 1, &m_pd3dsrvTextureArray);

	pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dDrawVertexBuffer, &stride, &offset);
	pd3dDeviceContext->DrawAuto();

	// Clear
	pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);
//	pd3dDeviceContext->GSSetConstantBuffers(GS_CB_SLOT_PARTICLE, 1, nullptr);

	ID3D11SamplerState* nullSampler[1] = { nullptr };
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	pd3dDeviceContext->GSSetSamplers(0, 1, nullSampler);
	pd3dDeviceContext->GSSetShaderResources(0, 1, nullSRV);

	pd3dDeviceContext->OMSetDepthStencilState(prevDepthStencilState, prevStencil);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

ID3D11ShaderResourceView* CParticleSystem::CreateRandomTexture1DSRV(ID3D11Device *pd3dDevice)
{
	XMFLOAT4 RV[1024];
	for (int i = 0; i < 1024; i++)
		RV[i] = XMFLOAT4(R(-1.0f, 1.0f), R(-1.0f, 1.0f), R(-1.0f, 1.0f), R(-1.0f, 1.0f));
	D3D11_SUBRESOURCE_DATA d3dSubResourceData;
	d3dSubResourceData.pSysMem = RV;
	d3dSubResourceData.SysMemPitch = sizeof(XMFLOAT4) * 1024;
	d3dSubResourceData.SysMemSlicePitch = 0;

	D3D11_TEXTURE1D_DESC d3dTextureDesc;
	ZeroMemory(&d3dTextureDesc, sizeof(D3D11_TEXTURE1D_DESC));
	d3dTextureDesc.Width = 1024;
	d3dTextureDesc.MipLevels = 1;
	d3dTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	d3dTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	d3dTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTextureDesc.ArraySize = 1;
	d3dTextureDesc.CPUAccessFlags = 0;
	d3dTextureDesc.MiscFlags = 0;
	ID3D11Texture1D *pd3dTexture;
	pd3dDevice->CreateTexture1D(&d3dTextureDesc, &d3dSubResourceData, &pd3dTexture);

	ID3D11ShaderResourceView *pd3dsrvTexture = nullptr;
	pd3dDevice->CreateShaderResourceView(pd3dTexture, NULL, &pd3dsrvTexture);
	DXUT_SetDebugName(pd3dsrvTexture, "Random 1D Texture");
	ReleaseCOM(pd3dTexture);

	return(pd3dsrvTexture);
}

void CParticleSystem::UpdateConstantBuffer(ID3D11DeviceContext* pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pCBParticleInfo, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

	CB_PARTICLEINFO *pcbParticleInfo = (CB_PARTICLEINFO*)d3dMappedResource.pData;
	pcbParticleInfo->m_fGameTime = m_fGameTime / SCENE_MGR->g_nowScene->GetFrameSpeed();
	pcbParticleInfo->m_fTimeStep = m_fTimeStep;
	pcbParticleInfo->m_d3dxvEmitPosition = m_f3EmitPosition;
	pcbParticleInfo->m_d3dxvEmitDirection = m_f3EmitDirection;

	pd3dDeviceContext->Unmap(m_pCBParticleInfo, 0);

	pd3dDeviceContext->GSSetConstantBuffers(GS_CB_SLOT_PARTICLE, 1, &m_pCBParticleInfo);
}

void CParticleSystem::CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, const wstring& pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;
	
	// UINT Stream, LPCSTR SemanticName, BYTE SemanticIndex, BYTE StartComponent, BYTE ComponentCount, BYTE OuputSlot
	D3D11_SO_DECLARATION_ENTRY pSODecls[] = {
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "VELOCITY", 0, 0, 3, 0 },
		{ 0, "SIZE", 0, 0, 2, 0 },
		{ 0, "AGE", 0, 0, 1, 0 },
		{ 0, "TYPE", 0, 0, 1, 0 }
	};
	UINT pBufferStrides[1] = { sizeof(CParticle) };

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;

	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreateGeometryShaderWithStreamOutput(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(),
			pSODecls, 5, pBufferStrides, 1,	0, NULL, ppd3dGeometryShader);

		pd3dShaderBlob->Release();

		DXUT_SetDebugName(*ppd3dGeometryShader, "ParticleGSWithSO");
	}
	else
	{
		MessageBox(NULL, L"SOGSERROR", L"SOGSERROR", MB_OK);
		exit(0);
	}
}

ID3D11ShaderResourceView *CParticleSystem::CreateTexture2DArraySRV(ID3D11Device *pd3dDevice, _TCHAR(*ppstrFilePaths)[128], UINT nTextures)
{
	ID3D11ShaderResourceView* pd3dsrvParticleTexture;
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, *ppstrFilePaths, NULL, NULL, &pd3dsrvParticleTexture, NULL);
	return pd3dsrvParticleTexture;

	D3DX11_IMAGE_LOAD_INFO d3dxImageLoadInfo;
	d3dxImageLoadInfo.Width = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Height = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Depth = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.FirstMipLevel = 0;
	d3dxImageLoadInfo.MipLevels = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Usage = D3D11_USAGE_STAGING;
	d3dxImageLoadInfo.BindFlags = 0;
	d3dxImageLoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	d3dxImageLoadInfo.MiscFlags = 0;
	d3dxImageLoadInfo.Format = DXGI_FORMAT_FROM_FILE; 
	d3dxImageLoadInfo.Filter = D3DX11_FILTER_NONE;
	d3dxImageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;
	d3dxImageLoadInfo.pSrcInfo = 0;

	ID3D11Texture2D **ppd3dTextures = new ID3D11Texture2D*[nTextures];
	for (UINT i = 0; i < nTextures; i++) 
		D3DX11CreateTextureFromFile(pd3dDevice, ppstrFilePaths[i], &d3dxImageLoadInfo, 0, (ID3D11Resource **)&ppd3dTextures[i], 0);

	D3D11_TEXTURE2D_DESC d3dTexure2DDesc;

	ppd3dTextures[0]->GetDesc(&d3dTexure2DDesc);

	D3D11_TEXTURE2D_DESC d3dTexture2DArrayDesc;
	d3dTexture2DArrayDesc.Width = d3dTexure2DDesc.Width;
	d3dTexture2DArrayDesc.Height = d3dTexure2DDesc.Height;
	d3dTexture2DArrayDesc.MipLevels = d3dTexure2DDesc.MipLevels;
	d3dTexture2DArrayDesc.ArraySize = nTextures;
	d3dTexture2DArrayDesc.Format = d3dTexure2DDesc.Format;
	d3dTexture2DArrayDesc.SampleDesc.Count = 1;
	d3dTexture2DArrayDesc.SampleDesc.Quality = 0;
	d3dTexture2DArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DArrayDesc.CPUAccessFlags = 0;
	d3dTexture2DArrayDesc.MiscFlags = 0;

	ID3D11Texture2D *pd3dTexture2DArray;
	pd3dDevice->CreateTexture2D(&d3dTexture2DArrayDesc, 0, &pd3dTexture2DArray);

	ID3D11DeviceContext *pd3dDeviceContext;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedTexture2D;
	for (UINT t = 0; t < nTextures; t++)
	{
		for (UINT m = 0; m < d3dTexure2DDesc.MipLevels; m++)
		{
			pd3dDeviceContext->Map(ppd3dTextures[t], m, D3D11_MAP_READ, 0, &d3dMappedTexture2D);
			pd3dDeviceContext->UpdateSubresource(pd3dTexture2DArray, D3D11CalcSubresource(m, t, d3dTexure2DDesc.MipLevels), 0, d3dMappedTexture2D.pData, d3dMappedTexture2D.RowPitch, d3dMappedTexture2D.DepthPitch);
			pd3dDeviceContext->Unmap(ppd3dTextures[t], m);
		}
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC d3dTextureSRVDesc;
	ZeroMemory(&d3dTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dTextureSRVDesc.Format = d3dTexture2DArrayDesc.Format;
	d3dTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	d3dTextureSRVDesc.Texture2DArray.MostDetailedMip = 0;
	d3dTextureSRVDesc.Texture2DArray.MipLevels = d3dTexture2DArrayDesc.MipLevels;
	d3dTextureSRVDesc.Texture2DArray.FirstArraySlice = 0;
	d3dTextureSRVDesc.Texture2DArray.ArraySize = nTextures;

	ID3D11ShaderResourceView *pd3dsrvTextureArray;
	pd3dDevice->CreateShaderResourceView(pd3dTexture2DArray, &d3dTextureSRVDesc, &pd3dsrvTextureArray);

	DXUT_SetDebugName(pd3dsrvTextureArray, "ParticleTexture2DArray");

	if (pd3dTexture2DArray) pd3dTexture2DArray->Release();

	for (UINT i = 0; i < nTextures; i++) if (ppd3dTextures[i]) ppd3dTextures[i]->Release();
	delete[] ppd3dTextures;

	if (pd3dDeviceContext) pd3dDeviceContext->Release();

	return pd3dsrvTextureArray;
}

void CParticleSystem::CreateBuffer(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CParticle);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	d3dBufferDesc.MiscFlags = 0;
	d3dBufferDesc.StructureByteStride = 0;

	CParticle particle;
	ZeroMemory(&particle, sizeof(CParticle));

	D3D11_SUBRESOURCE_DATA d3dSubResourceData;
	d3dSubResourceData.pSysMem = &particle;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dSubResourceData, &m_pd3dInitialVertexBuffer);
	DXUT_SetDebugName(m_pd3dInitialVertexBuffer, "InitialVertex");

	d3dBufferDesc.ByteWidth = sizeof(CParticle) * m_nMaxParticles;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dStreamOutVertexBuffer);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dDrawVertexBuffer);
	DXUT_SetDebugName(m_pd3dStreamOutVertexBuffer, "StreamOutVertex");
	DXUT_SetDebugName(m_pd3dDrawVertexBuffer, "DrawVertex");

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(CB_PARTICLEINFO);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pCBParticleInfo);
	DXUT_SetDebugName(m_pCBParticleInfo, "CB_Particle Info");
}