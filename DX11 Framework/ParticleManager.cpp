#include "stdafx.h"
#include "ParticleManager.h"

void CParticleManager::InitializeManager()
{
	cout << endl << " ----- Particle Texture Data -----" << endl;

	RESOURCE_MGR->AddResourece(TextureTag::eRain, "../Assets/Image/Particle/raindrop.dds");
	RESOURCE_MGR->AddResourece(TextureTag::eFire, "../Assets/Image/Particle/flare0.dds");
	RESOURCE_MGR->AddResourece(TextureTag::eSpark, "../Assets/Image/Particle/Spark.dds");
	RESOURCE_MGR->AddResourece(TextureTag::eBlood1, "../Assets/Image/Particle/blood.dds");
	RESOURCE_MGR->AddResourece(TextureTag::eBlood2, "../Assets/Image/Particle/blood2.dds");
	RESOURCE_MGR->AddResourece(TextureTag::eBlood3, "../Assets/Image/Particle/blood3.dds");
}

void CParticleManager::ReleseManager()
{
	SafeDelete(m_pRainParticle);

	for (auto& index = m_mapParticlePool.begin(); index != m_mapParticlePool.end(); ++index)
		for(auto& system : index->second)
			SafeDelete(system);

	m_mapParticlePool.clear();
}

void CParticleManager::CreateParticleSystems(ID3D11Device *pd3dDevice)
{
	// Particle Texture 는 Resource Manager에 저장된 텍스쳐를 가져오고 
	// 원하는 파티클의 형태마다 Particle Tag를 생성하여 해당 map에 넣어준다.
	
	vector<MapData> vecMapData;
	CParticleSystem* pParticle = nullptr;
	vector<CParticleSystem*> vecParticleSystemPool;
	wstring wstrShaderName;

	// ========================== Fire ================================= //
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eFireBarrel);
	wstrShaderName = L"Shader HLSL File/Fire.hlsli";
	for (int count = 0; count < vecMapData.size(); ++count) {
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, RESOURCE_MGR->CloneShaderResourceView(TextureTag::eFire), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 500, STATEOBJ_MGR->g_pFireBS);
		pParticle->CreateShader(pd3dDevice, wstrShaderName);

		XMFLOAT3 offsetPos = vecMapData[count].m_Position;
		offsetPos.y += 1.1f;
		offsetPos.z -= 0.7f;
		pParticle->SetEmitPosition(offsetPos);

		vecParticleSystemPool.push_back(pParticle);
	}
	m_mapParticlePool.insert(make_pair(ParticleTag::eFire, vecParticleSystemPool));
	vecParticleSystemPool.clear();

	// Spark
	wstrShaderName = L"Shader HLSL File/FireSpark.hlsli";
	for (int count = 0; count < vecMapData.size(); ++count) {
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, RESOURCE_MGR->CloneShaderResourceView(TextureTag::eSpark), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 500, STATEOBJ_MGR->g_pFireBS);
		pParticle->CreateShader(pd3dDevice, wstrShaderName);

		XMFLOAT3 offsetPos = vecMapData[count].m_Position;
		offsetPos.y += 1.5f;
		offsetPos.z -= 0.5f;
		pParticle->SetEmitPosition(offsetPos);

		vecParticleSystemPool.push_back(pParticle);
	}
	m_mapParticlePool.insert(make_pair(ParticleTag::eFireSpark, vecParticleSystemPool));
	vecParticleSystemPool.clear();

	// ========================== Blood ================================= //
	wstrShaderName = L"Shader HLSL File/Blood.hlsli";
	for (int i = 0; i < m_nMaxBloodParticle; ++i) {
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, RESOURCE_MGR->CloneShaderResourceView(TextureTag::eBlood1), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 5, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, wstrShaderName);
		pParticle->SetActive(false);
		vecParticleSystemPool.push_back(pParticle);

		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, RESOURCE_MGR->CloneShaderResourceView(TextureTag::eBlood2), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 5, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, wstrShaderName);
		pParticle->SetActive(false);
		vecParticleSystemPool.push_back(pParticle);

		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, RESOURCE_MGR->CloneShaderResourceView(TextureTag::eBlood3), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 5, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, wstrShaderName);
		pParticle->SetActive(false);
		vecParticleSystemPool.push_back(pParticle);
	}
	m_mapParticlePool.insert(make_pair(ParticleTag::eBleeding, vecParticleSystemPool));
	vecParticleSystemPool.clear();

	// Copious Bleeding
	wstrShaderName = L"Shader HLSL File/CopiousBleeding.hlsli";
	for (int i = 0; i < m_nMaxBloodParticle; ++i) {
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, RESOURCE_MGR->CloneShaderResourceView(TextureTag::eBlood1), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 8, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, wstrShaderName);
		pParticle->SetActive(false);
		vecParticleSystemPool.push_back(pParticle);

		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, RESOURCE_MGR->CloneShaderResourceView(TextureTag::eBlood2), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 8, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, wstrShaderName);
		pParticle->SetActive(false);
		vecParticleSystemPool.push_back(pParticle);

		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, RESOURCE_MGR->CloneShaderResourceView(TextureTag::eBlood3), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 8, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, wstrShaderName);
		pParticle->SetActive(false);
		vecParticleSystemPool.push_back(pParticle);
	}
	m_mapParticlePool.insert(make_pair(ParticleTag::eCopiousBleeding, vecParticleSystemPool));
	vecParticleSystemPool.clear();

	// ========================== Spark ================================= //
	wstrShaderName = L"Shader HLSL File/Spark.hlsli";
	for (int i = 0; i < m_nMaxSparkParticle; ++i) {
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, RESOURCE_MGR->CloneShaderResourceView(TextureTag::eSpark), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 10, STATEOBJ_MGR->g_pTransparentBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, wstrShaderName);
		pParticle->SetActive(false);
		vecParticleSystemPool.push_back(pParticle);
	}

	m_mapParticlePool.insert(make_pair(ParticleTag::eSpark, vecParticleSystemPool));
	vecParticleSystemPool.clear();

	// ========================== Rain ================================= //
	m_pRainParticle = new CParticleSystem();
	m_pRainParticle->Initialize(pd3dDevice, RESOURCE_MGR->CloneShaderResourceView(TextureTag::eRain), m_pRainParticle->CreateRandomTexture1DSRV(pd3dDevice), 10000, STATEOBJ_MGR->g_pTransparentBS);
	m_pRainParticle->CreateShader(pd3dDevice, L"Shader HLSL File/Rain.hlsli");
}

