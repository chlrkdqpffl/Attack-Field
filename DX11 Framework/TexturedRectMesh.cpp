#include "stdafx.h"
#include "TexturedRectMesh.h"


CTexturedRectMesh::CTexturedRectMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fLength)
{
	m_nVertices = 6;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fLength*0.5f;

	m_pPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	if ((fx != 0.0f) && (fz != 0.0f) && (fy == 0.0f))
	{
		m_pPositions[0] = XMFLOAT3(+fx, 0.0f, +fz); pd3dxvTexCoords[0] = XMFLOAT2(1.0f, 0.0f);
		m_pPositions[1] = XMFLOAT3(+fx, 0.0f, -fz); pd3dxvTexCoords[1] = XMFLOAT2(1.0f, 1.0f);
		m_pPositions[2] = XMFLOAT3(-fx, 0.0f, -fz); pd3dxvTexCoords[2] = XMFLOAT2(0.0f, 1.0f);
		m_pPositions[3] = XMFLOAT3(-fx, 0.0f, -fz); pd3dxvTexCoords[3] = XMFLOAT2(0.0f, 1.0f);
		m_pPositions[4] = XMFLOAT3(-fx, 0.0f, +fz); pd3dxvTexCoords[4] = XMFLOAT2(0.0f, 0.0f);
		m_pPositions[5] = XMFLOAT3(+fx, 0.0f, +fz); pd3dxvTexCoords[5] = XMFLOAT2(1.0f, 0.0f);
	}
	else if ((fx != 0.0f) && (fy != 0.0f) && (fz == 0.0f))
	{
		m_pPositions[0] = XMFLOAT3(+fx, +fy, 0.0f); pd3dxvTexCoords[0] = XMFLOAT2(1.0f, 0.0f);
		m_pPositions[1] = XMFLOAT3(+fx, -fy, 0.0f); pd3dxvTexCoords[1] = XMFLOAT2(1.0f, 1.0f);
		m_pPositions[2] = XMFLOAT3(-fx, -fy, 0.0f); pd3dxvTexCoords[2] = XMFLOAT2(0.0f, 1.0f);
		m_pPositions[3] = XMFLOAT3(-fx, -fy, 0.0f); pd3dxvTexCoords[3] = XMFLOAT2(0.0f, 1.0f);
		m_pPositions[4] = XMFLOAT3(-fx, +fy, 0.0f); pd3dxvTexCoords[4] = XMFLOAT2(0.0f, 0.0f);
		m_pPositions[5] = XMFLOAT3(+fx, +fy, 0.0f); pd3dxvTexCoords[5] = XMFLOAT2(1.0f, 0.0f);
	}
	else
	{
		m_pPositions[0] = XMFLOAT3(0.0f, +fy, -fz); pd3dxvTexCoords[0] = XMFLOAT2(1.0f, 0.0f);
		m_pPositions[1] = XMFLOAT3(0.0f, -fy, -fz); pd3dxvTexCoords[1] = XMFLOAT2(1.0f, 1.0f);
		m_pPositions[2] = XMFLOAT3(0.0f, -fy, +fz); pd3dxvTexCoords[2] = XMFLOAT2(0.0f, 1.0f);
		m_pPositions[3] = XMFLOAT3(0.0f, -fy, +fz); pd3dxvTexCoords[3] = XMFLOAT2(0.0f, 1.0f);
		m_pPositions[4] = XMFLOAT3(0.0f, +fy, +fz); pd3dxvTexCoords[4] = XMFLOAT2(0.0f, 0.0f);
		m_pPositions[5] = XMFLOAT3(0.0f, +fy, -fz); pd3dxvTexCoords[5] = XMFLOAT2(1.0f, 0.0f);
	}

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fx, fy, 0.0f };

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");
}

CTexturedRectMesh::~CTexturedRectMesh()
{
}
