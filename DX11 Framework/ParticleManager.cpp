#include "stdafx.h"
#include "ParticleManager.h"

void CParticleManager::InitializeManager()
{
	AddParticleTexture(ParticleTag::eRain,				"../Assets/Image/Particle/raindrop.dds");
	AddParticleTexture(ParticleTag::eFire,				"../Assets/Image/Particle/flare0.dds");
	AddParticleTexture(ParticleTag::eSpark,				"../Assets/Image/Particle/Spark.dds");
	AddParticleTexture(ParticleTag::eBleeding,			"../Assets/Image/Particle/blood.dds");
	AddParticleTexture(ParticleTag::eBleeding2,			"../Assets/Image/Particle/blood2.dds");
	AddParticleTexture(ParticleTag::eBleeding3,			"../Assets/Image/Particle/blood3.dds");
	AddParticleTexture(ParticleTag::eCopiousBleeding,	"../Assets/Image/Particle/raindrop.dds");
}

void CParticleManager::ReleseManager()
{
	SafeDelete(m_pRainParticle);
	for (auto& system : m_vecParticleSystemContainer)
		SafeDelete(system);

	for (auto& system : m_vecBloodParticleSystemPool)
		SafeDelete(system);

	for (auto& system : m_vecSparkParticleSystemPool)
		SafeDelete(system);

	for (auto& system : m_vecCopiousBleedingParticleSystemPool)
		SafeDelete(system);
}

void CParticleManager::AddParticleTexture(const ParticleTag& tag, const string& source)
{
	HRESULT hResult;
	ID3D11ShaderResourceView *pd3dsrvTexture = NULL;
	D3DX11CreateShaderResourceViewFromFile(STATEOBJ_MGR->g_pd3dDevice, s_to_ws(source).c_str(), NULL, NULL, &pd3dsrvTexture, &hResult);

	if ((HRESULT)hResult >= 0)
		ShowTaskSuccess("\t Success!!");
	else
		ShowTaskFail("\t Error!! \t\t 파일 또는 경로를 확인하세요.");

	string str = "Particle Tag : " + to_string(static_cast<int>(tag));
	DXUT_SetDebugName(pd3dsrvTexture, str.c_str());

	m_mapTexturePool.insert(make_pair(tag, pd3dsrvTexture));

	// 한 태그에 여러개 등록되었음
	assert(m_mapTexturePool.count(tag) <= 1);
}

ID3D11ShaderResourceView* CParticleManager::CloneShaderResourceView(const ParticleTag& tag)
{
	auto findResource = m_mapTexturePool.find(tag);

	// Pool에 해당 데이터가 존재하지 않는다.
	if (findResource == m_mapTexturePool.end())
		MessageBox(NULL, s_to_ws("Particle Texture Tag : " + to_string(static_cast<int>(tag))).c_str(), L"Resource Error", MB_OK);

	return (*findResource).second;
}

void CParticleManager::CreateParticleSystems(ID3D11Device *pd3dDevice)
{
	vector<MapData> vecMapData;
	CParticleSystem* pParticle = nullptr;
	vector<CParticleSystem*> vecParticleSystemPool;

	// ========================== Fire ================================= //
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eFireBarrel);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, CloneShaderResourceView(ParticleTag::eFire), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 500, STATEOBJ_MGR->g_pFireBS);
		pParticle->CreateShader(pd3dDevice, L"Shader HLSL File/Fire.hlsli");

		XMFLOAT3 offsetPos = vecMapData[count].m_Position;
		offsetPos.y += 1.4f;
		offsetPos.z -= 0.5f;
		pParticle->SetEmitPosition(offsetPos);

		vecParticleSystemPool.push_back(pParticle);
	}
	m_mapParticlePool.insert(make_pair( ParticleTag::eFire, vecParticleSystemPool));

	// Spark
	for (int count = 0; count < vecMapData.size(); ++count) {
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, CloneShaderResourceView(ParticleTag::eSpark), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 500, STATEOBJ_MGR->g_pFireBS);
		pParticle->CreateShader(pd3dDevice, L"Shader HLSL File/FireSpark.hlsli");

		XMFLOAT3 offsetPos = vecMapData[count].m_Position;
		offsetPos.y += 2.8f;
		offsetPos.z -= 0.5f;
		pParticle->SetEmitPosition(offsetPos);

		vecParticleSystemPool.push_back(pParticle);
	}
	m_mapParticlePool.insert(make_pair(ParticleTag::eSpark, vecParticleSystemPool));

	// ========================== Blood ================================= //
	for (int i = 0; i < m_nMaxBloodParticle; ++i) {
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, CloneShaderResourceView(ParticleTag::eBleeding), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 5, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, L"Shader HLSL File/Blood.hlsli");
		pParticle->SetActive(false);
		vecParticleSystemPool.push_back(pParticle);

		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, CloneShaderResourceView(ParticleTag::eBleeding), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 8, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, L"Shader HLSL File/CopiousBleeding.hlsli");
		pParticle->SetActive(false);
		m_vecCopiousBleedingParticleSystemPool.push_back(pParticle);

	//	어떻게하면 여러 피 텍스쳐를 동시에 사용할 지 생각해보기
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, CloneShaderResourceView(ParticleTag::eBleeding2), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 5, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, L"Shader HLSL File/Blood.hlsli");
		pParticle->SetActive(false);
		m_vecBloodParticleSystemPool.push_back(pParticle);
		
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, CloneShaderResourceView(ParticleTag::eBleeding2), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 8, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, L"Shader HLSL File/CopiousBleeding.hlsli");
		pParticle->SetActive(false);
		m_vecCopiousBleedingParticleSystemPool.push_back(pParticle);


		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, CloneShaderResourceView(ParticleTag::eBleeding3), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 5, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, L"Shader HLSL File/Blood.hlsli");
		pParticle->SetActive(false);
		m_vecBloodParticleSystemPool.push_back(pParticle);	
		
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, CloneShaderResourceView(ParticleTag::eBleeding3), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 8, STATEOBJ_MGR->g_pBloodBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, L"Shader HLSL File/CopiousBleeding.hlsli");
		pParticle->SetActive(false);
		m_vecCopiousBleedingParticleSystemPool.push_back(pParticle);
	}
	
	// ========================== Spark ================================= //
	for (int i = 0; i < m_nMaxSparkParticle; ++i) {
		pParticle = new CParticleSystem();
		pParticle->Initialize(pd3dDevice, CloneShaderResourceView(ParticleTag::eSpark), pParticle->CreateRandomTexture1DSRV(pd3dDevice), 10, STATEOBJ_MGR->g_pFireBS, 1.0f);
		pParticle->CreateShader(pd3dDevice, L"Shader HLSL File/Spark.hlsli");
		pParticle->SetActive(false);
		m_vecSparkParticleSystemPool.push_back(pParticle);
	}

	//m_mapParticlePool.insert(make_pair(tag, m_vecParticleSystemPool));

	// ========================== Rain ================================= //
	m_pRainParticle = new CParticleSystem();
	m_pRainParticle->Initialize(pd3dDevice, CloneShaderResourceView(ParticleTag::eRain), m_pRainParticle->CreateRandomTexture1DSRV(pd3dDevice), 5000, STATEOBJ_MGR->g_pFireBS);
	m_pRainParticle->CreateShader(pd3dDevice, L"Shader HLSL File/Rain.hlsli");

}

