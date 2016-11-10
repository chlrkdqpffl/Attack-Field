#pragma once
#include "SingletonManager.h"
#include "Scene.h"

class CSceneManager : public CSingletonManager<CSceneManager>
{
public:
	CScene				*m_nowScene		= nullptr;
	CPlayer				*m_pPlayer		= nullptr;
	CCamera				*m_pCamera		= nullptr;

	CSceneManager();
	virtual ~CSceneManager();

	void ChangeScene(CScene* newScene);

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;
};
