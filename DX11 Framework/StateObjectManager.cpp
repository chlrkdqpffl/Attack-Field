#include "stdafx.h"
#include "StateObjectManager.h"


ID3D11RasterizerState* CStateObjectManager::m_pDefaultRS			= 0;
ID3D11RasterizerState* CStateObjectManager::m_pWireframeRS			= 0;
ID3D11RasterizerState* CStateObjectManager::m_pNoCullRS				= 0;

ID3D11BlendState*      CStateObjectManager::m_pAlphaToCoverageBS	= 0;
ID3D11BlendState*      CStateObjectManager::m_pTransparentBS		= 0;

CStateObjectManager::CStateObjectManager()
{
	m_pd3dDevice = nullptr;
	m_pd3dImmediateDeviceContext = nullptr;
}


CStateObjectManager::~CStateObjectManager()
{
	ReleaseCOM(m_pDefaultRS);
	ReleaseCOM(m_pWireframeRS);
	ReleaseCOM(m_pNoCullRS);
	ReleaseCOM(m_pAlphaToCoverageBS);
	ReleaseCOM(m_pTransparentBS);
}

void CStateObjectManager::InitializeManager()
{
	// Default RS
	D3D11_RASTERIZER_DESC defaultDesc;
	ZeroMemory(&defaultDesc, sizeof(D3D11_RASTERIZER_DESC));
	defaultDesc.FillMode = D3D11_FILL_SOLID;
	defaultDesc.CullMode = D3D11_CULL_BACK;
	defaultDesc.FrontCounterClockwise = false;
	defaultDesc.DepthClipEnable = true;

	HR(m_pd3dDevice->CreateRasterizerState(&defaultDesc, &m_pDefaultRS));

	// WireframeRS
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(m_pd3dDevice->CreateRasterizerState(&wireframeDesc, &m_pWireframeRS));

	// NoCullRS
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(m_pd3dDevice->CreateRasterizerState(&noCullDesc, &m_pNoCullRS));

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
}