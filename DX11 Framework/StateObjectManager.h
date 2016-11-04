#pragma once
#include "SingletonManager.h"

class CStateObjectManager :	public CSingletonManager<CStateObjectManager>
{
public:
	CStateObjectManager();
	virtual ~CStateObjectManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;
	
	ComPtr<ID3D11Device>			m_pd3dDevice;
	ComPtr<ID3D11DeviceContext>		m_pd3dImmediateDeviceContext;

	static ID3D11RasterizerState*	m_pDefaultRS;
	static ID3D11RasterizerState*	m_pWireframeRS;
	static ID3D11RasterizerState*	m_pNoCullRS;

	static ID3D11BlendState*		m_pAlphaToCoverageBS;
	static ID3D11BlendState*		m_pTransparentBS;

	static ID3D11SamplerState*		m_pLinearClampSS;
	static ID3D11SamplerState*		m_pPointClampSS;
	static ID3D11SamplerState*		m_pPointWarpSS;
	static ID3D11SamplerState*		m_pLinearWarpSS;
	
};

