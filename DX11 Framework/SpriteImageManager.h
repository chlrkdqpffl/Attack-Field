#pragma once
#include "SingletonManager.h"
class CSpriteImageManager :	public CSingletonManager<CSpriteImageManager>
{


public:
	CSpriteImageManager() {}
	virtual ~CSpriteImageManager() {}

	virtual void InitializeManager() override;
	virtual void UpdateManager(float fDeltaTime);
	virtual void ReleseManager() override;
};