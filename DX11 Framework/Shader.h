//-----------------------------------------------------------------------------
// File: Shader.h
//-----------------------------------------------------------------------------

#pragma once

#include "Object.h"
#include "Camera.h"
#include "Player.h"

#define _WITH_INSTANCING_CULLING
#ifdef _WITH_INSTANCING_CULLING
#define _WITH_INSTANCING_FROM_OBJECTS_DYNAMICALLY
#endif

struct VS_CB_WORLD_MATRIX
{
	XMFLOAT4X4						m_d3dxmtxWorld;
};

struct VS_VB_INSTANCE
{
	XMFLOAT4X4						m_d3dxTransform;
};

class CShader
{
public:
	CShader();
	virtual ~CShader();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

protected:
	UINT							m_nType = 0;
	int								m_nInputElements = 0;
	D3D11_INPUT_ELEMENT_DESC		*m_pd3dInputElementDescs = nullptr;

	ID3D11VertexShader				*m_pd3dVertexShader		= nullptr;
	ID3D11InputLayout				*m_pd3dVertexLayout		= nullptr;
	ID3D11PixelShader				*m_pd3dPixelShader		= nullptr;
	ID3D11ComputeShader				*m_pd3dComputeShader	= nullptr;
	ID3D11GeometryShader			*m_pd3dGeometryShader	= nullptr;
	ID3D11HullShader				*m_pd3dHullShader		= nullptr;
	ID3D11DomainShader				*m_pd3dDomainShader		= nullptr;

public:
	void CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputElements, UINT nElements, ID3D11InputLayout **ppd3dInputLayout);
	void CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader);
	void CreateGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader);
	void CreateComputeShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11ComputeShader **ppd3dComputeShader);
	void CreateHullShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11HullShader **ppd3dHullShader);
	void CreateDomainShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11DomainShader **ppd3dDomainShader);

	virtual void GetInputElementDesc(UINT nVertexElementType);
	virtual void GetShaderName(UINT nVertexElementType, LPCSTR *ppszVSShaderName, LPCSTR *ppszVSShaderModel, LPCSTR *ppszPSShaderName, LPCSTR *ppszPSShaderModel);

	virtual void CreateShader(ID3D11Device *pd3dDevice) { }
	virtual void CreateShader(ID3D11Device *pd3dDevice, UINT nType);
	virtual void CreateShader(ID3D11Device *pd3dDevice, D3D11_INPUT_ELEMENT_DESC *pd3dInputElementDesc, int nInputElements, WCHAR *pszFileName, LPCSTR pszVSShaderName, LPCSTR pszVSShaderModel, LPCSTR pszPSShaderName, LPCSTR pszPSShaderModel);
	virtual void CreateShader(ID3D11Device *pd3dDevice, ShaderTag shaderTag);

	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void ReleaseShaderVariables();

	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
	virtual void OnPostRender(ID3D11DeviceContext *pd3dDeviceContext);

public:
	static ID3D11Buffer *CreateBuffer(ID3D11Device *pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags);
};

////////////////////////////////////////////////////////////////////////////////
class CObjectsShader : public CShader
{
public:
	CObjectsShader(int nObjects = 1);
	virtual ~CObjectsShader();

protected:
	vector<CGameObject*>			m_vObjectsVector;

	CMaterial						*m_pMaterial = nullptr;

	void							*m_pContext = nullptr;

public:
	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = NULL);
	virtual void ReleaseObjects();
	virtual void UpdateObjects(float fTimeElapsed);
	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	void SetObject(int nIndex, CGameObject *pGameObject);
	void AddObject(CGameObject *pGameObject);
	void SetMaterial(CMaterial *pMaterial);
	CGameObject *PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo);
};

class CInstancedObjectsShader : public CObjectsShader
{
public:
	CInstancedObjectsShader(int nObjects = 1);
	virtual ~CInstancedObjectsShader();

	void SetMesh(CMesh *pMesh);

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShader(ID3D11Device *pd3dDevice, UINT nType);

	virtual void BuildObjects(ID3D11Device *pd3dDevice, void *pContext = NULL);
	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

protected:
	CMesh							*m_pMesh = nullptr;

	UINT							m_nInstanceBufferStride = 0;
	UINT							m_nInstanceBufferOffset = 0;

	ID3D11Buffer					*m_pd3dInstanceBuffer = nullptr;
};
