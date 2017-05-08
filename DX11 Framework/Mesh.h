//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------

#pragma once
#include "HeightMap.h"

#define VERTEX_POSITION_ELEMENT			0x0001
#define VERTEX_COLOR_ELEMENT			0x0002
#define VERTEX_NORMAL_ELEMENT			0x0004
#define VERTEX_TANGENT_ELEMENT			0x0008
#define VERTEX_TEXTURE_ELEMENT_0		0x0010
#define VERTEX_TEXTURE_ELEMENT_1		0x0020
#define VERTEX_INSTANCING_ELEMENT		0x0040

#define VERTEX_BONE_ID_ELEMENT			0x0100
#define VERTEX_BONE_WEIGHT_ELEMENT		0x0200

#define TEXTURE_ELEMENT_0_TRANSFORM		0x0400
#define TEXTURE_ELEMENT_1_TRANSFORM		0x0800

#define VERTEX_BLENDING_SHADER			0x1000

class CGameObject;

struct CollisionInfo
{
	CGameObject*		m_pHitObject = nullptr;
	float				m_fDistance = FLT_MAX;
	XMFLOAT3			m_f3HitNormal = XMFLOAT3(0, 0, 0);
	DWORD				m_dwFaceIndex = 0;
	float				m_fU = 0.0f;
	float				m_fV = 0.0f;

	ChracterBoundingBoxParts	m_HitParts = ChracterBoundingBoxParts::eNone;
};

//------------------------------------------------------------------------------------------------
#define EPSILON				1.0e-10f

inline bool IsZero(float fValue) { return((fabsf(fValue) < EPSILON)); }
inline bool IsEqual(float fA, float fB) { return(::IsZero(fA - fB)); }
inline float InverseSqrt(float fValue) { return 1.0f / sqrtf(fValue); }
inline void Swap(float *pfS, float *pfT) { float fTemp = *pfS; *pfS = *pfT; *pfT = fTemp; }

//------------------------------------------------------------------------------------------------
class CMesh
{
public:
	CMesh();
	virtual ~CMesh();

protected:
	MeshTag							m_tagMesh = MeshTag::eNone;
	UINT							m_nType = 0;

	BoundingBox						m_bcBoundingBox;
	D3D11_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology;
	ID3D11Buffer					*m_pd3dPositionBuffer = nullptr;
	ID3D11Buffer					**m_ppd3dVertexBuffers = nullptr;
	ID3D11Buffer					*m_pd3dIndexBuffer = nullptr;

	int								m_nBuffers = 0;
	int								m_nVertices = 0;
	UINT							m_nStartVertex = 0;
	UINT							m_nSlot = 0;

	UINT							*m_pnVertexStrides = 0;
	UINT							*m_pnVertexOffsets  = 0 ;


	UINT							m_nIndices = 0;
	UINT							m_nStartIndex = 0;
	int								m_nBaseVertex = 0;

	DXGI_FORMAT						m_dxgiIndexFormat;
	UINT							m_nIndexOffset = 0;

	XMFLOAT3						*m_pPositions = nullptr;
	UINT							*m_pnIndices = nullptr;

	//------------------------------------------------------
	UINT							m_nStride = 0;
	UINT							m_nOffset = 0;
	ID3D11Buffer					*m_pd3dVertexBuffer = nullptr;
	//------------------------------------------------------

public:	
	void CalculateBoundingCube();

	ID3D11Buffer *CreateBuffer(ID3D11Device *pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags);
	void AssembleToVertexBuffer(int nBuffers = 0, ID3D11Buffer **m_pd3dBuffers = NULL, UINT *pnBufferStrides = NULL, UINT *pnBufferOffsets = NULL);

	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);

	int CheckRayIntersection(XMVECTOR *pd3dxvRayPosition, XMVECTOR *pd3dxvRayDirection, CollisionInfo *pd3dxIntersectInfo);

	// ----- Get, Setter ----- //
	MeshTag	GetMeshTag() const { return m_tagMesh; }
	UINT GetType() { return(m_nType); }
	BoundingBox GetBoundingCube() { return(m_bcBoundingBox); }
	
	void SetStartIndex(UINT index) { m_nStartIndex = index; }
};

