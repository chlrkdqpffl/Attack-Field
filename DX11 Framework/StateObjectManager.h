#pragma once
#include "SingletonManager.h"

class CStateObjectManager :	public CSingletonManager<CStateObjectManager>
{
public:
	CStateObjectManager();
	virtual ~CStateObjectManager();

	virtual void InitializeManager();
	
	ComPtr<ID3D11Device>			m_pd3dDevice;
	ComPtr<ID3D11DeviceContext>		m_pd3dImmediateDeviceContext;
};

