#include "stdafx.h"
#include "DecalManager.h"

const UINT CDecalManager::m_nVertStride = 8 * sizeof(float);
const UINT CDecalManager::m_nMaxDecalVerts = 256;
/*
void CDecalManager::InitializeManager()
{
	ID3D11Device* pd3dDevice = STATEOBJ_MGR->g_pd3dDevice;

	D3D11_BUFFER_DESC CBDesc;
	ZeroMemory(&CBDesc, sizeof(CBDesc));
	CBDesc.Usage = D3D11_USAGE_DYNAMIC;
	CBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CBDesc.ByteWidth = sizeof(D3DXMATRIX);
	HR(pd3dDevice->CreateBuffer(&CBDesc, NULL, &m_pDecalGenVSCB));
	DXUT_SetDebugName(m_pDecalGenVSCB, "Generate Decal VS CB");

	CBDesc.ByteWidth = sizeof(TDecalGenCB);
	HR(pd3dDevice->CreateBuffer(&CBDesc, NULL, &m_pDecalGenGSCB));
	DXUT_SetDebugName(m_pDecalGenGSCB, "Generate Decal GS CB");

	
	// --------------------- Create Shader -------------------------- //
	wstring strFileName;
	// Directional Light Shader
	strFileName = L"Shader HLSL File/DecalGen.hlsli";
	CShader::CreateVertexShaderFromFile(pd3dDevice, strFileName, "DecalGenVS", "vs_5_0", &m_pDecalGenVS);
	CShader::CreateGeometryShaderFromFile(pd3dDevice, strFileName, "DecalGenGS", "gs_5_0", &m_pDecalGenGS);


	D3D11_SO_DECLARATION_ENTRY pStreamOutDecl[] =
	{
		// stream, name, index, start component, component count, output slot
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "NORMAL", 0, 0, 3, 0 },
		{ 0,"TEXCOORD", 0, 0, 2, 0 },
	};
	UINT arrBufferStride[1] = { m_nVertStride };
	V_RETURN(g_pDevice->CreateGeometryShaderWithStreamOutput(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pStreamOutDecl, 3, arrBufferStride, 1, 0, NULL, &m_pDecalGenGS));
	DXUT_SetDebugName(m_pDecalGenGS, "Generate Decal GS");
	SAFE_RELEASE(pShaderBlob);

	CShader::crea
	strFileName = L"Shader HLSL File/DeferredShading.hlsli";
	CShader::CreateVertexShaderFromFile(pd3dDevice, strFileName, "RenderSceneVS", "vs_5_0", &m_pDecalRenderVS);
	CShader::CreatePixelShaderFromFile(pd3dDevice, strFileName, "RenderDecalPS", "ps_5_0", &m_pDecalRenderPS);

	// Decal generation depth/stencil states
	D3D11_DEPTH_STENCIL_DESC DSDesc;
	ZeroMemory(&DSDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DSDesc.DepthEnable = false;
	DSDesc.StencilEnable = false;
	V_RETURN(g_pDevice->CreateDepthStencilState(&DSDesc, &m_pDecalGenDepthStencilState));
	DXUT_SetDebugName(m_pDecalGenDepthStencilState, "Generate Decal DSS");

	// Create the VB for the decals
	static const UINT nDecalBufferSize = m_nMaxDecalVerts * m_nVertStride;
	D3D11_BUFFER_DESC VBDesc =
	{
		nDecalBufferSize,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT,
		0,
		0
	};
	g_pDevice->CreateBuffer(&VBDesc, NULL, &m_pDecalVB);

	// Create the querys in order to keep track of what got drawn so far
	D3D11_QUERY_DESC StatsDesc;
	StatsDesc.Query = D3D11_QUERY_SO_STATISTICS;
	StatsDesc.MiscFlags = 0;
	V_RETURN(g_pDevice->CreateQuery(&StatsDesc, &m_pStatsQuery));

	// Load the decal texture
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"..\\Media\\Decal.dds"));
	V_RETURN(D3DX11CreateShaderResourceViewFromFile(g_pDevice, str, NULL, NULL, &m_pDecalTexView, NULL));

	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"..\\Media\\White.dds"));
	V_RETURN(D3DX11CreateShaderResourceViewFromFile(g_pDevice, str, NULL, NULL, &m_pWhiteTexView, NULL));

	D3D11_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	const D3D11_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
	{
		TRUE,
		D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
		D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE,
	};
	for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		descBlend.RenderTarget[i] = defaultRenderTargetBlendDesc;
	V_RETURN(g_pDevice->CreateBlendState(&descBlend, &m_pDecalRenderBS));
	DXUT_SetDebugName(m_pDecalRenderBS, "Decal Alpha Blending BS");

	D3D11_RASTERIZER_DESC descRast = {
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK,
		FALSE,
		-85,
		D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		-0.75f,
		FALSE,
		FALSE,
		FALSE,
		FALSE
	};
	V_RETURN(g_pDevice->CreateRasterizerState(&descRast, &m_pRSDecalSolid));
	DXUT_SetDebugName(m_pDecalRenderBS, "Decal RS");

	descRast.FillMode = D3D11_FILL_WIREFRAME;
	V_RETURN(g_pDevice->CreateRasterizerState(&descRast, &m_pRSDecalWire));
	DXUT_SetDebugName(m_pDecalRenderBS, "Decal Wire RS");
}

void CDecalManager::ReleseManager()
{
	ReleaseCOM(m_pDecalGenVSCB);
	ReleaseCOM(m_pDecalGenGSCB);
	ReleaseCOM(m_pDecalGenVS);
	ReleaseCOM(m_pDecalGenGS);
	ReleaseCOM(m_pStatsQuery);
	ReleaseCOM(m_pDecalVB);
	ReleaseCOM(m_pDecalRenderCB);
	ReleaseCOM(m_pDecalRenderVS);
	ReleaseCOM(m_pDecalRenderPS);
	ReleaseCOM(m_pDecalTexView);
	ReleaseCOM(m_pWhiteTexView);
//	ReleaseCOM(m_pDecalRenderBS);
//	ReleaseCOM(m_pRSDecalSolid);
//	ReleaseCOM(m_pRSDecalWire);
}

void CDecalManager::CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, const wstring& pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
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
			pSODecls, 5, pBufferStrides, 1, 0, NULL, ppd3dGeometryShader);

		pd3dShaderBlob->Release();

		DXUT_SetDebugName(*ppd3dGeometryShader, "ParticleGSWithSO");
	}
	else
	{
		MessageBox(NULL, L"SOGSERROR", L"SOGSERROR", MB_OK);
		exit(0);
	}
}

void CDecalManager::UpdateManager()
{

}
*/