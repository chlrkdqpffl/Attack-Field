#pragma once

#include "Camera.h"
#include "Shader.h"

// ========== Mesh ========== 
#include "FbxModelMesh.h"
#include "FbxModelSkinnedMesh.h"
#include "TexturedRectMesh.h"

// ========== Shader ========== 
#include "SkyBoxShader.h"
#include "CharacterShader.h"

// ========== Object ========== 
#include "SkyBox.h"
#include "RotatingObject.h"
#include "NormalMapObject.h"
#include "AxisObjects.h"
#include "UIObject.h"
#include "BillboardObject.h"
#include "SphereObject.h"
#include "SpriteImageObject.h"
#include "PhysXObject.h"
#include "AimObject.h"

// ----- Character -----
#include "CharacterPlayer.h"
#include "TerroristPlayer.h"
#include "TerrainPlayer.h"
#include "PoliceCharacterObject.h"
#include "TerroristCharacterObject.h"

// ========== System ========== 
#include "ParticleSystem.h"


#define MAX_LIGHTS			4 //Multiple of 4
#define POINT_LIGHT			1.0f
#define SPOT_LIGHT			2.0f
#define DIRECTIONAL_LIGHT	3.0f

class CScene
{
public:
	CScene() {};
	virtual ~CScene() {};

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex = 0) {};
	virtual void OnChangedWindowsSize(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {};

	virtual void Initialize() = 0;
	virtual void ReleaseObjects();

	virtual void CreateConstantBuffers() {};
	virtual void UpdateConstantBuffers() {};
	virtual void ReleaseConstantBuffers() {};

	virtual void CreateLights() {};
	virtual void CreateUIImage() {};
	virtual void CreateTweakBars() {};
	virtual void CreatePlayer();
	virtual void IsCollisionUI(POINT mousePos , HWND hwnd) {};

	virtual void Update(float fDeltaTime);

	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera);
	virtual void RenderAllText(ID3D11DeviceContext *pd3dDeviceContext) {};

	void InitializePhysX();
	void ReleasePhysX();

public:
	CGameObject *PickObjectPointedByCursor(int xClient, int yClient);
	XMFLOAT2 ObjectPositionConvertToScreen(XMFLOAT3 d3dxvObjPos);

	// ----- Get, Setter ----- // 
	void SetResizeRatio(float x, float y) { m_fResizeRatioX = x, m_fResizeRatioY = y; }
	
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* GetPlayer() const { return m_pPlayer; }
	void SetDevice(ID3D11Device* pDevice) { m_pd3dDevice = pDevice; }
	void SetDeviceContext(ID3D11DeviceContext* pDeviceContext) { m_pd3dDeviceContext = pDeviceContext; }
	void SetTag(SceneTag tag) { m_tagScene = tag; }
	SceneTag GetSceneTag() const { return m_tagScene; }
	float GetFrameSpeed() const { return m_fFrameSpeed; }

protected:
	SceneTag							m_tagScene = SceneTag::eNone;
	ID3D11Device						*m_pd3dDevice = nullptr;
	ID3D11DeviceContext					*m_pd3dDeviceContext = nullptr;
	CCamera								*m_pCamera = nullptr;

	CGameObject							*m_pSelectedObject = nullptr;
	CAxisObjects						*m_pWorldCenterAxis = nullptr;

	// ResizeRatio
	float								m_fResizeRatioX = 1.0f;
	float								m_fResizeRatioY = 1.0f;

	// Environment
	CSkyBox								*m_pSkyBox		= nullptr;

	float								m_fGametime		= 0.0f;
	float								m_fDeltaTime	= 0.0f;
	CPlayer								*m_pPlayer		= nullptr;
//	CCharacterObject					*m_pPlayerCharacter = nullptr;
	CCharacterPlayer					*m_pPlayerCharacter = nullptr;

	// UI
	CUIManager							*m_pUIManager = nullptr;

	vector<CGameObject*>				m_vecObjectsContainer;
	vector<CInstancedObjectsShader*>	m_vecInstancedObjectsShaderContainer;
	CObjectsShader						m_vecShaderObjectContainer;
	
	CSphereObject*						m_pSphereObject = nullptr;
	float								m_fFrameSpeed = 1.0f;

	// PhysX
	PxPhysics*							m_pPxPhysicsSDK;
	PxScene*							m_pPxScene;
	PxControllerManager*				m_pPxControllerManager;
	PxFoundation*						m_pPxFoundation;
	PxDefaultErrorCallback				m_PxDefaultErrorCallback;
	PxDefaultAllocator					m_PxDefaultAllocatorCallback;
	PxVisualDebuggerConnection*			m_pPxPVDConnection;
	PxCooking*							m_pPxCooking;
	PxMaterial*							m_pPxMaterial;
};