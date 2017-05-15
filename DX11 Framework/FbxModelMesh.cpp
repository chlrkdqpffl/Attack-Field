#include "stdafx.h"
#include "FbxModelMesh.h"


CFbxModelMesh::CFbxModelMesh(ID3D11Device *pd3dDevice, const MeshTag meshTag, const XMFLOAT3 size)
	: m_fModelSize(size)
{
	m_tagMesh = meshTag;
}

CFbxModelMesh::~CFbxModelMesh()
{
	SafeDeleteArray(m_pNormals);
	SafeDeleteArray(m_pTangents);
	SafeDeleteArray(m_pTexCoords);
	ReleaseCOM(m_pd3dTangentBuffer);
}

void CFbxModelMesh::Initialize(ID3D11Device *pd3dDevice, bool isCalcTangent)
{
	m_meshData = RESOURCE_MGR->CloneFbxMeshData(m_tagMesh);

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nVertices = m_meshData.m_nVertexCount;
	m_nIndices = m_meshData.m_nIndexCount;

	m_pPositions = new XMFLOAT3[m_nVertices];
	m_pNormals = new XMFLOAT3[m_nVertices];
	if (m_meshData.m_bTangent || isCalcTangent)
		m_pTangents = new XMFLOAT3[m_nVertices];
	m_pTexCoords = new XMFLOAT2[m_nVertices];
	m_pnIndices = new UINT[m_nIndices];
	m_bcBoundingBox = m_meshData.m_boundingBox;


	if( (m_fModelSize.x == 1) && (m_fModelSize.y == 1) && (m_fModelSize.z == 1)) {
		for (int i = 0; i < m_nVertices; ++i) {
			m_pPositions[i] = m_meshData.m_vecPosition[i];
			m_pNormals[i] = m_meshData.m_vecNormal[i];
			if (m_meshData.m_bTangent)
				m_pTangents[i] = m_meshData.m_vecTangent[i];
			m_pTexCoords[i] = m_meshData.m_vecUV[i];
		}
	}
	else {
		for (int i = 0; i < m_nVertices; ++i) {
			m_pPositions[i].x = m_meshData.m_vecPosition[i].x * m_fModelSize.x;
			m_pPositions[i].y = m_meshData.m_vecPosition[i].y * m_fModelSize.y;
			m_pPositions[i].z = m_meshData.m_vecPosition[i].z * m_fModelSize.z;

			if (m_meshData.m_bTangent)
				m_pTangents[i] = m_meshData.m_vecTangent[i];
			m_pNormals[i] = m_meshData.m_vecNormal[i];
			m_pTexCoords[i] = m_meshData.m_vecUV[i];
		}
	}

	// Create Index Buffer
	for (UINT i = 0, j = 0; i < m_nIndices / 3; ++i, j += 3) {
		m_pnIndices[j] = (UINT)m_meshData.m_vecIndex[i].x;
		m_pnIndices[j + 1] = (UINT)m_meshData.m_vecIndex[i].y;
		m_pnIndices[j + 2] = (UINT)m_meshData.m_vecIndex[i].z;
	}

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");

	if (isCalcTangent)
		CalculateVertexTangent(m_pTangents);

	// Create Buffer
	if (m_meshData.m_bTangent || isCalcTangent) {
		m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dTangentBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pTangents, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		ID3D11Buffer *pd3dBuffers[4] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTangentBuffer, m_pd3dTexCoordBuffer };
		UINT pnBufferStrides[4] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
		UINT pnBufferOffsets[4] = { 0, 0, 0, 0 };
		AssembleToVertexBuffer(4, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	else {
		m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
		UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
		UINT pnBufferOffsets[3] = { 0, 0, 0 };
		AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	if (m_meshData.m_bTangent || isCalcTangent)
		DXUT_SetDebugName(m_pd3dTangentBuffer, "Tangent");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
}

void CFbxModelMesh::CalculateVertexTangent(XMFLOAT3 *pd3dxvTangents)
{
	XMVECTOR d3dxvSumOfTangent = XMVectorZero();
	XMVECTOR *pd3dxvPositions = NULL;
	UINT nIndex0, nIndex1, nIndex2;

	for (int j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfTangent = XMVectorZero();
		for (int i = 0; i < m_meshData.m_vecIndex.size(); i++)
		{
			nIndex0 = (3 * i + 0);
			nIndex0 = m_pnIndices[nIndex0];
			nIndex1 = (3 * i + 1);
			nIndex1 = m_pnIndices[nIndex1];
			nIndex2 = m_pnIndices[3 * i + 2];
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j))
				d3dxvSumOfTangent += CalculateTriAngleTangent(nIndex0, nIndex1, nIndex2);
		}
		d3dxvSumOfTangent = XMVector3Normalize(d3dxvSumOfTangent);
		XMStoreFloat3(&pd3dxvTangents[j], d3dxvSumOfTangent);
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + j;

	}
}

XMVECTOR CFbxModelMesh::CalculateTriAngleTangent(UINT nIndex0, UINT nIndex1, UINT nIndex2)
{
	XMFLOAT3 vTangent{ 0.0f, 0.0f, 0.0f };
	XMFLOAT3 vector1, vector2;
	XMFLOAT2 tuVector, tvVector;
	float den;

	vector1.x = m_pPositions[nIndex1].x - m_pPositions[nIndex0].x;
	vector1.y = m_pPositions[nIndex1].y - m_pPositions[nIndex0].y;
	vector1.z = m_pPositions[nIndex1].z - m_pPositions[nIndex0].z;

	vector2.x = m_pPositions[nIndex2].x - m_pPositions[nIndex0].x;
	vector2.y = m_pPositions[nIndex2].y - m_pPositions[nIndex0].y;
	vector2.z = m_pPositions[nIndex2].z - m_pPositions[nIndex0].z;

	tuVector.x = m_pTexCoords[nIndex1].x - m_pTexCoords[nIndex0].x;
	tuVector.y = m_pTexCoords[nIndex1].y - m_pTexCoords[nIndex0].y;
	tvVector.x = m_pTexCoords[nIndex2].x - m_pTexCoords[nIndex0].x;
	tvVector.y = m_pTexCoords[nIndex2].y - m_pTexCoords[nIndex0].y;

	den = (tuVector.x * tvVector.y - tuVector.y * tvVector.x);

	vTangent.x = ((tvVector.y * vector1.x) + (-tvVector.x * vector2.x)) / den;
	vTangent.y = ((tvVector.y * vector1.y) + (-tvVector.x * vector2.y)) / den;
	vTangent.z = ((tvVector.y * vector1.z) + (-tvVector.x * vector2.z)) / den;

	return(XMVector3Normalize(XMLoadFloat3(&vTangent))); 
}