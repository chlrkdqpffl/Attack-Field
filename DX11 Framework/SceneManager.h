#pragma once
#include "SingletonManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "LoadingScene.h"
#include "MainScene.h"

class CSceneManager : public CSingletonManager<CSceneManager>
{
public:
	CMainScene						*g_pMainScene	= nullptr;
	CScene							*g_nowScene		= nullptr;
	CCamera							*g_pCamera		= nullptr;
	CPlayer							*g_pPlayer		= nullptr;
	ID3D11RenderTargetView			*g_pd3dRenderTargetView = nullptr;
	ID3D11DepthStencilView			*g_pd3dDepthStencilView = nullptr;
	float							g_fTimeElapsed	= 0.0f;


public:
	CSceneManager();
	virtual ~CSceneManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	void ChangeScene(SceneTag tagScene);
	bool LoadSceneData();
};