void CParticleManager::CreateBlood(XMVECTOR pos)
{
	CParticleSystem* pParticle = nullptr;

	for (auto& particle: m_vecBloodParticleSystemPool) {
		if (particle->GetActive())
			continue;
		else {
			pParticle = particle;
			break;
		}
	}

	if (!pParticle)
		return;

	pParticle->ParticleRestart();
	pParticle->SetEmitvPosition(pos);
}

void CParticleManager::CreateCopiousBleeding(XMVECTOR pos)
{
	CParticleSystem* pParticle = nullptr;
	
	for (auto& particle : m_vecCopiousBleedingParticleSystemPool) {
		if (particle->GetActive())
			continue;
		else {
			pParticle = particle;
			break;
		}
	}

	if (!pParticle)
		return;

	pParticle->ParticleRestart();
	pParticle->SetEmitvPosition(pos);
}

void CParticleManager::CreateSpark(XMVECTOR pos)
{
	CParticleSystem* pParticle = nullptr;

	for (auto& particle : m_vecSparkParticleSystemPool) {
		if (particle->GetActive())
			continue;
		else {
			pParticle = particle;
			break;
		}
	}

	if (!pParticle)
		return;

	pParticle->ParticleRestart();
	pParticle->SetEmitvPosition(pos);
}

void CParticleManager::CreateParticle(ParticleTag tag, XMVECTOR pos)
{
	/*
	auto findParticlePool = m_mapNoEffectParticlePool.find(tag);
	
	if (findParticlePool == m_mapNoEffectParticlePool.end()) {
		findParticlePool = m_mapEffectParticlePool.find(tag);

		if (findParticlePool == m_mapNoEffectParticlePool.end())
			MessageBox(NULL, s_to_ws("Particle Tag : " + to_string(static_cast<int>(tag))).c_str(), L"Error", MB_OK);
	}

	CParticleSystem* pParticle = nullptr;
	for (auto& particle : findParticlePool->second) {
		if (particle->GetActive())
			continue;
		else {
			pParticle = particle;
			break;
		}
	}

	if (!pParticle)
		return;

	pParticle->ParticleRestart();
	pParticle->SetEmitvPosition(pos);
	*/
}

void CParticleManager::UpdateManager(float fDeltaTime)
{
	XMVECTOR rainOffset = SCENE_MGR->g_pCamera->GetvPosition();
	rainOffset += SCENE_MGR->g_pCamera->GetvUp() * 20;
	rainOffset += SCENE_MGR->g_pCamera->GetvLook() * 20;

	m_pRainParticle->SetEmitvPosition(rainOffset);
	m_pRainParticle->Update(fDeltaTime);

	for (auto& system : m_vecSparkParticleSystemPool)
		if (system->GetActive())
			system->Update(fDeltaTime);

	for (auto& system : m_vecBloodParticleSystemPool)
		if (system->GetActive())
			system->Update(fDeltaTime);

	for (auto& system : m_vecCopiousBleedingParticleSystemPool)
		if (system->GetActive())
			system->Update(fDeltaTime);

	for (auto& system : m_vecParticleSystemContainer)
		if (system->GetActive())
			system->Update(fDeltaTime);
}

void CParticleManager::RenderAllNoEffect(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_pRainParticle->Render(pd3dDeviceContext);



	for (auto& system : m_vecSparkParticleSystemPool)
		if (system->GetActive())
			system->Render(pd3dDeviceContext);

	for (auto& system : m_vecCopiousBleedingParticleSystemPool)
		if (system->GetActive())
			system->Render(pd3dDeviceContext);

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