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
	float							g_fDeltaTime	= 0.0f;
	PxScene*						g_pPxScene		= nullptr;
	bool							g_bMouseBindFlag = true;

	// Server
	bool							m_loginfail = false;
	LPARAM							g_lParam		= 0;

public:
	CSceneManager() {};
	virtual ~CSceneManager() {};

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	void ChangeScene(SceneTag tagScene);
};