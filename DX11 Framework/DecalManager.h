#pragma once
#include "SingletonManager.h"

struct TDecalGenCB
{
	XMFLOAT4X4	arrClipPlanes[6];
	XMFLOAT2	vDecalSize;
	UINT		pad0[2];
	XMFLOAT3	vHitRayNorm;
	UINT		pad1;
};

struct TDecalAddEntry
{
	XMFLOAT3 vHitPos;
	XMFLOAT3 vHitNorm;
	CMesh* pMesh;
	UINT nMesh;
	UINT nSubMesh;
	bool bStarted;
};

struct TDecalEntry
{
	XMFLOAT3 vHitPos;
	UINT nVertCount;
};

/*
class CDecalManager : public CSingletonManager<CDecalManager >
{
public:
	CDecalManager() {};
	virtual ~CDecalManager() {};

	virtual void InitializeManager() override;
	virtual void UpdateManager() override;
	virtual void ReleseManager() override;

	void AddDecal(const D3DXVECTOR3& vHitPos, const D3DXVECTOR3& vHitNorm, CMesh& mesh, UINT nMesh, UINT nSubMesh);
	void PreRender(ID3D11DeviceContext* pd3dImmediateContext);
	void Render(ID3D11DeviceContext* pd3dImmediateContext, bool bWireframe);

	void SetDecalScale(float fDecalScale) { m_fDecalSize = 0.025f + fDecalScale * 0.075f; }

private:
	void CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, const wstring& wstring, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dSOGeometryShader);
	void PrepareGenConstBuffer(ID3D11DeviceContext* pd3dImmediateContext, const D3DXVECTOR3& pos, const D3DXVECTOR3& norm);
	void AddDecalToVB(ID3D11DeviceContext* pd3dImmediateContext, CMesh& mesh, UINT nMesh, UINT nSubMesh, bool bOverwrite);
	void RemoveDecalFromList();

	ID3D11Buffer* m_pDecalGenVSCB		= nullptr;
	ID3D11Buffer* m_pDecalGenGSCB		= nullptr;
	ID3D11VertexShader* m_pDecalGenVS	= nullptr;
	ID3D11GeometryShader* m_pDecalGenGS = nullptr;

	ID3D11Buffer* m_pDecalRenderCB		= nullptr;
	ID3D11VertexShader* m_pDecalRenderVS = nullptr;
	ID3D11PixelShader* m_pDecalRenderPS = nullptr;
	ID3D11ShaderResourceView* m_pDecalTexView = nullptr;
	ID3D11ShaderResourceView* m_pWhiteTexView = nullptr;
	ID3D11Buffer* m_pDecalVB			= nullptr;
	ID3D11Query* m_pStatsQuery			= nullptr;

//	ID3D11BlendState* m_pDecalRenderBS;
//	ID3D11RasterizerState* m_pRSDecalSolid;
//	ID3D11RasterizerState* m_pRSDecalWire;
//	ID3D11DepthStencilState* m_pDecalGenDepthStencilState;


	list<TDecalAddEntry> m_DecalAddList;
	float m_fDecalSize					= 0.75f;
	UINT m_nDecalStart1					= 0;
	UINT m_nTotalDecalVerts1			= 0;
	UINT m_nTotalDecalVerts2			= 0;

	static const UINT m_nVertStride;
	static const UINT m_nMaxDecalVerts;
};
*/

class CDecalManager : public CSingletonManager<CDecalManager >
{
public:
	CDecalManager() {};
	virtual ~CDecalManager() {};

	virtual void InitializeManager() {}
	virtual void UpdateManager() {}
	virtual void ReleseManager() {}

	void AddDecal(const XMVECTOR& vHitPos, const XMVECTOR& vHitNorm, CMesh& mesh, UINT nMesh, UINT nSubMesh) {};
	void PreRender(ID3D11DeviceContext* pd3dImmediateContext) {};
	void Render(ID3D11DeviceContext* pd3dImmediateContext, bool bWireframe) {};

	void SetDecalScale(float fDecalScale) { m_fDecalSize = 0.025f + fDecalScale * 0.075f; }

private:
	void CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, const wstring& wstring, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dSOGeometryShader) {}
	void PrepareGenConstBuffer(ID3D11DeviceContext* pd3dImmediateContext, const D3DXVECTOR3& pos, const D3DXVECTOR3& norm) {}
	void AddDecalToVB(ID3D11DeviceContext* pd3dImmediateContext, CMesh& mesh, UINT nMesh, UINT nSubMesh, bool bOverwrite) {}
	void RemoveDecalFromList() {};

	ID3D11Buffer*			m_pDecalGenVSCB = nullptr;
	ID3D11Buffer*			m_pDecalGenGSCB = nullptr;
	ID3D11VertexShader*		m_pDecalGenVS = nullptr;
	ID3D11GeometryShader*	m_pDecalGenGS = nullptr;

	ID3D11Buffer*			m_pDecalRenderCB = nullptr;
	ID3D11VertexShader*		m_pDecalRenderVS = nullptr;
	ID3D11PixelShader*		m_pDecalRenderPS = nullptr;
	ID3D11Buffer*			m_pDecalVB = nullptr;
	ID3D11Query*			m_pStatsQuery = nullptr;

	float					m_fDecalSize = 0.75f;
	UINT					m_nDecalStart1 = 0;
	UINT					m_nTotalDecalVerts1 = 0;
	UINT					m_nTotalDecalVerts2 = 0;

	list<TDecalEntry>		m_DecalList;
	list<TDecalAddEntry>	m_DecalAddList;

	static const UINT m_nVertStride;
	static const UINT m_nMaxDecalVerts;
};