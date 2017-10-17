#pragma once
#include "SingletonManager.h"

class CSpriteImageObject;

struct SpriteInfo
{
	POINT		m_nSpriteCount = POINT{ 0,0 };
	float		m_fMeshSizeX = 0.0f;
	float		m_fMeshSizeY = 0.0f;
	float		m_fLifeTime = 0.0f;

	SpriteInfo() {}
	SpriteInfo(POINT spriteCount, float meshSizeX, float meshSizeY, float lifeTime) { m_nSpriteCount = spriteCount, m_fMeshSizeX = meshSizeX, m_fMeshSizeY = meshSizeY, m_fLifeTime = lifeTime; }
};

class CSpriteImageManager :	public CSingletonManager<CSpriteImageManager>
{
public:
	CSpriteImageManager() {}
	virtual ~CSpriteImageManager() {}

	virtual void InitializeManager() override;
	virtual void UpdateManager(float fDeltaTime);
	virtual void ReleseManager() override;

	void AddSpriteInfo(TextureTag tag, SpriteInfo info);
	SpriteInfo CloneSpriteInfo(TextureTag tag);

	CSpriteImageObject* CreateSpriteImage(TextureTag tag, XMFLOAT3 position, bool bIsInfinity = false);
	void ActivationSprite(CSpriteImageObject* spriteID);
	void SetPosition(CSpriteImageObject* spriteID, XMFLOAT3 position);
	void DisableSprite(CSpriteImageObject* spriteID);
	void RenderAll(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera);

private:
	unordered_map<TextureTag, SpriteInfo>	m_mapSpriteInfoPool;
	vector<CSpriteImageObject*>				m_vecSpriteObjectContainer;
};