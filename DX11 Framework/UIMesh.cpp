#include "stdafx.h"
#include "UIMesh.h"

CUIMesh::CUIMesh(ID3D11Device *pd3dDevice, POINT startPos, POINT endPos, float zPos)
{
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_pPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	float leftX, rightX, leftY, rightY;

	leftX = (2 * startPos.x / (float)FRAME_BUFFER_WIDTH) - 1;
	rightX = (2 * endPos.x / (float)FRAME_BUFFER_WIDTH) - 1;

	leftY = -1 * ((2 * startPos.y / (float)FRAME_BUFFER_HEIGHT) - 1);
	rightY = -1 * ((2 * endPos.y / (float)FRAME_BUFFER_HEIGHT) - 1);

	m_pPositions[0] = XMFLOAT3(rightX, leftY, zPos); pd3dxvTexCoords[0] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[1] = XMFLOAT3(rightX, rightY, zPos); pd3dxvTexCoords[1] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[2] = XMFLOAT3(leftX, leftY, zPos); pd3dxvTexCoords[2] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[3] = XMFLOAT3(leftX, rightY, zPos); pd3dxvTexCoords[3] = XMFLOAT2(0.0f, 1.0f);


	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");

	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
}

CUIMesh::~CUIMesh()
{
}
