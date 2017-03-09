#include "stdafx.h"
#include "FbxModelMesh.h"


CFbxModelMesh::CFbxModelMesh(ID3D11Device *pd3dDevice, const MeshTag meshTag, const float size)
	: m_meshTag(meshTag), m_fModelSize(size)
{
}

CFbxModelMesh::~CFbxModelMesh()
{
	if (m_pNormals) delete[] m_pNormals;
	if (m_pTangents) delete[] m_pTangents;
	if (m_pTexCoords) delete[] m_pTexCoords;
	ReleaseCOM(m_pd3dTangentBuffer);
}

void CFbxModelMesh::Initialize(ID3D11Device *pd3dDevice)
{
	m_meshData = RESOURCE_MGR->CloneFbxMeshData(m_meshTag);

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nVertices = m_meshData.m_nVertexCount;
	m_nIndices = m_meshData.m_nIndexCount;

	m_pPositions = new XMFLOAT3[m_nVertices];
	m_pNormals = new XMFLOAT3[m_nVertices];
	if (m_meshData.m_bTangent)
		m_pTangents = new XMFLOAT3[m_nVertices];
	m_pTexCoords = new XMFLOAT2[m_nVertices];
	m_pnIndices = new UINT[m_nIndices];

	if (m_fModelSize == 1.0f) {
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
			XMStoreFloat3(&m_pPositions[i], XMVectorScale(XMLoadFloat3(&m_meshData.m_vecPosition[i]), m_fModelSize));
			//		XMStoreFloat3(&m_pvNormals[i], XMVectorScale(XMLoadFloat3(&m_meshData.m_vecNormal[i]), size));
			//		XMStoreFloat2(&m_pvTexCoords[i], XMVectorScale(XMLoadFloat2(&m_meshData.m_vecUV[i]), size));
			if (m_meshData.m_bTangent)
				m_pTangents[i] = m_meshData.m_vecTangent[i];
			m_pNormals[i] = m_meshData.m_vecNormal[i];
			m_pTexCoords[i] = m_meshData.m_vecUV[i];
		}
	}

	// Create Buffer
	if (m_meshData.m_bTangent) {
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
	if (m_meshData.m_bTangent)
		DXUT_SetDebugName(m_pd3dTangentBuffer, "Tangent");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");

	for (UINT i = 0, j = 0; i < m_nIndices / 3; ++i, j += 3) {
		m_pnIndices[j] = (UINT)m_meshData.m_vecIndex[i].x;
		m_pnIndices[j + 1] = (UINT)m_meshData.m_vecIndex[i].y;
		m_pnIndices[j + 2] = (UINT)m_meshData.m_vecIndex[i].z;
	}

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");
}