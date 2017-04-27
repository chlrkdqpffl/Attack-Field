#pragma once
#include "Scene.h"
#include "BoundingBoxShader.h"
#include "GBuffer.h"

class CMainScene : public CScene
{
public:
	CMainScene();
	virtual ~CMainScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex = 0);

	virtual void Initialize();
	virtual void CreateTweakBars();
	virtual void CreateUIImage() override;
	virtual void ReleaseObjects();

	virtual void CreateConstantBuffers();
	virtual void UpdateConstantBuffers(LIGHTS *pLights);
	virtual void ReleaseConstantBuffers();

	virtual void CreateLights();

	virtual void UpdateObjects(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera);
	virtual void RenderAllText(ID3D11DeviceContext *pd3dDeviceContext);

	void CreateMapDataObject();
	void ModifiedSelectObject();
	void AddShaderObject(ShaderTag tag, CGameObject* pObject);

	void RenderBoundingBox();

private:
	float							m_fGlobalAmbient;
	ID3D11Buffer					*m_pd3dcbLights		= nullptr;


	// Bounding Box Render
	bool							m_bIsPreCollisionCheck = false;
	CBoundingBoxShader				*m_pBoundingBoxShader = nullptr;
	vector<CGameObject*>			m_vecBBoxRenderContainer;
	vector<CCharacterObject*>		m_vecCharacterContainer;

	CGBuffer*						m_GBuffer = nullptr;
};