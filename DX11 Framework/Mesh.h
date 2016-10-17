//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------

#pragma once
#include "HeightMap.h"

#define VERTEX_POSITION_ELEMENT			0x0001
#define VERTEX_COLOR_ELEMENT			0x0002
#define VERTEX_NORMAL_ELEMENT			0x0004
#define VERTEX_TEXTURE_ELEMENT_0		0x0008
#define VERTEX_TEXTURE_ELEMENT_1		0x0010

#define TEXTURE_ELEMENT_0_TRANSFORM		0x0100
#define TEXTURE_ELEMENT_1_TRANSFORM		0x0200

#define VERTEX_INSTANCING_ELEMENT		0x0020

#define VERTEX_BONE_ID_ELEMENT			0x0400
#define VERTEX_BONE_WEIGHT_ELEMENT		0x0800

#define VERTEX_BLENDING_SHADER			0x1000
#define INSTANCING_SHADER				0x2000

struct MESHINTERSECTINFO {
	DWORD	m_dwFaceIndex;
	float	m_fU;
	float	m_fV;
	float	m_fDistance;
};

//------------------------------------------------------------------------------------------------
#define EPSILON				1.0e-10f

inline bool IsZero(float fValue) { return((fabsf(fValue) < EPSILON)); }
inline bool IsEqual(float fA, float fB) { return(::IsZero(fA - fB)); }
inline float InverseSqrt(float fValue) { return 1.0f / sqrtf(fValue); }
inline void Swap(float *pfS, float *pfT) { float fTemp = *pfS; *pfS = *pfT; *pfT = fTemp; }

extern bool RayIntersectTriangle(XMVECTOR *pd3dxvOrigin, XMVECTOR *pd3dxvDirection, XMVECTOR *pd3dxvP0, XMVECTOR *pd3dxvP1, XMVECTOR *pd3dxvP2, float *pfU, float *pfV, float *pfRayToTriangle);

//------------------------------------------------------------------------------------------------
class CMesh
{
public:
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();

private:
	int								m_nReferences;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

protected:
	
	BoundingBox						m_bcBoundingCube;
	D3D11_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology;
	UINT							m_nType;
	ID3D11Buffer					*m_pd3dPositionBuffer;

	/*인스턴싱을 위한 정점 버퍼는 메쉬의 정점 데이터와
	인스턴싱 데이터(객체의 위치와 방향)를 갖는다.
	그러므로 인스턴싱을 위한 정점 버퍼는
	하나가 아니라 버퍼들의 배열이다.
	정점의 요소들을 나타내는 버퍼들을
	입력조립기에 전달하기 위한 버퍼이다.*/
	ID3D11Buffer					**m_ppd3dVertexBuffers;

	//정점을 조립하기 위해 필요한 버퍼의 개수이다. 
	int								m_nBuffers;

	//정점의 개수이다. 
	int								m_nVertices;
	UINT							m_nStartVertex;

	//버퍼들을 입력조립기에 연결하기 위한 시작 슬롯 번호이다. 
	UINT							m_nSlot;

	//정점의 요소들을 나타내는 버퍼들의 원소의 바이트 수를 나타내는 배열이다. 
	UINT							*m_pnVertexStrides;

	//정점의 요소들을 나타내는 버퍼들의 시작 위치(바이트 수)를 나타내는 배열이다. 
	UINT							*m_pnVertexOffsets;

	//인덱스 버퍼(인덱스의 배열)에 대한 인터페이스 포인터이다. 
	ID3D11Buffer					*m_pd3dIndexBuffer;

	//인덱스 버퍼가 포함하는 인덱스의 개수이다. 
	UINT							m_nIndices;

	//인덱스 버퍼에서 메쉬를 표현하기 위해 사용되는 시작 인덱스이다. 
	UINT							m_nStartIndex;

	//각 인덱스에 더해질 인덱스이다. 
	int								m_nBaseVertex;

	//각 인덱스의 형식(DXGI_FORMAT_R32_UINT 또는 DXGI_FORMAT_R16_UINT)이다. 
	DXGI_FORMAT						m_dxgiIndexFormat;
	UINT							m_nIndexOffset;

	XMFLOAT3						*m_pd3dxvPositions;
	UINT							*m_pnIndices;

	//-------------------------------------------------알파블렌딩
	ID3D11BlendState				*m_pd3dBlendState;
	ID3D11RasterizerState			*m_pd3dRasterizerState;

	//------------------------------------------------------
	UINT		  m_nStride;
	UINT		  m_nOffset;
	ID3D11Buffer *m_pd3dVertexBuffer;
	//------------------------------------------------------

public:
	UINT GetType() { return(m_nType); }
	BoundingBox GetBoundingCube() { return(m_bcBoundingCube); }
	
	void CalculateBoundingCube();

	ID3D11Buffer *CreateBuffer(ID3D11Device *pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags);
	void AssembleToVertexBuffer(int nBuffers = 0, ID3D11Buffer **m_pd3dBuffers = NULL, UINT *pnBufferStrides = NULL, UINT *pnBufferOffsets = NULL);

	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);

	int CheckRayIntersection(XMVECTOR *pd3dxvRayPosition, XMVECTOR *pd3dxvRayDirection, MESHINTERSECTINFO *pd3dxIntersectInfo);
};

