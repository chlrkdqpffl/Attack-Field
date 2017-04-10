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
	g_pMainScene = new CMainScene();		// �̸� �ε��� �ؾ��ϹǷ� ���� ����
}

void CSceneManager::ReleseManager()
{
	SafeDelete(g_pMainScene);
}

bool CSceneManager::LoadSceneData()
{
	g_pMainScene->SetDevice(STATEOBJ_MGR->g_pd3dDevice);
	g_pMainScene->SetDeviceContext(STATEOBJ_MGR->g_pd3dImmediateDeviceContext);
	g_pMainScene->Initialize();
	return true;
}

void CSceneManager::ChangeScene(SceneTag tagScene)
{
	CScene* pScene;
	switch (tagScene) {
	case SceneTag::eTitleScene:
		pScene = new CTitleScene();
		break;
	case SceneTag::eLoadingScene:
		pScene = new CLoadingScene();
		break;
	case SceneTag::eMainScene:
		pScene = g_pMainScene;
		break;
	default:
		cout << "Scene ������ ����" << endl;
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