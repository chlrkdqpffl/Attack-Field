#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "Material.h"

class CShader;
class CBoundingBoxMesh;
class CAxisObjects;
class CGameObject;

struct CollisionInfo
{
	CGameObject*	m_pHitObject = nullptr;
	float			m_fDistance = FLT_MAX;
	XMFLOAT3		m_f3HitNormal = XMFLOAT3(0, 0, 0);

};

class CGameObject
{
public:
	CGameObject(int nMeshes = 0);
	virtual ~CGameObject();
	virtual void CreateObjectData(ID3D11Device *pd3dDevice);

protected:
	virtual void CreateMesh(ID3D11Device *pd3dDevice)		{ cout << "No have Mesh" << endl; };
	virtual void CreateShader(ID3D11Device *pd3dDevice)		{ cout << "No have Shader" << endl; };
	virtual void CreateMaterial()							{ cout << "No have Material" << endl; };

public:
	XMMATRIX						m_mtxLocal = XMMatrixIdentity();
	XMMATRIX						m_mtxWorld = XMMatrixIdentity();
	XMMATRIX						m_mtxShadow = XMMatrixIdentity();

protected:
	// ----- Identity ----- //
	UINT							m_nObjectId = 0;
	MeshTag							m_tagMesh = MeshTag::eNone;
	TextureTag						m_tagTexture = TextureTag::eNone;
	ShaderTag						m_tagShader = ShaderTag::eNone;
	TeamType						m_tagTeam = TeamType::eNone;		// 팀 종류 구별하여 충돌 처리시 사용하기. 현재는 아무것도 없음

	bool							m_bActive = true;
	bool							m_bIsVisible = true;

	CMesh							**m_ppMeshes = nullptr;
	int								m_nMeshes = 0;

	CMaterial						*m_pMaterial = nullptr;
	CShader							*m_pShader = nullptr;
	CAxisObjects					*m_pAxisObject = nullptr;
	
	// Collision Info
	bool							m_bIsCollision = false;
//	BoundingBox						m_bcBoundingBox;			// 사용자가 정하는 바운딩 박스
	BoundingBox						m_bcMeshBoundingBox;		// 실제 메쉬 바운딩 박스
	BoundingOrientedBox				m_bcMeshBoundingOBox;		// 테스트용 OBB 박스
	CollisionInfo					m_infoCollision;
	CBoundingBoxMesh				*m_pBoundingBoxMesh	= nullptr;

public:
	ID3D11DepthStencilState			*m_pd3dDepthStencilState = nullptr;
	ID3D11BlendState				*m_pd3dBlendState = nullptr;

public:
	virtual void Update(float fTimeElapsed);
	virtual void OnPrepareRender();
	virtual void RenderMesh(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
	virtual void OnCollisionCheck() {};

	void BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext);

	void CreateAxisObject(ID3D11Device *pd3dDevice);
	void CreateBoundingBox(ID3D11Device *pd3dDevice);

	bool IsVisible(CCamera *pCamera = NULL);
	bool IsCollision(CGameObject* pObject);

	void GenerateRayForPicking(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxWorld, XMMATRIX *pd3dxmtxView, XMVECTOR *pd3dxvPickRayPosition, XMVECTOR *pd3dxvPickRayDirection);
	int PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);
	void Move(XMFLOAT3 vPos, bool isLocal = false);
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f, bool isLocal = false);
	void Rotate(XMFLOAT3 fAngle, bool isLocal = false);
	void Rotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal = false);
	// ---------- Get, Setter ---------- //
	void SetMesh(CMesh *pMesh, int nIndex = 0); CMesh *GetMesh(int nIndex = 0) { return(m_ppMeshes[nIndex]); }
	UINT GetMeshType() { return((m_ppMeshes) ? m_ppMeshes[0]->GetType() : 0x00); }

	void SetActive(bool bActive = false) { m_bActive = bActive; }	bool GetActive() const { return m_bActive; }

	void SetShader(CShader *pShader);
	void SetShader(ID3D11Device *pd3dDevice, ShaderTag tag);
	CShader *GetShader() { return(m_pShader); }

	void SetShadowMatrix(XMMATRIX *pd3dxmtxShadow) { m_mtxShadow = *pd3dxmtxShadow; }
	void SetShadowMatrix(XMVECTOR d3dxvLight, XMVECTOR d3dxPlane);

	void SetMaterial(CMaterial *pMaterial);
	void SetMaterial(int textureCount, TextureTag tag, ...);
	CMaterial *GetMaterial() { return(m_pMaterial); }

	void SetPosition(float x, float y, float z, bool isLocal = false);
	void SetPosition(XMVECTOR d3dxvPosition, bool isLocal = false);
	void SetPosition(XMFLOAT3 d3dxvPosition, bool isLocal = false);
	void SetRotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f, bool isLocal = false);
	void SetRotate(XMFLOAT3 fAngle, bool isLocal = false);
	void SetRotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal = false);
	
	XMVECTOR GetvPosition(bool bIsLocal = false) const;
	XMFLOAT3 GetPosition(bool isLocal = false) const;
	XMVECTOR GetLook(bool bIsLocal = false) const;
	XMVECTOR GetUp(bool bIsLocal = false) const;
	XMVECTOR GetRight(bool bIsLocal = false) const;

	void SetLook(XMFLOAT3 axis, bool bIsLocal = false);
	void SetUp(XMFLOAT3 axis, bool bIsLocal = false);
	void SetRight(XMFLOAT3 axis, bool bIsLocal = false);

	void SetShaderTag(ShaderTag tag) { m_tagShader = tag; } ShaderTag GetShaderTag() const { return m_tagShader; }
	void SetMeshTag(MeshTag tag) { m_tagMesh = tag; }		MeshTag GetMeshTag() const { return m_tagMesh; }
	BoundingBox GetBoundingBox(bool isLocal = false) const;
	BoundingOrientedBox GetBoundingOBox(bool isLocal = false) const;
	bool GetCollisionCheck() const { return m_bIsCollision; }
	void SetCollision(bool collision) { m_bIsCollision = collision; }
	UINT GetObjectID()const { return m_nObjectId; }
	CollisionInfo GetCollisionInfo() const { return m_infoCollision; }

public:
	static ID3D11Buffer				*m_pd3dcbWorldMatrix;
	static ID3D11Buffer				*m_pd3dcbMaterialColors;
	static UINT						g_nObjectId;
public:
	static void CreateConstantBuffers(ID3D11Device *pd3dDevice);
	static void ReleaseConstantBuffers();
	static void UpdateConstantBuffer_WorldMtx(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxWorld);
	static void UpdateConstantBuffer_Material(ID3D11DeviceContext *pd3dDeviceContext, CMaterialColors *pMaterialColors);

	virtual void CreateDepthStencilState(ID3D11Device *pd3dDevice) { }
	virtual void CreateBlendState(ID3D11Device *pd3dDevice) { }
};
