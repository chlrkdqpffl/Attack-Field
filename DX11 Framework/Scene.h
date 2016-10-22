//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once

#include "Camera.h"
#include "Shader.h"


// Object
#include "SkyBox.h"
#include "HeightMapTerrain.h"
#include "TerrainWater.h"
#include "RotatingObject.h"

#define MAX_LIGHTS			4 //Multiple of 4
#define POINT_LIGHT			1.0f
#define SPOT_LIGHT			2.0f
#define DIRECTIONAL_LIGHT	3.0f

struct LIGHT
{
	XMFLOAT4				m_d3dxcAmbient;
	XMFLOAT4				m_d3dxcDiffuse;
	XMFLOAT4				m_d3dxcSpecular;
	XMFLOAT3				m_d3dxvPosition;
	float					m_fRange;
	XMFLOAT3				m_d3dxvDirection;
	float					m_nType;
	XMFLOAT3				m_d3dxvAttenuation;
	float 					m_fFalloff;
	float 					m_fTheta; //cos(m_fTheta)
	float					m_fPhi; //cos(m_fPhi)
	float					m_bEnable;
	float					padding;
};

struct LIGHTS
{
	LIGHT					m_pLights[MAX_LIGHTS];
	XMFLOAT4				m_d3dxcGlobalAmbient;
	XMFLOAT4				m_d3dxvCameraPosition;
};

class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex = 0);

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void ReleaseObjects();

	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS *pLights);
	virtual void ReleaseShaderVariables();

	virtual bool ProcessInput(UCHAR *pKeysBuffer);
	virtual void UpdateObjects(float fTimeElapsed);
	virtual void OnPreRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera);
	virtual void RenderAllText(ID3D11DeviceContext *pd3dDeviceContext);

public:
	CGameObject *PickObjectPointedByCursor(int xClient, int yClient);

	// ----- Get, Setter ----- // 
	CHeightMapTerrain *GetTerrain() const {	return m_pTerrain; }

	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }


protected:
	CGameObject						**m_ppObjects;
	int								m_nObjects;

	CObjectsShader					**m_ppObjectShaders;
	int								m_nObjectShaders;

	CInstancedObjectsShader			**m_ppInstancingShaders;
	int								m_nInstancingShaders;

	CPlayer							*m_pPlayer;
	CCamera							*m_pCamera;
	CGameObject						*m_pSelectedObject;

	LIGHTS							*m_pLights;
	ID3D11Buffer					*m_pd3dcbLights;

	CHeightMapTerrain				*m_pTerrain;
};