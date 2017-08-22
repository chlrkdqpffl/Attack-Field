#pragma once
#include "SingletonManager.h"
#include "ParticleSystem.h"

class CParticleManager : public CSingletonManager<CParticleManager>
{
public:
	/*
	const UINT m_nMaxCopiousBleedingParticle =	3;
//	vector<CParticleSystem*>			m_vecCopiousBleedingParticleSystemPool;

	const UINT m_nMaxBloodParticle		= 5;
//	vector<CParticleSystem*>			m_vecBloodParticleSystemPool;

	const UINT m_nMaxSparkParticle		= 10;
//	vector<CParticleSystem*>			m_vecSparkParticleSystemPool;

	CParticleSystem*					m_pRainParticle = nullptr;
//	vector<CParticleSystem*>			m_vecParticleSystemContainer;

	multimap<ParticleTag, ID3D11ShaderResourceView*>	m_mapTexturePool;
	multimap<ParticleTag, vector<CParticleSystem*>>		m_mapParticlePool;
	*/

	const UINT m_nMaxCopiousBleedingParticle = 3;
	vector<CParticleSystem*>			m_vecCopiousBleedingParticleSystemPool;

	const UINT m_nMaxBloodParticle = 5;
	vector<CParticleSystem*>			m_vecBloodParticleSystemPool;

	const UINT m_nMaxSparkParticle = 10;
	vector<CParticleSystem*>			m_vecSparkParticleSystemPool;

	CParticleSystem*					m_pRainParticle = nullptr;
	vector<CParticleSystem*>			m_vecParticleSystemContainer;

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
	void CreateBlood(XMVECTOR pos);
	void CreateCopiousBleeding(XMVECTOR pos);
	void CreateSpark(XMVECTOR pos);


private:
	void AddParticleTexture(const ParticleTag& tag, const string& source);
	ID3D11ShaderResourceView* CloneShaderResourceView(const ParticleTag& tag);
};