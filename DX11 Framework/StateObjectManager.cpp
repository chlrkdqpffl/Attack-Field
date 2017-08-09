#include "stdafx.h"
#include "StateObjectManager.h"

ID3D11Device*			CStateObjectManager::g_pd3dDevice					= 0;
ID3D11DeviceContext*	CStateObjectManager::g_pd3dImmediateDeviceContext	= 0;

ID3D11RasterizerState*	CStateObjectManager::g_pDefaultRS			= 0;
ID3D11RasterizerState*	CStateObjectManager::g_pWireframeRS			= 0;
ID3D11RasterizerState*	CStateObjectManager::g_pNoCullRS			= 0;

ID3D11BlendState*		CStateObjectManager::g_pAlphaToCoverageBS	= 0;
ID3D11BlendState*		CStateObjectManager::g_pTransparentBS		= 0;

ID3D11SamplerState*		CStateObjectManager::g_pLinearClampSS		= 0;
ID3D11SamplerState*		CStateObjectManager::g_pPointClampSS		= 0;
ID3D11SamplerState*		CStateObjectManager::g_pLinearWarpSS		= 0;
ID3D11SamplerState*		CStateObjectManager::g_pPointWarpSS			= 0;


CStateObjectManager::CStateObjectManager()
{
}

CStateObjectManager::~CStateObjectManager()
{
}

void CStateObjectManager::InitializeManager()
{
	// ---------------------------------------------------------------------------- //
	// --------------------------- Rasterizer State ------------------------------- //
	// Default RS
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	HR(g_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &g_pDefaultRS));
	DXUT_SetDebugName(g_pDefaultRS, "DefaultRS:");

	// WireframeRS
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

	HR(g_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &g_pWireframeRS));
	DXUT_SetDebugName(g_pWireframeRS, "WireFrameRS:");
	// NoCullRS
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;

	HR(g_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &g_pNoCullRS));
	DXUT_SetDebugName(g_pNoCullRS, "NoCullRS:");
	// ---------------------------------------------------------------------------- //
	// ------------------------------- Blend State -------------------------------- //
	// AlphaToCoverageBS
	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(g_pd3dDevice->CreateBlendState(&alphaToCoverageDesc, &g_pAlphaToCoverageBS));
	DXUT_SetDebugName(g_pAlphaToCoverageBS, "AlphaToCoverageBS:");

	// TransparentBS
	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(g_pd3dDevice->CreateBlendState(&transparentDesc, &g_pTransparentBS));
	DXUT_SetDebugName(g_pTransparentBS, "TransparentBS:");

	// ---------------------------------------------------------------------------- //
	// ------------------------------ Sampler State ------------------------------- //
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	HR(g_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &g_pLinearClampSS));
	DXUT_SetDebugName(g_pLinearClampSS, "LinearClampSS:");

	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	HR(g_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &g_pPointClampSS));
	DXUT_SetDebugName(g_pPointClampSS, "PointClampSS:");

	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HR(g_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &g_pPointWarpSS));
	DXUT_SetDebugName(g_pPointWarpSS, "PointWarpSS:");

	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	HR(g_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &g_pLinearWarpSS));
	DXUT_SetDebugName(g_pLinearWarpSS, "LinearWarpSS:");
}

void CStateObjectManager::ReleseManager()
{
	ReleaseCOM(g_pDefaultRS);
	ReleaseCOM(g_pWireframeRS);
	ReleaseCOM(g_pNoCullRS);

	ReleaseCOM(g_pAlphaToCoverageBS);
	ReleaseCOM(g_pTransparentBS);

	ReleaseCOM(g_pLinearClampSS);
	ReleaseCOM(g_pPointClampSS);
	ReleaseCOM(g_pLinearWarpSS);
	ReleaseCOM(g_pPointWarpSS);
}