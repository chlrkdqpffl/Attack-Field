#pragma once
#include "SingletonManager.h"

class CSpriteImageObject;

struct SpriteInfo
{
	int			m_nSizeX = 0;
	int			m_nSizeY = 0;
	float		m_fLifeTime = 0.0f;

	SpriteInfo() {}
	SpriteInfo(int sizeX, int sizeY, float lifeTime) { m_nSizeX = sizeX, m_nSizeY = sizeY, m_fLifeTime = lifeTime; }
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

	void CreateSpriteImage(TextureTag tag, XMFLOAT3 position, float sizeX, float sizeY, bool bIsInfinity = false);
	void RenderAll(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera);

private:
	map<TextureTag, SpriteInfo>			m_mapSpriteInfoPool;
	vector<CSpriteImageObject*>			m_vecSpriteObjectContainer;
};