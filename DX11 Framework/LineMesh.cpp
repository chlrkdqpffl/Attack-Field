#include "stdafx.h"
#include "LineMesh.h"

CLineMesh::CLineMesh(ID3D11Device *pd3dDevice, XMFLOAT3 start, XMFLOAT3 end, XMVECTOR color)
{
	m_nVertices = 2;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	m_pPositions = new XMFLOAT3[m_nVertices];

	m_pPositions[0] = start;
	m_pPositions[1] = end;

	XMVECTOR pd3dxColors[2];
	pd3dxColors[0] = color;
	pd3dxColors[1] = color;

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dColorBuffer = CreateBuffer(pd3dDevice, sizeof(XMVECTOR), m_nVertices, pd3dxColors, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dColorBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMVECTOR) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dColorBuffer, "Color");
}

CLineMesh::~CLineMesh()
{
}
