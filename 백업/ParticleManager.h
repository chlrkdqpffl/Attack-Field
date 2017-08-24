#pragma once
#include "SingletonManager.h"
#include "ParticleSystem.h"

class CParticleManager : public CSingletonManager<CParticleManager> 
{
	const UINT m_nMaxBloodParticle		= 5;
	const UINT m_nMaxSparkParticle		= 10;
	CParticleSystem*					m_pRainParticle = nullptr;

	multimap<ParticleTag, ID3D11ShaderResourceView*>	m_mapTexturePool;
	multimap<ParticleTag, vector<CParticleSystem*>>		m_mapParticlePool;

public:
	CParticleManager() {};
	virtual ~CParticleManager() {};

	virtual void InitializeManager() override;
	virtual void UpdateManager(float fDeltaTime);
	virtual void ReleseManager() override;

	void CreateParticleSystems(ID3D11Device *pd3dDevice);
	void RenderAllNoEffect(ID3D11DeviceContext *pd3dDeviceContext);
	void RenderAllEffect(ID3D11DeviceContext *pd3dDeviceContext);

	void CreateParticle(ParticleTag tag, XMVECTOR pos);

private:
	void Update_Rain(float fDeltaTime);
};