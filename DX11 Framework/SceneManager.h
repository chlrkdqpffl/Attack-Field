#pragma once
#include "SingletonManager.h"
#include "Scene.h"

class CSceneManager : public CSingletonManager<CSceneManager>
{
public:
	CScene				*g_nowScene		= nullptr;
	CCamera				*g_pCamera		= nullptr;
	CPlayer				*g_pPlayer		= nullptr;
	float				g_fTimeElapsed	= 0.0f;


public:
	CSceneManager();
	virtual ~CSceneManager();

	void ChangeScene(CScene* newScene);

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;
};
