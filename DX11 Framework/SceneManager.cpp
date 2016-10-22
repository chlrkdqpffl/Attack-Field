#include "stdafx.h"
#include "SceneManager.h"

CSceneManager::CSceneManager()
{
	m_nowScene = nullptr;
}

CSceneManager::~CSceneManager()
{

}

void CSceneManager::InitializeManager()
{
}

void CSceneManager::ChangeScene(CScene* newScene)
{
	if (m_nowScene) {
		m_nowScene->ReleaseObjects();
		delete m_nowScene;
	}
	m_nowScene = newScene;

	m_nowScene->BuildObjects(STATEOBJ_MGR->m_pd3dDevice.Get());
}