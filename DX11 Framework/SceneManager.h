#pragma once
#include "SingletonManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "LoadingScene.h"
#include "MainScene.h"
#include "WaitScene.h"

class CSceneManager : public CSingletonManager<CSceneManager>
{
public:
	CMainScene						*g_pMainScene	= nullptr;
	CScene							*g_nowScene		= nullptr;

	// ----- Etc ----- // 
	CCamera							*g_pCamera		= nullptr;
	CPlayer							*g_pPlayer		= nullptr;
	CCharacterPlayer				*g_pPlayerCharacter = nullptr;
	ID3D11RenderTargetView			*g_pd3dRenderTargetView = nullptr;
	ID3D11DepthStencilView			*g_pd3dDepthStencilView = nullptr;
	bool							m_loginfail = false;
	float							g_fDeltaTime	= 0.0f;
	LPARAM							g_lParam			= 0;
	PxScene*						g_pPxScene		= nullptr;

public:
	CSceneManager() {};
	virtual ~CSceneManager() {};

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	void ChangeScene(SceneTag tagScene);
};