#include "stdafx.h"
#include "SpriteImageManager.h"
#include "SpriteImageObject.h"

void CSpriteImageManager::InitializeManager()
{
//	AddSpriteInfo(TextureTag::eExplosionSprite, SpriteInfo(10, 5, 1.0f));
//	AddSpriteInfo(TextureTag::eExplosionSprite2, SpriteInfo(5, 3, 1.0f));

	AddSpriteInfo(TextureTag::eExplosionSprite, SpriteInfo(7, 1, 0.6f));
	AddSpriteInfo(TextureTag::eExplosionSprite2, SpriteInfo(7, 1, 0.6f));
	CSpriteImageObject::CreateConstantBuffers();


}

void CSpriteImageManager::ReleseManager()
{
	for (auto& object : m_vecSpriteObjectContainer)
		SafeDelete(object);

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

void CSpriteImageManager::CreateSpriteImage(TextureTag tag, XMFLOAT3 position, float sizeX, float sizeY, bool bIsInfinity)
{
	CSpriteImageObject* pSpriteObject = new CSpriteImageObject(SCENE_MGR->g_pPlayer, tag, sizeX, sizeY, bIsInfinity);
	pSpriteObject->CreateObjectData(STATEOBJ_MGR->g_pd3dDevice);
	pSpriteObject->SetPosition(position);

	m_vecSpriteObjectContainer.push_back(pSpriteObject);
}

void CSpriteImageManager::UpdateManager(float fDeltaTime)
{
	for (auto& object : m_vecSpriteObjectContainer)
		object->Update(fDeltaTime);
}

void CSpriteImageManager::RenderAll(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	pd3dDeviceContext->OMSetBlendState(STATEOBJ_MGR->g_pFireBS, nullptr, 0xffffffff);

	for (auto& object : m_vecSpriteObjectContainer)
		object->Render(pd3dDeviceContext, pCamera);

	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}