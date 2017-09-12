#pragma once
#include "SingletonManager.h"

class CStateObjectManager :	public CSingletonManager<CStateObjectManager>
{
public:
	CStateObjectManager();
	virtual ~CStateObjectManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	static ID3D11Device*			g_pd3dDevice;
	static ID3D11DeviceContext*		g_pd3dImmediateDeviceContext;

	static ID3D11RasterizerState*	g_pDefaultRS;
	static ID3D11RasterizerState*	g_pWireframeRS;
	static ID3D11RasterizerState*	g_pNoCullRS;

	static ID3D11BlendState*		g_pAlphaToCoverageBS;
	static ID3D11BlendState*		g_pTransparentBS;
	static ID3D11BlendState*		g_pAddativeBS;
	static ID3D11BlendState*		g_pFireBS;
	static ID3D11BlendState*		g_pBloodBS;

	static ID3D11SamplerState*		g_pLinearClampSS;
	static ID3D11SamplerState*		g_pPointClampSS;
	static ID3D11SamplerState*		g_pPointWarpSS;
	static ID3D11SamplerState*		g_pLinearWarpSS;

	static ID3D11DepthStencilState* g_pNoDepthWritesDSS;
	static ID3D11DepthStencilState* g_pNoDepthEqualWritesDSS;
	static ID3D11DepthStencilState* g_pDisableDepthDSS;
	static ID3D11DepthStencilState* g_pDefaultDSS;
	static ID3D11DepthStencilState* g_pGBufferDSS;
};