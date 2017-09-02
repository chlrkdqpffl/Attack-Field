#include "stdafx.h"
#include "SpriteImageManager.h"
#include "SpriteImageObject.h"

void CSpriteImageManager::InitializeManager()
{
//	AddSpriteInfo(TextureTag::eExplosionSprite, SpriteInfo(10, 5, 1.0f));
//	AddSpriteInfo(TextureTag::eExplosionSprite2, SpriteInfo(5, 3, 1.0f));

//	AddSpriteInfo(TextureTag::eExplosionSprite, SpriteInfo(POINT{ 10, 5 }, 0.4f, 0.2f, 0.1f));
	AddSpriteInfo(TextureTag::eGunExplosionOtherPlayer, SpriteInfo(POINT{ 5, 1 }, 0.3f, 0.2f, 0.1f));
	AddSpriteInfo(TextureTag::eGunExplosionPlayer,		SpriteInfo(POINT{ 5, 1 }, 0.05f, 0.05f, 0.1f));

	CSpriteImageObject::CreateConstantBuffers();
}

void CSpriteImageManager::ReleseManager()
{
	for (auto& obj : m_vecSpriteObjectContainer)
		SafeDelete(obj);

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

CSpriteImageObject* CSpriteImageManager::CreateSpriteImage(TextureTag tag, XMFLOAT3 position, bool bIsInfinity)
{
	CSpriteImageObject* spriteID = new CSpriteImageObject(tag, bIsInfinity);
	spriteID->CreateObjectData(STATEOBJ_MGR->g_pd3dDevice);
	spriteID->SetPosition(position);

	m_vecSpriteObjectContainer.push_back(spriteID);

	return spriteID;
}

void CSpriteImageManager::ActivationSprite(CSpriteImageObject* spriteID)
{
	for (auto& spriteObject : m_vecSpriteObjectContainer)
		if (spriteObject == spriteID) {
			spriteObject->SetActive(true);
			return;
		}
}

void CSpriteImageManager::SetPosition(CSpriteImageObject* spriteID, XMFLOAT3 position)
{
	for (auto& spriteObject : m_vecSpriteObjectContainer)
		if (spriteObject == spriteID) {
			spriteObject->SetPosition(position);
			return;
		}
}

void CSpriteImageManager::DisableSprite(CSpriteImageObject* spriteID)
{
	for (auto& spriteObject : m_vecSpriteObjectContainer)
		if (spriteObject == spriteID) {
			spriteObject->SetActive(false);
			return;
		}
}

void CSpriteImageManager::UpdateManager(float fDeltaTime)
{
	for (auto& spriteObject : m_vecSpriteObjectContainer)
		if (spriteObject->GetActive())
			spriteObject->Update(fDeltaTime);
}

void CSpriteImageManager::RenderAll(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	ID3D11DepthStencilState* prevDSS; UINT prevStencil;
	pd3dDeviceContext->OMGetDepthStencilState(&prevDSS, &prevStencil);

	pd3dDeviceContext->OMSetBlendState(STATEOBJ_MGR->g_pFireBS, nullptr, 0xffffffff);
	pd3dDeviceContext->OMSetDepthStencilState(STATEOBJ_MGR->g_pNoDepthWritesDSS, 0);

	for (auto& spriteObject : m_vecSpriteObjectContainer)
		if (spriteObject->GetActive())
			spriteObject->Render(pd3dDeviceContext, pCamera);

	pd3dDeviceContext->OMSetDepthStencilState(prevDSS, prevStencil);
	pd3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}