#pragma once
#include "Scene.h"
#include "BoundingBoxShader.h"
#include "GBuffer.h"
#include "PostFX.h"
#include "SSReflection.h"

class CMainScene : public CScene 
{
public:
	CMainScene();
	virtual ~CMainScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex = 0);
	virtual void OnChangedWindowsSize(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void Initialize() override;
	virtual void CreateTweakBars();
	virtual void CreateUIImage() override;
	virtual void ReleaseObjects();

	virtual void CreateConstantBuffers();
	virtual void UpdateConstantBuffers();
	virtual void ReleaseConstantBuffers();

	virtual void CreateLights();

	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera) override;
	virtual void RenderAllText(ID3D11DeviceContext *pd3dDeviceContext) override;

	void RenderBoundingBox();
	void RenderReflection(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	void CreateSound();
	void CreateMapDataInstancingObject();
	void CreateTestingObject();
	void ModifiedSelectObject();
	void AddShaderObject(ShaderTag tag, CGameObject* pObject);

	void Update_Light();
	void Update_LightningStrikes(float fDeltaTime);
	void GameRoundOver(float fDeltaTime);
	void CalcTime();
	void CalcOccupyTime();
	void CalcOccupyPosition();

	// UI
	void PrepareRenderUI();
	void RenderUI();
	void ShowAimUI();
	void ShowDamageDirection();
	void ShowDeathRespawnUI();
	void ShowOccupyUI();
	void ShowDeadlyUI();
	void ShowDeadlyAttackUI();
	void CalcDamagedDirection();


	vector<CCharacterObject*> &GetCharcontainer() { return m_vecCharacterContainer; }
	vector<CGameObject*>	&GetBbBoxcontainer()	{return m_vecBBoxRenderContainer; }
	
	void SetGameTime(UINT time) { m_nGameTime = time; }
	void SetOccupyTime(float time) { m_OccupyTime = time; }
	void SetRedTeamKill(UINT kill) { m_nRedTeamTotalKill = kill; }
	void SetBlueTeamKill(UINT kill) { m_nBlueTeamTotalKill = kill; }
	void SetOccupyTeam(TeamType team) { m_tagOccupyTeam = team; }
	BYTE GetOcuupyTeam() { return static_cast<BYTE>(m_tagOccupyTeam); }

	CGBuffer* GetGBuffer() const { return m_GBuffer; }
private:
	// Light Info
	XMFLOAT3						m_f3DirectionalAmbientLowerColor = XMFLOAT3(0, 0, 0);
	XMFLOAT3						m_f3DirectionalAmbientUpperColor = XMFLOAT3(0, 0, 0);
	XMFLOAT3						m_f3DirectionalDirection = XMFLOAT3(0, 0, 0);
	XMFLOAT3						m_f3DirectionalColor = XMFLOAT3(0, 0, 0);

	ID3D11Buffer					*m_pd3dcbLights		= nullptr;
	ID3D11Buffer					*m_pd3dcbTestVariable = nullptr;

	CAimObject*						m_pAimObject = nullptr;

	// Bounding Box Render
	bool							m_bIsPreCollisionCheck = false;
	CBoundingBoxShader				*m_pBoundingBoxShader = nullptr;

	// Container
	vector<CGameObject*>			m_vecBBoxRenderContainer;
	vector<CCharacterObject*>		m_vecCharacterContainer;

	// Deferred Rendering
	CGBuffer*						m_GBuffer = nullptr;
	CPostFX*						m_PostFX = nullptr;

	ID3D11Texture2D*				m_pHDRTexture = nullptr;
	ID3D11RenderTargetView*			m_HDRRTV = nullptr;
	ID3D11ShaderResourceView*		m_HDRSRV = nullptr;

	// SSReflection
	CSSReflection*					m_pSSReflection = nullptr;
	vector<CGameObject*>			m_vecReflectObjectContainer;

	// ----- Game System ----- //
	DWORD							m_dwTime = 0;
	DWORD							m_dwLastLightningTime = 0;
	bool							m_bIsGameRoundOver = false;
	UINT							m_nGameTime = 0;
	int								m_OccupyTime = 0;
	UINT							m_nRedTeamTotalKill = 0;
	UINT							m_nBlueTeamTotalKill = 0;
	GameMode						m_tagGameMode = GameMode::eNone;


	// ----- Occupy Variable ----- //
	const XMFLOAT3					m_cf3OccupyPosition = XMFLOAT3(130.0f, 25.0f, 168.0f);
	DWORD							m_dwGameRoundOverTime = 0;
	TeamType						m_tagOccupyTeam = TeamType::eNone;
	UINT							m_nRedScore = 0;
	UINT							m_nBlueScore = 0;
};