#pragma once
#include "Scene.h"

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
	virtual void ReleaseObjects();

	virtual void CreateConstantBuffers();
	virtual void UpdateConstantBuffers(LIGHTS *pLights);
	virtual void ReleaseConstantBuffers();

	virtual void CreateLights();

	virtual bool ProcessInput(UCHAR *pKeysBuffer);
	virtual void UpdateObjects(float fTimeElapsed);
	virtual void OnPreRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera);
	virtual void RenderAllText(ID3D11DeviceContext *pd3dDeviceContext);

	void CreateMapDataObject();

private:
	float							m_fGlobalAmbient;
	ID3D11Buffer					*m_pd3dcbLights		= nullptr;
};

