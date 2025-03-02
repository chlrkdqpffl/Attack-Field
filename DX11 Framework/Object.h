#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "Material.h"

class CShader;
class CBoundingBoxMesh;
class CAxisObjects;

class CGameObject
{
public:
	CGameObject(int nMeshes = 0);
	virtual ~CGameObject();
	virtual void CreateObjectData(ID3D11Device *pd3dDevice);

protected:
	virtual void CreateMesh(ID3D11Device *pd3dDevice)		{ cout << "No have Mesh" << endl; };
	virtual void CreateShader(ID3D11Device *pd3dDevice)		{ cout << "No have Shader" << endl; };

public:
	XMMATRIX						m_mtxLocal = XMMatrixIdentity();
	XMMATRIX						m_mtxWorld = XMMatrixIdentity();
	virtual void CreateMaterial()							{ cout << "No have Material" << endl; };

protected:
	// ----- Identity ----- //
	UINT							m_nObjectId = 0;
	MeshTag							m_tagMesh = MeshTag::eNone;
	TextureTag						m_tagTexture = TextureTag::eNone;
	ShaderTag						m_tagShader = ShaderTag::eNone;
	TeamTag							m_tagTeam = TeamTag::eNone;

	bool							m_bActive = true;
	bool							m_bIsVisible = true;

	vector<CMesh*>					m_vecMeshContainer;
	CMaterial						*m_pMaterial = nullptr;
	CShader							*m_pShader = nullptr;
	CAxisObjects					*m_pAxisObject = nullptr;
	
	// ----- Collision ------ //
	bool							m_bIsCollision = false;

	BoundingBox						m_bcMeshBoundingBox;		// 카메라 컬링용 Box
	BoundingSphere					m_bsMeshBoundingSphere;
	BoundingOrientedBox				m_bcMeshBoundingOBox;
	CollisionInfo					m_infoCollision;
	CBoundingBoxMesh				*m_pBoundingBoxMesh	= nullptr;

public:
	ID3D11DepthStencilState			*m_pd3dDepthStencilState = nullptr;
	ID3D11BlendState				*m_pd3dBlendState = nullptr;

public:
	virtual void Update(float fDeltaTime);
	virtual void OnPrepareRender();
	virtual void RenderMesh(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
	virtual void OnCollisionCheck() {};
	virtual void InitCollisionInfo();

	virtual void CreateBoundingBox(ID3D11Device *pd3dDevice);
	virtual void CreateBoundingBox(ID3D11Device *pd3dDevice, CMesh* pMesh);
	virtual void BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext);
	void CreateAxisObject(ID3D11Device *pd3dDevice);

	bool IsVisible(CCamera *pCamera = NULL);
	bool IsCollision(CGameObject* pObject);

	void GenerateRayForPicking(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxWorld, XMMATRIX *pd3dxmtxView, XMVECTOR *pd3dxvPickRayPosition, XMVECTOR *pd3dxvPickRayDirection);
	int PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, CollisionInfo *pd3dxIntersectInfo);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);
	void Move(XMFLOAT3 vPos, bool isLocal = false);
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f, bool isLocal = false);
	void Rotate(XMFLOAT3 fAngle, bool isLocal = false);
	void Rotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal = false);
	// ---------- Get, Setter ---------- //
	void SetMesh(CMesh *pMesh, int nIndex = 0); CMesh *GetMesh(int nIndex = 0) { return m_vecMeshContainer[nIndex]; }
	UINT GetMeshType() { return m_vecMeshContainer[0]->GetType(); }

	void SetActive(bool bActive = false) { m_bActive = bActive; }	bool GetActive() const { return m_bActive; }

	void SetShader(CShader *pShader);
	void SetShader(ID3D11Device *pd3dDevice, ShaderTag tag);
	CShader *GetShader() { return(m_pShader); }

	void SetMaterial(CMaterial *pMaterial);
	void SetMaterial(int textureCount, ...);
	void SetMaterial(XMFLOAT2 offset, int textureCount, ...);
	CMaterial *GetMaterial() { return(m_pMaterial); }

	virtual void SetPosition(float x, float y, float z, bool isLocal = false);
	virtual void SetPosition(XMVECTOR vPosition, bool isLocal = false);
	virtual void SetPosition(XMFLOAT3 vPosition, bool isLocal = false);
	virtual void SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal = false);
	virtual void SetRotate(XMFLOAT3 fAngle, bool isLocal = false);
	virtual void SetRotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal = false);

	XMVECTOR GetvPosition(bool bIsLocal = false) const;
	XMFLOAT3 GetPosition(bool isLocal = false) const;
	XMVECTOR GetvLook(bool bIsLocal = false) const;
	XMVECTOR GetvUp(bool bIsLocal = false) const;
	XMVECTOR GetvRight(bool bIsLocal = false) const;
	XMFLOAT3 GetLook(bool bIsLocal = false) const;
	XMFLOAT3 GetUp(bool bIsLocal = false) const;
	XMFLOAT3 GetRight(bool bIsLocal = false) const;

	void SetLook(XMFLOAT3 axis, bool bIsLocal = false);
	void SetvLook(XMVECTOR axis, bool bIsLocal = false);
	void SetUp(XMFLOAT3 axis, bool bIsLocal = false);
	void SetvUp(XMVECTOR axis, bool bIsLocal = false);
	void SetRight(XMFLOAT3 axis, bool bIsLocal = false);
	void SetvRight(XMVECTOR axis, bool bIsLocal = false);


	void SetShaderTag(ShaderTag tag) { m_tagShader = tag; } ShaderTag GetShaderTag() const { return m_tagShader; }
	void SetMeshTag(MeshTag tag) { m_tagMesh = tag; }		MeshTag GetMeshTag() const { return m_tagMesh; }
	BoundingBox GetBoundingBox(bool isLocal = false) const;
	BoundingSphere GetBoundingSphere(bool isLocal = false) const;
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