//------------------------------------------------------------------------------------------------
class CMeshDiffused : public CMesh
{
public:
	CMeshDiffused(ID3D11Device *pd3dDevice);
	virtual ~CMeshDiffused();

protected:
	ID3D11Buffer					*m_pd3dColorBuffer;
};

//------------------------------------------------------------------------------------------------
class CMeshIlluminated : public CMesh
{
public:
	CMeshIlluminated(ID3D11Device *pd3dDevice);
	virtual ~CMeshIlluminated();

protected:
	ID3D11Buffer					*m_pd3dNormalBuffer;

public:
	XMVECTOR& CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2);
	void SetTriAngleListVertexNormal(XMVECTOR *pd3dxvNormals);
	void SetAverageVertexNormal(XMVECTOR *pd3dxvNormals, int nPrimitives, int nOffset, bool bStrip);
	void CalculateVertexNormal(XMVECTOR *pd3dxvNormals);
};

//------------------------------------------------------------------------------------------------
class CMeshTextured : public CMesh
{
public:
	CMeshTextured(ID3D11Device *pd3dDevice);
	virtual ~CMeshTextured();

protected:
	ID3D11Buffer					*m_pd3dTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CMeshDetailTextured : public CMeshTextured
{
public:
	CMeshDetailTextured(ID3D11Device *pd3dDevice);
	virtual ~CMeshDetailTextured();

protected:
	ID3D11Buffer					*m_pd3dDetailTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CMeshTexturedIlluminated : public CMeshIlluminated
{
public:
	CMeshTexturedIlluminated(ID3D11Device *pd3dDevice);
	virtual ~CMeshTexturedIlluminated();

protected:
	ID3D11Buffer					*m_pd3dTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CMeshDetailTexturedIlluminated : public CMeshIlluminated
{
public:
	CMeshDetailTexturedIlluminated(ID3D11Device *pd3dDevice);
	virtual ~CMeshDetailTexturedIlluminated();

protected:
	ID3D11Buffer					*m_pd3dTexCoordBuffer;
	ID3D11Buffer					*m_pd3dDetailTexCoordBuffer;
};

//------------------------------------------------------------------------------------------------
class CAirplaneMeshDiffused : public CMeshDiffused
{
public:
	CAirplaneMeshDiffused(ID3D11Device *pd3dDevice, float fWidth = 20.0f, float fHeight = 4.0f, float fDepth = 20.0f, XMVECTOR d3dxColor = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CAirplaneMeshDiffused();
};

//------------------------------------------------------------------------------------------------
class CCubeMeshDiffused : public CMeshDiffused
{
public:
	CCubeMeshDiffused(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f, XMVECTOR d3dxColor = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));
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

//------------------------------------------------------------------------------------------------
class CHeightMapGridMesh : public CMeshDetailTexturedIlluminated
{
protected:
	int							m_nWidth;
	int							m_nLength;
	XMFLOAT3					m_d3dxvScale;

public:
	CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, XMVECTOR d3dxvScale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), void *pContext = NULL, D3D11_USAGE d3dUsage = D3D11_USAGE_DEFAULT);
	virtual ~CHeightMapGridMesh();

	XMVECTOR& GetScale() { return(XMLoadFloat3(&m_d3dxvScale)); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	virtual float OnGetHeight(int x, int z, void *pContext);
};

//-------------------------------------------------------------------------------------------------
class CSkyBoxMesh : public CMeshTextured
{
public:
	CSkyBoxMesh(ID3D11Device *pd3dDevice, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f);
	virtual ~CSkyBoxMesh();
};

class CWaterGridMesh : public CHeightMapGridMesh
{
private:
	int							m_xStart;
	int							m_zStart;

	float						m_fK1;
	float						m_fK2;
	float						m_fK3;

	float						m_fTimeDelta;
	float						m_fSpatialDelta;
	float						m_fAccumulatedTime;

	XMFLOAT3					*m_pd3dxvPreviousPositions;
	XMFLOAT3					*m_pd3dxvCurrentPositions;
	XMFLOAT3					*m_pd3dxvTempPositions;
	XMFLOAT3					*m_pd3dxvNormalVectors;

public:
	CWaterGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, XMVECTOR d3dxvScale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), void *pContext = NULL, D3D11_USAGE d3dUsage = D3D11_USAGE_DEFAULT);
	virtual ~CWaterGridMesh();

	virtual float OnGetHeight(int x, int z, void *pContext);

	void SetDisturbingForce(int i, int j, float fMagnitude);
	void OnPrepareAnimation(float fTimeDelta, float fSpatialDelta, float fSpeed, float fDamping);

	virtual void Animate(ID3D11DeviceContext *pd3dDeviceContext, float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

//--------------------------------------------------------------------------------------------------
class CTextureToScreenRectMesh : public CMeshTextured
{
public:
	CTextureToScreenRectMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f);
	virtual ~CTextureToScreenRectMesh();
};

//------------------------------------------------------------------------------------------------
class CTexturedRectMesh : public CMeshTextured
{
public:
	CTexturedRectMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fLength = 2.0f);
	virtual ~CTexturedRectMesh();
};

