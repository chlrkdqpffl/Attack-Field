#pragma once
#include "stdafx.h"
#include "ParticleSystem.h"

float R(float a, float b) {	return(a + (float)(rand() / (float)RAND_MAX) * (b - a));}

CParticleSystem::CParticleSystem()
{
	m_nMaxParticles = 0;	
	m_bInitializeParticle = false;
	m_fGameTime = 0.0f;			
	m_fTimeStep = 0.0f;		
	m_fAge = 0.0f;
	m_nOffset = 0;
	m_nStride = 0;
	m_d3dxvAcceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvEmitPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvEmitDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_pd3dcbParticleInfo = NULL;
	
	m_pd3dInitialVertexBuffer = NULL;
	m_pd3dStreamOutVertexBuffer = NULL;
	m_pd3dDrawVertexBuffer = NULL;
	m_pd3dVertexLayout = NULL;
	m_pd3dVertexShader = NULL;
	m_pd3dGeometryShader = NULL;
	m_pd3dPixelShader = NULL;
	m_pd3dSOVertexShader = NULL;
	m_pd3dSOGeometryShader = NULL;
	m_pd3dSODepthStencilState = NULL;
	m_pd3dDepthStencilState = NULL;
	m_pd3dBlendState = NULL;
	m_pd3dsrvRandomTexture = NULL;
	m_pd3dsrvTextureArray = NULL;
}

CParticleSystem::~CParticleSystem()
{
	if (m_pd3dInitialVertexBuffer) m_pd3dInitialVertexBuffer->Release();
	if (m_pd3dStreamOutVertexBuffer) m_pd3dStreamOutVertexBuffer->Release(); 
	if (m_pd3dDrawVertexBuffer) m_pd3dDrawVertexBuffer->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release(); 
	if (m_pd3dGeometryShader) m_pd3dGeometryShader->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	if (m_pd3dSOVertexShader) m_pd3dSOVertexShader->Release();
	if (m_pd3dSOGeometryShader) m_pd3dSOGeometryShader->Release();
	if (m_pd3dSODepthStencilState) m_pd3dSODepthStencilState->Release();
	if (m_pd3dDepthStencilState) m_pd3dDepthStencilState->Release();
	if (m_pd3dBlendState) m_pd3dBlendState->Release();
	if (m_pd3dsrvRandomTexture) m_pd3dsrvRandomTexture->Release();
	if (m_pd3dsrvTextureArray) m_pd3dsrvTextureArray->Release();
}

void CParticleSystem::Initialize(ID3D11Device *pd3dDevice, ID3D11ShaderResourceView* pd3dsrvTexArray, ID3D11ShaderResourceView* pd3dsrvRandomTexture, UINT nMaxParticles)
{
	m_pd3dsrvTextureArray = pd3dsrvTexArray;
	m_pd3dsrvRandomTexture = pd3dsrvRandomTexture;

	m_nMaxParticles = nMaxParticles;

	m_bInitializeParticle = true;

	m_fGameTime = 0;
	m_fTimeStep = 0;
	m_fAge = 0;
	
	m_d3dxvAcceleration = XMFLOAT3(0, 0, 0);
	m_d3dxvEmitPosition = XMFLOAT3(0, 0, 0);
	m_d3dxvEmitDirection = XMFLOAT3(0, 0, 0);

	CreateParticle(pd3dDevice, XMFLOAT3(60.0f, 3, 10.0f), XMFLOAT3(0, 1, 0), XMFLOAT3(0, 1, 0));
}
void CParticleSystem::CreateParticle(ID3D11Device *pd3dDevice, XMFLOAT3& Position, XMFLOAT3& Direction, XMFLOAT3& Accelerater)
{
	m_d3dxvEmitPosition = Position;
	m_d3dxvEmitDirection = Direction;
	m_d3dxvAcceleration = Accelerater;
	m_nOffset = 0;
	m_nStride = sizeof(CParticleVertex);

	CreateShaderVariables(pd3dDevice);	
}

