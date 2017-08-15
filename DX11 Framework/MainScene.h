#pragma once
#include "Scene.h"
#include "BoundingBoxShader.h"
#include "GBuffer.h"
#include "PostFX.h"
#include "LightManager.h"

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
	virtual void UpdateConstantBuffers(LIGHTS *pLights);
	virtual void ReleaseConstantBuffers();

	virtual void CreateLights();

	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera) override;
	virtual void RenderAllText(ID3D11DeviceContext *pd3dDeviceContext) override;

	void CreateMapDataObject();
	void CreateMapDataInstancingObject();
	void CreateTestingObject();
	void ModifiedSelectObject();
	void AddShaderObject(ShaderTag tag, CGameObject* pObject);

	void RenderBoundingBox();
	void CalcTime();

	// UI
	void RenderUI();
	void ShowDeathRespawnUI();
	void ShowOccupyRespawnUI();
	void ShowDeadlyUI();
	void ShowDeadlyAttackUI();

	vector<CCharacterObject*> &GetCharcontainer() { return m_vecCharacterContainer; }
	vector<CGameObject*>	&GetBbBoxcontainer()	{return m_vecBBoxRenderContainer; }
	
	void SetGameTime(UINT time) { m_nGameTime = time; }
	void SetRedTeamKill(UINT kill) { m_nRedTeamTotalKill = kill; }
	void SetBlueTeamKill(UINT kill) { m_nBlueTeamTotalKill = kill; }

private:
	// Light Info
	XMFLOAT3						m_f3DirectionalAmbientLowerColor = XMFLOAT3(0, 0, 0);
	XMFLOAT3						m_f3DirectionalAmbientUpperColor = XMFLOAT3(0, 0, 0);
	XMFLOAT3						m_f3DirectionalDirection = XMFLOAT3(0, 0, 0);
	XMFLOAT3						m_f3DirectionalColor = XMFLOAT3(0, 0, 0);

	ID3D11Buffer					*m_pd3dcbLights		= nullptr;
	ID3D11Buffer					*m_pd3dcbTestVariable = nullptr;


	// Bounding Box Render
	bool							m_bIsPreCollisionCheck = false;
	CBoundingBoxShader				*m_pBoundingBoxShader = nullptr;

	vector<CGameObject*>			m_vecBBoxRenderContainer;
	vector<CCharacterObject*>		m_vecCharacterContainer;

	// Deferred Rendering
	CLightManager*					m_pLightManager = nullptr;
	CGBuffer*						m_GBuffer = nullptr;
	CPostFX*						m_PostFX = nullptr;

	ID3D11Texture2D*				m_pHDRTexture = nullptr;
	ID3D11RenderTargetView*			m_HDRRTV = nullptr;
	ID3D11ShaderResourceView*		m_HDRSRV = nullptr;

	// ----- Game System ----- //
	DWORD							m_dwTime = 0;
	UINT							m_nGameTime = 0;
	UINT							m_nRedTeamTotalKill = 0;
	UINT							m_nBlueTeamTotalKill = 0;
	CUIObject*						m_pDamageUI = nullptr;
};