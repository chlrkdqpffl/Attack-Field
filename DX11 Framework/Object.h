//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"

class CShader;
class CBoundingBoxMesh;
class CBoundingBoxShader;
class CAxisObjects;
class CGameObject;

class CMaterialColors
{
public:
	CMaterialColors();
	virtual ~CMaterialColors();

private:
	int								m_nReferences;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	XMFLOAT4						m_d3dxcAmbient;
	XMFLOAT4						m_d3dxcDiffuse;
	XMFLOAT4						m_d3dxcSpecular; //(r,g,b,a=power)
	XMFLOAT4						m_d3dxcEmissive;
};

class CTexture
{
public:
	CTexture(int nTextures = 1, int nSamplers = 1, int nTextureStartSlot = 0, int nSamplerStartSlot = 0);
	virtual ~CTexture();

private:
	TextureTag						m_textureTag;
	int								m_nReferences = 0;

	int								m_nTextures = 0;
	ID3D11ShaderResourceView		**m_ppd3dsrvTextures = nullptr;
	int								m_nTextureStartSlot = 0;

	int								m_nSamplers = 0;
	ID3D11SamplerState				**m_ppd3dSamplerStates = nullptr;
	int								m_nSamplerStartSlot = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture);
	void SetTexture(int nIndex, TextureTag);

	void SetSampler(int nIndex, ID3D11SamplerState *pd3dSamplerState);
	void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext);
	void UpdateTextureShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, int nIndex = 0, int nSlot = 0);
	void UpdateSamplerShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, int nIndex = 0, int nSlot = 0);

	static ID3D11Buffer				*m_pd3dcbTextureMatrix;

	static void CreateShaderVariables(ID3D11Device *pd3dDevice);
	static void ReleaseShaderVariables();
	static void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxTexture);
	static ID3D11ShaderResourceView *CreateTexture2DArraySRV(ID3D11Device *pd3dDevice, _TCHAR(*ppstrFilePaths)[128], UINT nTextures);
};

class CMaterial
{
public:
	CMaterial(CMaterialColors *pColors = nullptr);
	virtual ~CMaterial();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void SetTexture(CTexture *pTexture);
	void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext);

	CMaterialColors					*m_pColors = nullptr;
	CTexture						*m_pTexture = nullptr;
};

class CollisionInfo {
	CGameObject*	m_pHitObject = nullptr;
	float			m_fDistance = 0.0f;
	XMFLOAT3		m_f3HitNormal;

public:
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
	
private:
	int								m_nReferences = 0;

	bool							m_bActive = true;
	bool							m_bIsVisible = true;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	XMMATRIX						m_mtxLocal = XMMatrixIdentity();
	XMMATRIX						m_mtxWorld = XMMatrixIdentity();
	XMMATRIX						m_mtxShadow = XMMatrixIdentity();

protected:
	static UINT						m_iObjectId;
	MeshTag							m_tagMesh = MeshTag::eNone;
	TeamType						m_tagTeam = TeamType::eNone;		// 팀 종류 구별하여 충돌 처리시 사용하기. 현재는 아무것도 없음

	CMesh							**m_ppMeshes = nullptr;
	int								m_nMeshes = 0;

	CMaterial						*m_pMaterial = nullptr;
	CShader							*m_pShader = nullptr;

	CGameObject 					*m_pChild = nullptr;
	CGameObject 					*m_pSibling = nullptr;
	CGameObject 					*m_pParent = nullptr;
	CAxisObjects					*m_pAxisObject = nullptr;

	
	// Collision Info
	bool							m_bIsCollision = false;
//	BoundingBox						m_bcBoundingBox;			// 사용자가 정하는 바운딩 박스
	BoundingBox						m_bcMeshBoundingBox;		// 실제 메쉬 바운딩 박스
	BoundingOrientedBox				m_bcMeshBoundingOBox;		// 테스트용 OBB 박스

	CBoundingBoxMesh				*m_pBoundingBoxMesh	= nullptr;
	CBoundingBoxShader				*m_pBoundingBoxShader = nullptr;
	


public:
	ID3D11DepthStencilState			*m_pd3dDepthStencilState = nullptr;
	ID3D11BlendState				*m_pd3dBlendState = nullptr;

public:
	virtual void Update(float fTimeElapsed);
	virtual void Update(float fTimeElapsed, XMMATRIX *pd3dxmtxParent);
	virtual void OnPrepareRender();
	virtual void RenderMesh(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
	void BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext);

	void CreateAxisObject(ID3D11Device *pd3dDevice);
	void CreateBoundingBox(ID3D11Device *pd3dDevice);

	bool IsVisible(CCamera *pCamera = NULL);
	bool IsCollision(CGameObject* pObject);

	void Animate(XMMATRIX *pd3dxmtxParent);
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
	void SetMesh(CMesh *pMesh, int nIndex = 0);
	CMesh *GetMesh(int nIndex = 0) { return(m_ppMeshes[nIndex]); }
	UINT GetMeshType() { return((m_ppMeshes) ? m_ppMeshes[0]->GetType() : 0x00); }

	void SetActive(bool bActive = false) { m_bActive = bActive; }
	bool GetActive() const { return m_bActive; }

	void SetShader(CShader *pShader);
	CShader *GetShader() { return(m_pShader); }

	void SetShadowMatrix(XMMATRIX *pd3dxmtxShadow) { m_mtxShadow = *pd3dxmtxShadow; }
	void SetShadowMatrix(XMVECTOR d3dxvLight, XMVECTOR d3dxPlane);

	void SetMaterial(CMaterial *pMaterial);
	CMaterial *GetMaterial() { return(m_pMaterial); }

	void SetChild(CGameObject *pChild);
	CGameObject *GetParent() { return(m_pParent); }

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

	void SetMeshTag(MeshTag tag) { m_tagMesh = tag; }
	MeshTag GetMeshTag() const { return m_tagMesh; }
	BoundingBox GetBoundingBox() const { return m_bcMeshBoundingBox; }
	bool GetCollisionCheck() const { return m_bIsCollision; }
	void SetCollision(bool collision) { m_bIsCollision = collision; }

public:
	static ID3D11Buffer				*m_pd3dcbWorldMatrix;
	static ID3D11Buffer				*m_pd3dcbMaterialColors;

public:
	static void CreateConstantBuffers(ID3D11Device *pd3dDevice);
	static void ReleaseConstantBuffers();
	static void UpdateConstantBuffer_WorldMtx(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxWorld);
	static void UpdateConstantBuffer_Material(ID3D11DeviceContext *pd3dDeviceContext, CMaterialColors *pMaterialColors);

	virtual void CreateDepthStencilState(ID3D11Device *pd3dDevice) { }
	virtual void CreateBlendState(ID3D11Device *pd3dDevice) { }
};
