#pragma once
#include "SingletonManager.h"
#include "ParticleSystem.h"

class CParticleManager : public CSingletonManager<CParticleManager>
{
public:

	const UINT m_nMaxCopiousBleedingParticle =	3;
	vector<CParticleSystem*>			m_vecCopiousBleedingParticleSystemPool;

	const UINT m_nMaxBloodParticle		= 5;
	vector<CParticleSystem*>			m_vecBloodParticleSystemPool;

	const UINT m_nMaxSparkParticle		= 20;
	vector<CParticleSystem*>			m_vecSparkParticleSystemPool;


	CParticleSystem*					m_pRainParticle = nullptr;
	vector<CParticleSystem*>			m_vecParticleSystemContainer;

public:
	CParticleManager() {};
	virtual ~CParticleManager() {};

	virtual void InitializeManager() override;
	virtual void UpdateManager(float fDeltaTime);
	virtual void ReleseManager() override;

	void CreateParticleSystems(ID3D11Device *pd3dDevice);
	void RenderAllNoEffect(ID3D11DeviceContext *pd3dDeviceContext);
	void RenderAllEffect(ID3D11DeviceContext *pd3dDeviceContext);
	void CreateBlood(XMVECTOR pos);
	void CreateCopiousBleeding(XMVECTOR pos);
	void CreateSpark(XMVECTOR pos);
};