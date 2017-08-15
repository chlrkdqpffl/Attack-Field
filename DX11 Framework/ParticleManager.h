#pragma once
#include "SingletonManager.h"
#include "ParticleSystem.h"

class CParticleManager : public CSingletonManager<CParticleManager>
{
public:

	CParticleSystem*					m_pRainParticle = nullptr;
	vector<CParticleSystem*>			m_vecParticleSystemContainer;


public:
	CParticleManager() {};
	virtual ~CParticleManager() {};

	virtual void InitializeManager() override;
	virtual void UpdateManager(float fDeltaTime);
	virtual void ReleseManager() override;

	void CreateParticleSystems(ID3D11Device *pd3dDevice);
	void RenderAll(ID3D11DeviceContext *pd3dDeviceContext);
};