void CParticleSystem::CreateShader(ID3D11Device *pd3dDevice, const wstring& wstring)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE", 0, DXGI_FORMAT_R32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CShader::CreateVertexShaderFromFile(pd3dDevice, wstring, "VSParticleStreamOut", "vs_5_0", &m_pd3dSOVertexShader, d3dInputLayout, 5, &m_pd3dVertexLayout);
	CShader::CreateVertexShaderFromFile(pd3dDevice, wstring, "VSParticleDraw", "vs_5_0", &m_pd3dVertexShader, d3dInputLayout, 5, &m_pd3dVertexLayout);
	CShader::CreateGeometryShaderFromFile(pd3dDevice, wstring, "GSParticleDraw", "gs_5_0", &m_pd3dGeometryShader);
	CreateSOGeometryShaderFromFile(pd3dDevice, wstring, "GSParticleStreamOut", "gs_5_0", &m_pd3dSOGeometryShader);
	CShader::CreatePixelShaderFromFile(pd3dDevice, wstring, "PSParticleDraw", "ps_5_0", &m_pd3dPixelShader);

	// 블렌드 상태 설정
	D3D11_BLEND_DESC d3dBlendStateDesc;
	ZeroMemory(&d3dBlendStateDesc, sizeof(D3D11_BLEND_DESC));
	d3dBlendStateDesc.IndependentBlendEnable = false;
	ZeroMemory(&d3dBlendStateDesc.RenderTarget[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	d3dBlendStateDesc.AlphaToCoverageEnable = true;
	d3dBlendStateDesc.RenderTarget[0].BlendEnable = true;
	d3dBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	d3dBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pd3dDevice->CreateBlendState(&d3dBlendStateDesc, &m_pd3dBlendState);

	DXUT_SetDebugName(m_pd3dBlendState, "ParticleBS");
}

void CParticleSystem::Update(float fTimeStep, float fGameTime)
{
	m_fTimeStep = fTimeStep;
	m_fGameTime = fGameTime;
}

void CParticleSystem::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	UpdateShaderVariables(pd3dDeviceContext);
	 
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->SOSetTargets(1, &m_pd3dStreamOutVertexBuffer, &m_nOffset);

	pd3dDeviceContext->VSSetShader(m_pd3dSOVertexShader, NULL, 0);//
	pd3dDeviceContext->GSSetShader(m_pd3dSOGeometryShader, NULL, 0);//
	pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pNoCullRS);
	
	pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

	ID3D11DepthStencilState* prevDepthStencilState; UINT prevStencil;
	pd3dDeviceContext->OMGetDepthStencilState(&prevDepthStencilState, &prevStencil);

	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dSODepthStencilState, 0);
	pd3dDeviceContext->GSSetSamplers(0, 1, &STATEOBJ_MGR->g_pPointWarpSS);
	pd3dDeviceContext->GSSetShaderResources(GS_TEXTURE_SLOT_RANDOM, 1, &m_pd3dsrvRandomTexture);

	if (m_bInitializeParticle)
	{
		pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dInitialVertexBuffer, &m_nStride, &m_nOffset);
		pd3dDeviceContext->Draw(1, 0);
		m_bInitializeParticle = false;
	}
	else
	{
		pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dDrawVertexBuffer, &m_nStride, &m_nOffset);
		pd3dDeviceContext->DrawAuto();
	}
	// 스트림 출력 버퍼를 입력 조립기에 넣을 버퍼로 변경
	ID3D11Buffer *pd3dBuffer = m_pd3dDrawVertexBuffer;
	m_pd3dDrawVertexBuffer = m_pd3dStreamOutVertexBuffer;
	m_pd3dStreamOutVertexBuffer = pd3dBuffer;

	ID3D11Buffer *pd3dBuffers[1] = { nullptr };
	UINT pStreamOffSets[1] = { 0 };
	pd3dDeviceContext->SOSetTargets(1, pd3dBuffers, pStreamOffSets);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, nullptr, 0);
	pd3dDeviceContext->GSSetShader(m_pd3dGeometryShader, nullptr, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, nullptr, 0);

	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 0);
	pd3dDeviceContext->OMSetBlendState(m_pd3dBlendState, nullptr, 0xffffffff);

	pd3dDeviceContext->PSSetSamplers(0, 1, &STATEOBJ_MGR->g_pPointWarpSS);
	pd3dDeviceContext->PSSetShaderResources(PS_CB_SLOT_PARTICLE, 1, &m_pd3dsrvTextureArray);

	pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dDrawVertexBuffer, &m_nStride, &m_nOffset);
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
	D3DXCOLOR RV[1024];
	for (int i = 0; i < 1024; i++)
		RV[i] = D3DXCOLOR(R(-1.0f, 1.0f), R(-1.0f, 1.0f), R(-1.0f, 1.0f), R(-1.0f, 1.0f));
	D3D11_SUBRESOURCE_DATA d3dSubResourceData;
	d3dSubResourceData.pSysMem = RV;
	d3dSubResourceData.SysMemPitch = sizeof(D3DXCOLOR) * 1024;
	d3dSubResourceData.SysMemSlicePitch = 0;

	D3D11_TEXTURE1D_DESC d3dTextureDesc;
	ZeroMemory(&d3dTextureDesc, sizeof(D3D11_TEXTURE1D_DESC));
	d3dTextureDesc.Width = 1024;
	d3dTextureDesc.MipLevels = 1;
	d3dTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	d3dTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	d3dTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTextureDesc.ArraySize = 1;
	ID3D11Texture1D *pd3dTexture;
	pd3dDevice->CreateTexture1D(&d3dTextureDesc, &d3dSubResourceData, &pd3dTexture);
	ID3D11ShaderResourceView *pd3dsrvTexture;
	pd3dDevice->CreateShaderResourceView(pd3dTexture, NULL, &pd3dsrvTexture);
	DXUT_SetDebugName(pd3dsrvTexture, "Random1DTexture");
	pd3dTexture->Release();

	return(pd3dsrvTexture);
}

