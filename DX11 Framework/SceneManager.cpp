#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	m_nowScene = nullptr;
}


SceneManager::~SceneManager()
{
}

void SceneManager::InitializeManager()
{

}

void SceneManager::ChangeScene(CScene* newScene)
{
	if (m_nowScene) {
		m_nowScene->ReleaseObjects();
		delete m_nowScene;
	}
	m_nowScene = newScene;

//	m_nowScene->BuildObjects(G_VAR->g_pd3dDevice);
}