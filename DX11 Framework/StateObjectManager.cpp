#include "stdafx.h"
#include "StateObjectManager.h"

ID3D11Device*			CStateObjectManager::g_pd3dDevice					= nullptr;
ID3D11DeviceContext*	CStateObjectManager::g_pd3dImmediateDeviceContext	= nullptr;

ID3D11RasterizerState*	CStateObjectManager::g_pDefaultRS			= nullptr;
ID3D11RasterizerState*	CStateObjectManager::g_pWireframeRS			= nullptr;
ID3D11RasterizerState*	CStateObjectManager::g_pNoCullRS			= nullptr;

ID3D11BlendState*		CStateObjectManager::g_pAlphaToCoverageBS	= nullptr;
ID3D11BlendState*		CStateObjectManager::g_pTransparentBS		= nullptr;
ID3D11BlendState*		CStateObjectManager::g_pAddativeBS			= nullptr;
ID3D11BlendState*		CStateObjectManager::g_pDecalBS				= nullptr;
ID3D11BlendState*		CStateObjectManager::g_pBloodBS				= nullptr;
ID3D11BlendState*		CStateObjectManager::g_pFireBS				= nullptr;

ID3D11SamplerState*		CStateObjectManager::g_pLinearClampSS		= nullptr;
ID3D11SamplerState*		CStateObjectManager::g_pPointClampSS		= nullptr;
ID3D11SamplerState*		CStateObjectManager::g_pLinearWrapSS		= nullptr;
ID3D11SamplerState*		CStateObjectManager::g_pPointWrapSS			= nullptr;
ID3D11SamplerState*		CStateObjectManager::g_pAnisotropicClampSS	= nullptr;
ID3D11SamplerState*		CStateObjectManager::g_pAnisotropicWrapSS	= nullptr;

ID3D11DepthStencilState* CStateObjectManager::g_pNoDepthWritesDSS		= nullptr;
ID3D11DepthStencilState* CStateObjectManager::g_pNoDepthEqualWritesDSS	= nullptr;
ID3D11DepthStencilState* CStateObjectManager::g_pDisableDepthDSS		= nullptr;
ID3D11DepthStencilState* CStateObjectManager::g_pGBufferDSS				= nullptr;
ID3D11DepthStencilState* CStateObjectManager::g_pDefaultDSS				= nullptr;

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

	for (int i = 0; i < 8; ++i) {
		transparentDesc.RenderTarget[i].BlendEnable = true;
		transparentDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		transparentDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		//transparentDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
		transparentDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		transparentDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		transparentDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		transparentDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		transparentDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	HR(g_pd3dDevice->CreateBlendState(&transparentDesc, &g_pAddativeBS));
	DXUT_SetDebugName(g_pAddativeBS, "Addative BS:");


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
	HR(g_pd3dDevice->CreateBlendState(&descBlend, &g_pDecalBS));
	DXUT_SetDebugName(g_pDecalBS, "Decal Alpha Blending BS");


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
	HR(g_pd3dDevice->CreateBlendState(&d3dBlendStateDesc, &g_pBloodBS));
	DXUT_SetDebugName(g_pBloodBS, "Blood BS:");

	d3dBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	HR(g_pd3dDevice->CreateBlendState(&d3dBlendStateDesc, &g_pFireBS));
	DXUT_SetDebugName(g_pFireBS, "Fire BS:");

	// ---------------------------------------------------------------------------- //
	// ------------------------------ Sampler State ------------------------------- //
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = -FLT_MAX;
	d3dSamplerDesc.MaxLOD = FLT_MAX;
	d3dSamplerDesc.MaxAnisotropy = 16;

	HR(g_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &g_pLinearClampSS));
	DXUT_SetDebugName(g_pLinearClampSS, "Linear Clamp SS:");

	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	HR(g_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &g_pPointClampSS));
	DXUT_SetDebugName(g_pPointClampSS, "Point Clamp SS:");

	d3dSamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	HR(g_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &g_pAnisotropicClampSS));
	DXUT_SetDebugName(g_pAnisotropicClampSS, "Anisotropic Clamp SS:");
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	HR(g_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &g_pLinearWrapSS));
	DXUT_SetDebugName(g_pLinearWrapSS, "Linear Wrap SS:");

	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	HR(g_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &g_pPointWrapSS));
	DXUT_SetDebugName(g_pPointWrapSS, "Point Wrap SS:");

	d3dSamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	HR(g_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &g_pAnisotropicWrapSS));
	DXUT_SetDebugName(g_pAnisotropicWrapSS, "Anisotropic Wrap SS:");
	// ---------------------------------------------------------------------------------- //
	// ------------------------------ Depth Stencil State ------------------------------- //
	D3D11_DEPTH_STENCIL_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDesc.StencilEnable = false;
	HR(g_pd3dDevice->CreateDepthStencilState(&depthDesc, &g_pDefaultDSS));
	DXUT_SetDebugName(g_pDefaultDSS, "Default DSS");

	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;		 	
	HR(g_pd3dDevice->CreateDepthStencilState(&depthDesc, &g_pNoDepthWritesDSS));
	DXUT_SetDebugName(g_pNoDepthWritesDSS, "No Depth Write DSS");

	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	HR(g_pd3dDevice->CreateDepthStencilState(&depthDesc, &g_pNoDepthEqualWritesDSS));
	DXUT_SetDebugName(g_pNoDepthEqualWritesDSS, "No Depth Equal Write DSS");

	depthDesc.DepthEnable = false;
	HR(g_pd3dDevice->CreateDepthStencilState(&depthDesc, &g_pDisableDepthDSS));
	DXUT_SetDebugName(g_pDisableDepthDSS, "Disable Depth DSS");


	// ================================= Create Depth Stencil State ===================================== //
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = TRUE;
	descDepth.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	descDepth.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC stencilMarkOp = { D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS };
	descDepth.FrontFace = stencilMarkOp;
	descDepth.BackFace = stencilMarkOp;
	HR(g_pd3dDevice->CreateDepthStencilState(&descDepth, &g_pGBufferDSS));
	DXUT_SetDebugName(g_pGBufferDSS, "GBuffer - Depth Stencil Mark DS");
}

void CStateObjectManager::ReleseManager()
{
	ReleaseCOM(g_pDefaultRS);
	ReleaseCOM(g_pWireframeRS);
	ReleaseCOM(g_pNoCullRS);

	ReleaseCOM(g_pAlphaToCoverageBS);
	ReleaseCOM(g_pTransparentBS);
	ReleaseCOM(g_pAddativeBS);
	ReleaseCOM(g_pDecalBS);
	ReleaseCOM(g_pBloodBS);
	ReleaseCOM(g_pFireBS);

	ReleaseCOM(g_pLinearClampSS);
	ReleaseCOM(g_pPointClampSS);
	ReleaseCOM(g_pLinearWrapSS);
	ReleaseCOM(g_pPointWrapSS);
	ReleaseCOM(g_pAnisotropicClampSS);
	ReleaseCOM(g_pAnisotropicWrapSS);

	ReleaseCOM(g_pNoDepthEqualWritesDSS);
	ReleaseCOM(g_pNoDepthWritesDSS);
	ReleaseCOM(g_pDisableDepthDSS);
	ReleaseCOM(g_pGBufferDSS);
	ReleaseCOM(g_pDefaultDSS);
}