#include "stdafx.h"
#include "SpriteImageManager.h"

void CSpriteImageManager::InitializeManager()
{
	AddSpriteMesh(TextureTag::eFireEffect, 7, POINT{ 200,200 }, 3.0f, 10.0f);

}

void CSpriteImageManager::ReleseManager()
{
	for (auto& index = m_mapSpriteMeshPool.begin(); index != m_mapSpriteMeshPool.end(); ++index)
		SafeDelete(index->second);
}

void CSpriteImageManager::AddSpriteMesh(TextureTag tag, int frame, POINT perSize, float lifeTime, float totalTime)
{
//	CSpriteImageMesh* pSpriteMesh = new CSpriteImageMesh(STATEOBJ_MGR->g_pd3dDevice, tag, frame, perSize, lifeTime, totalTime);

//	m_mapSpriteMeshPool.insert(make_pair(tag, pSpriteMesh));

	// �� �±׿� ������ ��ϵǾ���
	assert(m_mapSpriteMeshPool.count(tag) <= 1);
}

CSpriteImageMesh* CSpriteImageManager::CloneSpriteMesh(TextureTag tag)
{
	auto findResource = m_mapSpriteMeshPool.find(tag);

	// Pool�� �ش� �����Ͱ� �������� �ʴ´�.
	if (findResource == m_mapSpriteMeshPool.end())
		MessageBox(NULL, s_to_ws("Texture Tag : " + to_string(static_cast<int>(tag))).c_str(), L"Sprite Image Error", MB_OK);

	return (*findResource).second;
}

void CSpriteImageManager::UpdateManager(float fDeltaTime)
{

}