#pragma once
#include "Scene.h"
#include "BoundingBoxShader.h"
#include "GBuffer.h"


class CMainScene : public CScene 
{
public:
	CMainScene();
	virtual ~CMainScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex = 0);

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
	void ModifiedSelectObject();
	void AddShaderObject(ShaderTag tag, CGameObject* pObject);

	void RenderBoundingBox();
	void CalcTime();

	vector<CCharacterObject*> &GetCharcontainer() { return m_vecCharacterContainer; }
	vector<CGameObject*>	&GetBbBoxcontainer()	{return m_vecBBoxRenderContainer; }
	
	void SetGameTime(UINT time) { m_nGameTime = time; }
	void SetRedTeamKill(UINT kill) { m_nRedTeamTotalKill = kill; }
	void SetBlueTeamKill(UINT kill) { m_nBlueTeamTotalKill = kill; }

private:
	float							m_fGlobalAmbient	= 0.0f;
	ID3D11Buffer					*m_pd3dcbLights		= nullptr;


	// Bounding Box Render
	bool							m_bIsPreCollisionCheck = false;
	CBoundingBoxShader				*m_pBoundingBoxShader = nullptr;

	vector<CGameObject*>			m_vecBBoxRenderContainer;
	vector<CCharacterObject*>		m_vecCharacterContainer;

	CGBuffer*						m_GBuffer = nullptr;

	// ----- Game System ----- //
	DWORD							m_dwTime = 0;
	UINT							m_nGameTime = 0;
	UINT							m_nRedTeamTotalKill = 0;
	UINT							m_nBlueTeamTotalKill = 0;
};