void CParticleSystem::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CParticleVertex);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	CParticleVertex particle;
	ZeroMemory(&particle, sizeof(CParticleVertex));
	D3D11_SUBRESOURCE_DATA d3dSubResourceData;
	d3dSubResourceData.pSysMem = &particle;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dSubResourceData, &m_pd3dInitialVertexBuffer);
	DXUT_SetDebugName(m_pd3dInitialVertexBuffer, "InitialVertex");

	m_nStride = sizeof(CParticleVertex);
	d3dBufferDesc.ByteWidth = sizeof(CParticleVertex) * m_nMaxParticles;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dStreamOutVertexBuffer);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dDrawVertexBuffer);
	DXUT_SetDebugName(m_pd3dStreamOutVertexBuffer, "StreamOutVertex");
	DXUT_SetDebugName(m_pd3dDrawVertexBuffer, "DrawVertex");

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(CB_PARTICLEINFO) + sizeof(float);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbParticleInfo);
	DXUT_SetDebugName(m_pd3dcbParticleInfo, "ParticleInfo");
}

void CParticleSystem::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbParticleInfo, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

	CB_PARTICLEINFO *pcbParticleInfo = (CB_PARTICLEINFO*)d3dMappedResource.pData;
	pcbParticleInfo->m_fGameTime = m_fGameTime;
	pcbParticleInfo->m_fTimeStep = m_fTimeStep;
	pcbParticleInfo->m_d3dxvAcceleration = m_d3dxvAcceleration;
	pcbParticleInfo->m_d3dxvEmitDirection = m_d3dxvEmitDirection;
	pcbParticleInfo->m_d3dxvEmitPosition = m_d3dxvEmitPosition;

	pd3dDeviceContext->Unmap(m_pd3dcbParticleInfo, 0);

	pd3dDeviceContext->GSSetConstantBuffers(GS_CB_SLOT_PARTICLE, 1, &m_pd3dcbParticleInfo);
}

void CParticleSystem::CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, const wstring& pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;
	
	// UINT Stream, LPCSTR SemanticName, BYTE SemanticIndex, BYTE StartComponent, BYTE ComponentCount, BYTE OuputSlot
	// 
	D3D11_SO_DECLARATION_ENTRY pSODecls[] = {
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "VELOCITY", 0, 0, 3, 0 },
		{ 0, "SIZE", 0, 0, 2, 0 },
		{ 0, "TYPE", 0, 0, 1, 0 },
		{ 0, "AGE", 0, 0, 1, 0 }
	};
	UINT pBufferStrides[1] = { sizeof(CParticleVertex) };//개수 많이 지정해야되지않나

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreateGeometryShaderWithStreamOutput(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(),
			pSODecls,			// 스트림아웃 디클러레이션
			5,					// 엔트리 개수
			pBufferStrides,		// 버퍼 스트라이드
			1,					// 스트라이드 개수
			0,					// 래스터라이즈 스트림
			NULL, ppd3dGeometryShader);

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
	for (UINT i = 0; i < nTextures; i++) D3DX11CreateTextureFromFile(pd3dDevice, ppstrFilePaths[i], &d3dxImageLoadInfo, 0, (ID3D11Resource **)&ppd3dTextures[i], 0);

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