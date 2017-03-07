#include "stdafx.h"
#include "SkyBoxMesh.h"


CSkyBoxMesh::CSkyBoxMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth)
{
	m_nVertices = 24;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_pPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	int i = 0;
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;
	// Front Quad (remember all quads point inward)
	m_pPositions[i] = XMFLOAT3(-fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(+fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(+fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[i] = XMFLOAT3(-fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	// Back Quad																
	m_pPositions[i] = XMFLOAT3(+fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(-fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(-fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[i] = XMFLOAT3(+fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	// Left Quad																
	m_pPositions[i] = XMFLOAT3(-fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(-fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(-fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[i] = XMFLOAT3(-fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	// Right Quad																
	m_pPositions[i] = XMFLOAT3(+fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(+fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(+fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[i] = XMFLOAT3(+fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	// Top Quad																	
	m_pPositions[i] = XMFLOAT3(-fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(+fx, +fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(+fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[i] = XMFLOAT3(-fx, +fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);
	// Bottom Quad																
	m_pPositions[i] = XMFLOAT3(-fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(+fx, -fx, +fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[i] = XMFLOAT3(+fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[i] = XMFLOAT3(-fx, -fx, -fx); pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_nIndices = 4;
	m_pnIndices = new UINT[m_nIndices];

	m_pnIndices[0] = 0;
	m_pnIndices[1] = 1;
	m_pnIndices[2] = 3;
	m_pnIndices[3] = 2;

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fx, fy, fz };

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");	
}

CSkyBoxMesh::~CSkyBoxMesh()
{
}
