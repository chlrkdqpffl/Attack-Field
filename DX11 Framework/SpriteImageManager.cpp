#include "stdafx.h"
#include "SpriteImageManager.h"
#include "SpriteImageObject.h"

void CSpriteImageManager::InitializeManager()
{
//	AddSpriteInfo(TextureTag::eExplosionSprite, SpriteInfo(10, 5, 1.0f));
//	AddSpriteInfo(TextureTag::eExplosionSprite2, SpriteInfo(5, 3, 1.0f));

//	AddSpriteInfo(TextureTag::eExplosionSprite, SpriteInfo(7, 1, 0.6f));
	AddSpriteInfo(TextureTag::eExplosionSprite2, SpriteInfo(POINT{ 7, 1 }, 0.05f, 0.05f, 0.1f));

	CSpriteImageObject::CreateConstantBuffers();
}

void CSpriteImageManager::ReleseManager()
{
	for (auto& index = m_mapSpriteObjectContainer.begin(); index != m_mapSpriteObjectContainer.end(); ++index)
		for (auto& system : index->second)
			SafeDelete(system);

	CSpriteImageObject::ReleaseConstantBuffers();
}

void CSpriteImageManager::AddSpriteInfo(TextureTag tag, SpriteInfo info)
{
	m_mapSpriteInfoPool.insert(make_pair(tag, info));

	// 한 태그에 여러개 등록되었음
	assert(m_mapSpriteInfoPool.count(tag) <= 1);
}

SpriteInfo CSpriteImageManager::CloneSpriteInfo(TextureTag tag)
{
	auto findResource = m_mapSpriteInfoPool.find(tag);

	// Pool에 해당 데이터가 존재하지 않는다.
	if (findResource == m_mapSpriteInfoPool.end())
		MessageBox(NULL, s_to_ws("Texture Tag : " + to_string(static_cast<int>(tag))).c_str(), L"Sprite Image Error", MB_OK);

	return (*findResource).second;
}

void CSpriteImageManager::CreateSpriteImage(TextureTag tag, XMFLOAT3 position, bool bIsInfinity)
{
	CSpriteImageObject* pSpriteObject = new CSpriteImageObject(SCENE_MGR->g_pPlayer, tag, bIsInfinity);
	pSpriteObject->CreateObjectData(STATEOBJ_MGR->g_pd3dDevice);
	pSpriteObject->SetPosition(position);

	auto findSprite = m_mapSpriteObjectContainer.find(tag);
	if (findSprite != m_mapSpriteObjectContainer.end()) {
		findSprite->second.push_back(pSpriteObject);
		return;
	}

	// Not Found Sprite - new Sprite Container
	vector<CSpriteImageObject*> vecObjectContainer;
	vecObjectContainer.reserve(5);
	vecObjectContainer.push_back(pSpriteObject);

	m_mapSpriteObjectContainer.insert(make_pair(tag, vecObjectContainer));
}

void CSpriteImageManager::ActivationSprite(TextureTag tag, UINT objectID)
{
	auto findSpriteContainer = m_mapSpriteObjectContainer.find(tag);
	if (findSpriteContainer != m_mapSpriteObjectContainer.end()) {
		auto spriteObject = findSpriteContainer->second[objectID];
		spriteObject->SetActive(true);
	}
	else
		MessageBox(NULL, s_to_ws("Texture Tag : " + to_string(static_cast<int>(tag))).c_str(), L"Sprite Image Error", MB_OK);
}

void CSpriteImageManager::SetPosition(TextureTag tag, XMFLOAT3 position, UINT objectID)
{
	auto findSpriteContainer = m_mapSpriteObjectContainer.find(tag);
	if (findSpriteContainer != m_mapSpriteObjectContainer.end()) {
		auto spriteObject = findSpriteContainer->second[objectID];
		spriteObject->SetPosition(position);
	}
	else
		MessageBox(NULL, s_to_ws("Texture Tag : " + to_string(static_cast<int>(tag))).c_str(), L"Sprite Image Error", MB_OK);
}

void CSpriteImageManager::DisableSprite(TextureTag tag, UINT objectID)
{
	auto findSpriteContainer = m_mapSpriteObjectContainer.find(tag);
	if (findSpriteContainer != m_mapSpriteObjectContainer.end()) {
		auto spriteObject = findSpriteContainer->second[objectID];
		spriteObject->SetActive(false);
	}
	else
		MessageBox(NULL, s_to_ws("Texture Tag : " + to_string(static_cast<int>(tag))).c_str(), L"Sprite Image Error", MB_OK);
}

void CSpriteImageManager::UpdateManager(float fDeltaTime)
{
	for (auto& container : m_mapSpriteObjectContainer)
		for (auto& system : container.second)
			if (system->GetActive())
				system->Update(fDeltaTime);
}

void CSpriteImageManager::RenderAll(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	pd3dDeviceContext->OMSetBlendState(STATEOBJ_MGR->g_pFireBS, nullptr, 0xffffffff);

	for (auto& container : m_mapSpriteObjectContainer) {
			for (auto& system : container.second)
				if (system->GetActive())
					system->Render(pd3dDeviceContext, pCamera);
	}

	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}