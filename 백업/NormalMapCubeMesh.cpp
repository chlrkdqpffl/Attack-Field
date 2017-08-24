#include "stdafx.h"
#include "NormalMapCubeMesh.h"


CNormalMapCubeMesh::CNormalMapCubeMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth)
{
	m_nVertices = 36;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
//	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pPositions = new XMFLOAT3[36];
	int i = 0;
	m_pPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, -fz);

	m_pPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	m_pPositions[i++] = XMFLOAT3(-fx, -fy, -fz);

	m_pPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, -fz);

	m_pPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(-fx, +fy, -fz);

	m_pPositions[i++] = XMFLOAT3(-fx, -fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, +fz);

	m_pPositions[i++] = XMFLOAT3(-fx, -fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(-fx, +fy, +fz);

	m_pPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, +fz);

	m_pPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pPositions[i++] = XMFLOAT3(-fx, -fy, +fz);

	m_pPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(-fx, -fy, -fz);

	m_pPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pPositions[i++] = XMFLOAT3(-fx, -fy, +fz);

	m_pPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, +fz);

	m_pPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, -fz);

	i = 0;
	m_pTexCoords = new XMFLOAT2[36];
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	XMVECTOR tangent[36];
	
	m_pNormals = new XMFLOAT3[36];
	m_pTangents = new XMFLOAT3[36];
	CalculateVertexNormal(m_pNormals);
	CalculateVertexTangent(m_pTangents);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTangentBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pTangents, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	DXUT_SetDebugName(m_pd3dTangentBuffer, "Tangent");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");

	ID3D11Buffer *pd3dBuffers[] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTangentBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[] = { 0, 0, 0, 0 };
	AssembleToVertexBuffer(4, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fx, fy, fz };

	delete[] m_pTexCoords;
	delete[] m_pNormals;
	delete[] m_pTangents;
}

CNormalMapCubeMesh::~CNormalMapCubeMesh()
{
}