//------------------------------------------------------------------------------------------------
class CMeshDiffused : public CMesh
{
public:
	CMeshDiffused();
	virtual ~CMeshDiffused();

protected:
	ID3D11Buffer					*m_pd3dColorBuffer;
};

//------------------------------------------------------------------------------------------------
class CMeshIlluminated : public CMesh
{
public:
	CMeshIlluminated();
	virtual ~CMeshIlluminated();

protected:
	ID3D11Buffer					*m_pd3dNormalBuffer;
public:
	XMVECTOR CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2);
	void SetTriAngleListVertexNormal(XMVECTOR *pd3dxvNormals);
	void SetAverageVertexNormal(XMVECTOR *pd3dxvNormals, int nPrimitives, int nOffset, bool bStrip);
	void CalculateVertexNormal(XMVECTOR *pd3dxvNormals);
};

//------------------------------------------------------------------------------------------------
class CMeshTextured : public CMesh
{
public:
	CMeshTextured();
	virtual ~CMeshTextured();

protected:
	ID3D11Buffer					*m_pd3dTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CMeshDetailTextured : public CMeshTextured
{
public:
	CMeshDetailTextured();
	virtual ~CMeshDetailTextured();

protected:
	ID3D11Buffer					*m_pd3dDetailTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CMeshTexturedIlluminated : public CMeshIlluminated
{
public:
	CMeshTexturedIlluminated();
	virtual ~CMeshTexturedIlluminated();

protected:
	ID3D11Buffer					*m_pd3dTexCoordBuffer;
};

class CMeshNormalMap : public CMeshTexturedIlluminated
{
public:
	CMeshNormalMap(ID3D11Device *pd3dDevice);
	virtual ~CMeshNormalMap();

	XMVECTOR CalculateTriAngleTangent(UINT nIndex0, UINT nIndex1, UINT nIndex2);

	void SetTriAngleListVertexTangent(XMVECTOR *pd3dxvTangents);
	void SetAverageVertexTangent(XMVECTOR *pd3dxvTangents, int nPrimitives, int nOffset, bool bStrip);
	void CalculateVertexTangent(XMVECTOR *pd3dxvTangents);

protected:
	XMFLOAT2				*m_pTexCoords = nullptr;
	XMFLOAT3				*m_pNormals = nullptr;
	XMFLOAT3				*m_pTangents = nullptr;

	ID3D11Buffer			*m_pd3dTangentBuffer = nullptr;
};


//------------------------------------------------------------------------------------------------
class CMeshDetailTexturedIlluminated : public CMeshIlluminated
{
public:
	CMeshDetailTexturedIlluminated();
	virtual ~CMeshDetailTexturedIlluminated();

protected:
	ID3D11Buffer					*m_pd3dTexCoordBuffer;
	ID3D11Buffer					*m_pd3dDetailTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CCubeMeshDiffused : public CMeshDiffused
{
public:
	CCubeMeshDiffused(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f, XMVECTOR d3dxColor = XMVectorSet(0.8f, 0.8f, 1.0f, 0.0f));
	virtual ~CCubeMeshDiffused();
};

//------------------------------------------------------------------------------------------------
class CSphereMeshDiffused : public CMeshDiffused
{
public:
	CSphereMeshDiffused(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20, XMVECTOR d3dxColor = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CSphereMeshDiffused();
};

//------------------------------------------------------------------------------------------------
class CCubeMeshIlluminated : public CMeshIlluminated
{
public:
	CCubeMeshIlluminated(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshIlluminated();
};

class CSphereMeshIlluminated : public CMeshIlluminated
{
public:
	CSphereMeshIlluminated(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshIlluminated();
};

class CCubeMeshTextured : public CMeshTextured
{
public:
	CCubeMeshTextured(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshTextured();
};

class CSphereMeshTextured : public CMeshTextured
{
public:
	CSphereMeshTextured(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshTextured();
};

class CCubeMeshTexturedIlluminated : public CMeshTexturedIlluminated
{
public:
	CCubeMeshTexturedIlluminated(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshTexturedIlluminated();
};

class CSphereMeshTexturedIlluminated : public CMeshTexturedIlluminated
{
public:
	CSphereMeshTexturedIlluminated(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshTexturedIlluminated();
};
