#pragma once
#include "SingletonManager.h"
//#include "UserDefine.h"
#include "Scene.h"


class SceneManager : public CSingletonManager<SceneManager>
{
public:
	CScene				*m_nowScene;

	SceneManager();
	virtual ~SceneManager();


	void ChangeScene(CScene* newScene);

	virtual void InitializeManager();
};
