#include "stdafx.h"
#include "ParticleManager.h"

void CParticleManager::InitializeManager()
{
}

void CParticleManager::ReleseManager()
{
	SafeDelete(m_pRainParticle);
	for (auto& system : m_vecParticleSystemContainer)
		SafeDelete(system);

	for (auto& system : m_vecBloodParticleSystemPool)
		SafeDelete(system);
}

void CParticleManager::CreateParticleSystems(ID3D11Device *pd3dDevice)
{
	ID3D11ShaderResourceView* pParticleTexture = nullptr;
	vector<MapData> vecMapData;
	CParticleSystem* pFireParticle = nullptr;

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eFireBarrel);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pFireParticle = new CParticleSystem();
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Particle/flare0.dds"), NULL, NULL, &pParticleTexture, NULL);
		pFireParticle->Initialize(pd3dDevice, pParticleTexture, pFireParticle->CreateRandomTexture1DSRV(pd3dDevice), 500, STATEOBJ_MGR->g_pFireBS);
		pFireParticle->CreateShader(pd3dDevice, L"Shader HLSL File/Fire.hlsli");

		XMFLOAT3 offsetPos = vecMapData[count].m_Position;
		offsetPos.y += 1.5f;
		offsetPos.z -= 0.5f;
		pFireParticle->SetEmitPosition(offsetPos);

		m_vecParticleSystemContainer.push_back(pFireParticle);
	}

	// Blood
	for (int i = 0; i < m_nMaxBloodParticle; ++i) {
		CParticleSystem* pBloodParticle = new CParticleSystem();
		D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Particle/blood.dds"), NULL, NULL, &pParticleTexture, NULL);
		pBloodParticle->Initialize(pd3dDevice, pParticleTexture, pBloodParticle->CreateRandomTexture1DSRV(pd3dDevice), 5, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pBloodParticle->CreateShader(pd3dDevice, L"Shader HLSL File/Blood.hlsli");
		pBloodParticle->SetActive(false);
		m_vecBloodParticleSystemPool.push_back(pBloodParticle);
	}

	// Rain
	m_pRainParticle = new CParticleSystem();
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Particle/raindrop.dds"), NULL, NULL, &pParticleTexture, NULL);
	m_pRainParticle->Initialize(pd3dDevice, pParticleTexture, m_pRainParticle->CreateRandomTexture1DSRV(pd3dDevice), 5000, STATEOBJ_MGR->g_pFireBS);
	m_pRainParticle->CreateShader(pd3dDevice, L"Shader HLSL File/Rain.hlsli");

}

void CParticleManager::CreateBlood(XMVECTOR pos)
{
	CParticleSystem* pBloodParticle = nullptr;

	for (auto& blood : m_vecBloodParticleSystemPool) {
		if (blood->GetActive())
			continue;
		else
			pBloodParticle = blood;
	}

	pBloodParticle->ParticleRestart();
	pBloodParticle->SetEmitvPosition(pos);
}

void CParticleManager::UpdateManager(float fDeltaTime)
{
	XMVECTOR rainOffset = SCENE_MGR->g_pCamera->GetvPosition();
	rainOffset += SCENE_MGR->g_pCamera->GetvUp() * 20;
	rainOffset += SCENE_MGR->g_pCamera->GetvLook() * 20;

	m_pRainParticle->SetEmitvPosition(rainOffset);
	m_pRainParticle->Update(fDeltaTime);

	for (auto& system : m_vecBloodParticleSystemPool)
		if (system->GetActive())
			system->Update(fDeltaTime);

	for (auto& system : m_vecParticleSystemContainer)
		if (system->GetActive())
			system->Update(fDeltaTime);
}

void CParticleManager::RenderAllNoEffect(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_pRainParticle->Render(pd3dDeviceContext);

	for (auto& system : m_vecBloodParticleSystemPool)
		if (system->GetActive())
			system->Render(pd3dDeviceContext);
}

void CParticleManager::RenderAllEffect(ID3D11DeviceContext *pd3dDeviceContext)
{
	for (auto& system : m_vecParticleSystemContainer)
		if (system->GetActive())
			system->Render(pd3dDeviceContext);
}