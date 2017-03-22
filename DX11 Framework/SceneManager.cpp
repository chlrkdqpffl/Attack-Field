#include "stdafx.h"
#include "SceneManager.h"

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::InitializeManager()
{
}

void CSceneManager::ReleseManager()
{
}

void CSceneManager::ChangeScene(CScene* newScene)
{
	if (g_nowScene) {
		g_nowScene->ReleaseObjects();
		delete g_nowScene;
	}
	g_nowScene = newScene;

	g_nowScene->BuildObjects(STATEOBJ_MGR->g_pd3dDevice);
}