void CParticleManager::CreateParticle(ParticleTag tag, XMVECTOR pos)
{
#ifndef FASTLOAD_MODE
	auto findParticlePool = m_mapParticlePool.find(tag);
	assert(findParticlePool != m_mapParticlePool.end());

	CParticleSystem* pParticle = nullptr;
	for (auto& particleSystem : findParticlePool->second) {
		if (particleSystem->GetActive())
			continue;
		else {
			pParticle = particleSystem;
			break;
		}
	}

	if (nullptr == pParticle)
		return;

	pParticle->ParticleRestart();
	pParticle->SetEmitvPosition(pos);
#endif
}

void CParticleManager::Update_Rain(float fDeltaTime)
{
	XMVECTOR rainOffset = SCENE_MGR->g_pCamera->GetvPosition();
	rainOffset += SCENE_MGR->g_pCamera->GetvUp() * 20;
	rainOffset += SCENE_MGR->g_pCamera->GetvLook() * 20;

	m_pRainParticle->SetEmitvPosition(rainOffset);
	m_pRainParticle->Update(fDeltaTime);
}

void CParticleManager::UpdateManager(float fDeltaTime)
{
	if(m_pRainParticle)
		Update_Rain(fDeltaTime);

	for (auto& container : m_mapParticlePool)
		for (auto& system : container.second)
			if (system->GetActive())
				system->Update(fDeltaTime);
}

void CParticleManager::RenderAllNoEffect(ID3D11DeviceContext *pd3dDeviceContext)
{
	if(m_pRainParticle)
		m_pRainParticle->Render(pd3dDeviceContext);

	for (auto& container : m_mapParticlePool)
		if (static_cast<UINT>(container.first) > static_cast<UINT>(ParticleTag::MaxPostProcessingParticle)) {
			for (auto& particle : container.second)
				if (particle->GetActive())
					particle->Render(pd3dDeviceContext);
		}
}

void CParticleManager::RenderAllEffect(ID3D11DeviceContext *pd3dDeviceContext)
{
	for (auto& container : m_mapParticlePool)
		if (static_cast<UINT>(container.first) < static_cast<UINT>(ParticleTag::MaxPostProcessingParticle)) {
			for (auto& particle : container.second)
				if (particle->GetActive())
					particle->Render(pd3dDeviceContext);
		}
}