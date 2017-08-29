#pragma once
#include "SingletonManager.h"
#include "SpriteImageMesh.h"

class CSpriteImageManager :	public CSingletonManager<CSpriteImageManager>
{
	map<TextureTag, CSpriteImageMesh*>	m_mapSpriteMeshPool;

public:
	CSpriteImageManager() {}
	virtual ~CSpriteImageManager() {}

	virtual void InitializeManager() override;
	virtual void UpdateManager(float fDeltaTime);
	virtual void ReleseManager() override;

	void AddSpriteMesh(TextureTag tag, int frame, POINT perSize, float lifeTime, float totalTime);
	CSpriteImageMesh* CloneSpriteMesh(TextureTag tag);
};