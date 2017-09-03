#include "stdafx.h"
#include "SceneManager.h"

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
	{
		pScene = new CTitleScene();

		GLOBAL_MGR->g_vRenderOption = XMFLOAT4(0, 0, 0, 1.0f);

		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Map(GLOBAL_MGR->g_pd3dcbRenderOption, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		XMFLOAT4 *pcbRenderOption = (XMFLOAT4 *)d3dMappedResource.pData;
		*pcbRenderOption = GLOBAL_MGR->g_vRenderOption;
		STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Unmap(GLOBAL_MGR->g_pd3dcbRenderOption, 0);
	}
		break;
	case SceneTag::eWaitScene:
		pScene = new CWaitScene();
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