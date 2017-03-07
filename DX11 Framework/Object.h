//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"

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

enum Resource_TextrueTag;

class CTexture
{
public:
	CTexture(int nTextures = 1, int nSamplers = 1, int nTextureStartSlot = 0, int nSamplerStartSlot = 0);
	virtual ~CTexture();

private:
	int								m_nReferences = 0;

	int								m_nTextures = 0;
	ID3D11ShaderResourceView		**m_ppd3dsrvTextures = nullptr;
	int								m_nTextureStartSlot = 0;

	int								m_nSamplers = 0;
	ID3D11SamplerState				**m_ppd3dSamplerStates = nullptr;
	int								m_nSamplerStartSlot = 0;

	Resource_TextrueTag				m_Tag;
public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture);
	void SetTexture(int nIndex, Resource_TextrueTag);

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

class CShader;

class CGameObject
{
public:
	CGameObject(int nMeshes = 0);
	virtual ~CGameObject();
	virtual void CreateObjectData(ID3D11Device *pd3dDevice);

protected:
	virtual void CreateMesh(ID3D11Device *pd3dDevice)		{ print("No have Mesh"); };
	virtual void CreateShader(ID3D11Device *pd3dDevice)		{ print("No have Shader"); };
	virtual void CreateMaterial(ID3D11Device *pd3dDevice)	{ print("No have Material"); };

private:
	int								m_nReferences = 0;

	bool							m_bActive = true;
	bool							m_bIsVisible = true;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	XMFLOAT4X4						m_d3dxmtxLocal;
	XMFLOAT4X4						m_d3dxmtxWorld;
	XMFLOAT4X4						m_d3dxmtxShadow;

protected:
	static UINT						m_iObjectId;

	CMesh							**m_ppMeshes = nullptr;
	int								m_nMeshes = 0;

	CMaterial						*m_pMaterial = nullptr;
	CShader							*m_pShader = nullptr;

	CGameObject 					*m_pChild = nullptr;
	CGameObject 					*m_pSibling = nullptr;
	CGameObject 					*m_pParent = nullptr;

	BoundingBox						m_bcBoundingBox;			// 사용자가 정하는 바운딩 박스
	BoundingBox						m_bcMeshBoundingBox;		// 실제 메쉬 바운딩 박스
	CMesh							*m_pBoundingBoxMesh	= nullptr;
	
public:
	ID3D11DepthStencilState			*m_pd3dDepthStencilState = nullptr;
	ID3D11BlendState				*m_pd3dBlendState = nullptr;

public:
	void SetMesh(CMesh *pMesh, int nIndex = 0);
	CMesh *GetMesh(int nIndex = 0) { return(m_ppMeshes[nIndex]); }
	UINT GetMeshType() { return((m_ppMeshes) ? m_ppMeshes[0]->GetType() : 0x00); }

	void SetShader(CShader *pShader);
	CShader *GetShader() { return(m_pShader); }

	void SetShadowMatrix(XMMATRIX *pd3dxmtxShadow) { XMStoreFloat4x4(&m_d3dxmtxShadow, *pd3dxmtxShadow); }
	void SetShadowMatrix(XMVECTOR d3dxvLight, XMVECTOR d3dxPlane);

	void SetMaterial(CMaterial *pMaterial);
	CMaterial *GetMaterial() { return(m_pMaterial); }

	void SetChild(CGameObject *pChild);
	CGameObject *GetParent() { return(m_pParent); }

	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(XMVECTOR d3dxvPosition);
	virtual void SetPosition(XMFLOAT3 d3dxvPosition);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMVECTOR *pd3dxvAxis, float fAngle);

	XMVECTOR GetvPosition(bool bIsLocal = true) const;
	XMFLOAT3 GetPosition() const;
	XMVECTOR GetLook(bool bIsLocal = true);
	XMVECTOR GetUp(bool bIsLocal = true);
	XMVECTOR GetRight(bool bIsLocal = true);

	bool IsVisible(CCamera *pCamera = NULL);

	void Animate(XMMATRIX *pd3dxmtxParent);

	virtual void Update(float fTimeElapsed);
	virtual void Update(float fTimeElapsed, XMMATRIX *pd3dxmtxParent);
	virtual void OnPrepareRender();
	virtual void RenderMesh(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	void SetActive(bool bActive = false) { m_bActive = bActive; }
	void GenerateRayForPicking(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxWorld, XMMATRIX *pd3dxmtxView, XMVECTOR *pd3dxvPickRayPosition, XMVECTOR *pd3dxvPickRayDirection);
	int PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo);

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
