#include "stdafx.h"
#include "StateObjectManager.h"


ID3D11RasterizerState*	CStateObjectManager::m_pDefaultRS			= 0;
ID3D11RasterizerState*	CStateObjectManager::m_pWireframeRS			= 0;
ID3D11RasterizerState*	CStateObjectManager::m_pNoCullRS			= 0;

ID3D11BlendState*		CStateObjectManager::m_pAlphaToCoverageBS	= 0;
ID3D11BlendState*		CStateObjectManager::m_pTransparentBS		= 0;

ID3D11SamplerState*		CStateObjectManager::m_pLinearClampSS		= 0;
ID3D11SamplerState*		CStateObjectManager::m_pPointClampSS		= 0;
ID3D11SamplerState*		CStateObjectManager::m_pLinearWarpSS		= 0;
ID3D11SamplerState*		CStateObjectManager::m_pPointWarpSS			= 0;



CStateObjectManager::CStateObjectManager()
{
	m_pd3dDevice = nullptr;
	m_pd3dImmediateDeviceContext = nullptr;
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

	HR(m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &m_pDefaultRS));

	// WireframeRS
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

	HR(m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &m_pWireframeRS));

	// NoCullRS
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;

	HR(m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &m_pNoCullRS));

	// ---------------------------------------------------------------------------- //
	// ------------------------------- Blend State -------------------------------- //
	// AlphaToCoverageBS
	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(m_pd3dDevice->CreateBlendState(&alphaToCoverageDesc, &m_pAlphaToCoverageBS));

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

	HR(m_pd3dDevice->CreateBlendState(&transparentDesc, &m_pTransparentBS));


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
	HR(m_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_pLinearClampSS));

	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	HR(m_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_pPointClampSS));

	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HR(m_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_pPointWarpSS));

	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	HR(m_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_pLinearWarpSS));
	
}

void CStateObjectManager::ReleseManager()
{
	ReleaseCOM(m_pDefaultRS);
	ReleaseCOM(m_pWireframeRS);
	ReleaseCOM(m_pNoCullRS);

	ReleaseCOM(m_pAlphaToCoverageBS);
	ReleaseCOM(m_pTransparentBS);

	ReleaseCOM(m_pLinearClampSS);
	ReleaseCOM(m_pPointClampSS);
	ReleaseCOM(m_pLinearWarpSS);
	ReleaseCOM(m_pPointWarpSS);
}