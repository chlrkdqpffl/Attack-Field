#pragma once

#include "Camera.h"
#include "Shader.h"

// ========== Mesh ========== 
#include "FbxModelMesh.h"
#include "FbxModelSkinnedMesh.h"

// ========== Shader ========== 
#include "SkyBoxShader.h"
#include "WaterShader.h"
#include "TerrainShader.h"
#include "CharacterShader.h"

// ========== Object ========== 
#include "SkyBox.h"
#include "HeightMapTerrain.h"
#include "TerrainWater.h"
#include "RotatingObject.h"
#include "NormalMapObject.h"
#include "AxisObjects.h"
#include "UIObject.h"

// ----- Character -----
#include "TerrainPlayer.h"
#include "PoliceCharacterObject.h"
#include "TerroristCharacterObject.h"

// ========== System ========== 
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

	virtual void Initialize();
	virtual void ReleaseObjects();

	virtual void CreateConstantBuffers() {};
	virtual void UpdateConstantBuffers() {};
	virtual void ReleaseConstantBuffers() {};

	virtual void CreateLights() {};
	virtual void CreateUIImage() {};
	virtual void CreateTweakBars() {};
	virtual void CreatePlayer();
	virtual void IsCollisionUI(POINT mousePos) {};

	virtual bool ProcessInput(UCHAR *pKeysBuffer);
	virtual void UpdateObjects(float fTimeElapsed);

	virtual void OnPreRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera);
	virtual void RenderAllText(ID3D11DeviceContext *pd3dDeviceContext) {};

public:
	CGameObject *PickObjectPointedByCursor(int xClient, int yClient);

	// ----- Get, Setter ----- // 
	CHeightMapTerrain *GetTerrain() const {	return m_pTerrain; }
	void SetResizeRatio(float x, float y) { m_fResizeRatioX = x, m_fResizeRatioY = y; }
	
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* GetPlayer() const { return m_pPlayer; }
	void SetDevice(ID3D11Device* pDevice) { m_pd3dDevice = pDevice; }
	void SetDeviceContext(ID3D11DeviceContext* pDeviceContext) { m_pd3dDeviceContext = pDeviceContext; }
	void SetTag(SceneTag tag) { m_tagScene = tag; }

protected:
	SceneTag							m_tagScene = SceneTag::eNone;
	ID3D11Device						*m_pd3dDevice = nullptr;
	ID3D11DeviceContext					*m_pd3dDeviceContext = nullptr;
	CCamera								*m_pCamera;

	CGameObject							*m_pSelectedObject;
	CAxisObjects						*m_pWorldCenterAxis = nullptr;

	// ResizeRatio
	float								m_fResizeRatioX = 1.0f;
	float								m_fResizeRatioY = 1.0f;

	// Light
	LIGHTS								*m_pLights;

	// Environment
	CSkyBox								*m_pSkyBox			= nullptr;
	CHeightMapTerrain					*m_pTerrain			= nullptr;

	// Particle
	float								m_fGametime;
	CParticleSystem						*m_pParticleSystem;


	float								m_fTimeElapsed = 0.0f;
	CPlayer								*m_pPlayer = nullptr;
	CCharacterObject					*m_pPlayerCharacter = nullptr;

	// UI
	CUIManager							*m_pUIManager = nullptr;

	vector<CGameObject*>				m_vObjectsVector;
	vector<CObjectsShader*>				m_vObjectsShaderVector;
	vector<CInstancedObjectsShader*>	m_vInstancedObjectsShaderVector;

	vector<CGameObject*>				m_vecStaticMeshContainer;
	vector<CGameObject*>				m_vecDynamicMeshContainer;
};