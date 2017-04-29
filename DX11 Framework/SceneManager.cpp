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

void CSceneManager::ChangeScene(SceneTag tagScene)
{
	SceneTag nCurrentSceneTag = (g_nowScene) ? g_nowScene->GetSceneTag() : SceneTag::eNone;
	if (tagScene == nCurrentSceneTag)
		return;

	CScene* pScene;
	switch (tagScene) {
	case SceneTag::eTitleScene:
		pScene = new CTitleScene();
		break;
	case SceneTag::eLoadingScene:
		pScene = new CLoadingScene();
		break;
	case SceneTag::eMainScene:
		g_pMainScene = new CMainScene();
		pScene = g_pMainScene;
		break;
	default:
		cout << "CSceneManager::ChangeScene - Scene 정보가 없음" << endl;
		break;
	}

	if (g_nowScene) {
		g_nowScene->ReleaseObjects();
		delete g_nowScene;
	}

	g_nowScene = pScene;
	g_nowScene->SetDevice(STATEOBJ_MGR->g_pd3dDevice);
	g_nowScene->SetDeviceContext(STATEOBJ_MGR->g_pd3dImmediateDeviceContext);

	g_nowScene->Initialize();
}