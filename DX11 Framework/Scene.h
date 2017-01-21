#pragma once

#include "Camera.h"
#include "Shader.h"

// Mesh
#include "ModelMesh_FBX.h"
#include "ModelSkinnedMesh.h"

// Shader
#include "TerrainShader.h"

// Object
#include "SkyBox.h"
#include "HeightMapTerrain.h"
#include "TerrainWater.h"
#include "RotatingObject.h"
#include "NormalMapObject.h"
#include "AxisObjects.h"

// System
#include "ParticleSystem.h"

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
//	vector<LIGHT>			m_vLightVector;
	LIGHT					m_pLights[MAX_LIGHTS];
	XMFLOAT4				m_d3dxcGlobalAmbient;
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
	virtual void CreateTweakBars();
	virtual void ReleaseObjects();

	virtual void CreateConstantBuffers(ID3D11Device *pd3dDevice);
	virtual void UpdateConstantBuffers(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void ReleaseConstantBuffers();

	virtual void CreateLights() = 0;

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

	vector<CGameObject*>				m_vObjectsVector;
	vector<CObjectsShader*>				m_vObjectsShaderVector;
	vector<CInstancedObjectsShader*>	m_vInstancedObjectsShaderVector;

	CPlayer							*m_pPlayer;
	CCamera							*m_pCamera;
	CGameObject						*m_pSelectedObject;

	// Light
	LIGHTS							*m_pLights;

	// Environment
	CSkyBox							*m_pSkyBox			= nullptr;
	CHeightMapTerrain				*m_pTerrain			= nullptr;

	// Particle
	float							m_fGametime;
	CParticleSystem					*m_pParticleSystem;

	// RenderOption
	CAxisObjects					*m_pAxisObjects		= nullptr;
	bool							m_bShowRGBAxis		= false;
	XMFLOAT4						m_vRenderOption;		// (x : Fog )

	// Constant Buffer
	ID3D11Buffer					*m_pd3dcbRenderOption;
};