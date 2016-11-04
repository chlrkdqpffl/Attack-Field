#pragma once
#include "SingletonManager.h"
#include "Scene.h"

class CSceneManager : public CSingletonManager<CSceneManager>
{
public:
	CScene				*m_nowScene;

	CSceneManager();
	virtual ~CSceneManager();


	void ChangeScene(CScene* newScene);

	virtual void InitializeManager();
	
};
