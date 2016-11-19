#pragma once
#include "Scene.h"

class CScene_Main : public CScene
{
public:
	CScene_Main();
	virtual ~CScene_Main();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex = 0);

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void ReleaseObjects();

	virtual void CreateConstantBuffers(ID3D11Device *pd3dDevice);
	virtual void UpdateConstantBuffers(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS *pLights);
	virtual void ReleaseConstantBuffers();

	virtual void CreateLights() override;

	virtual bool ProcessInput(UCHAR *pKeysBuffer);
	virtual void UpdateObjects(float fTimeElapsed);
	virtual void OnPreRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera);
	virtual void RenderAllText(ID3D11DeviceContext *pd3dDeviceContext);

private:

	ID3D11Buffer					*m_pd3dcbLights		= nullptr;
	
};

