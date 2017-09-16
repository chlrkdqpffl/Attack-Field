#include "stdafx.h"
#include "AimMesh.h"


CAimMesh::CAimMesh(ID3D11Device *pd3dDevice, POINT startPos, POINT endPos, UINT width, XMFLOAT4 color)
	: m_ptStartPos(startPos), m_ptEndPos(endPos), m_nWitdh(width)
{
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_pPositions = new XMFLOAT3[m_nVertices];

	float leftX, rightX, leftY, rightY;
	leftX = (2 * (startPos.x - width) / (float)FRAME_BUFFER_WIDTH) - 1;
	rightX = (2 * (endPos.x + width) / (float)FRAME_BUFFER_WIDTH) - 1;

	leftY = -1 * ((2 * (startPos.y - width) / (float)FRAME_BUFFER_HEIGHT) - 1);
	rightY = -1 * ((2 * (endPos.y + width) / (float)FRAME_BUFFER_HEIGHT) - 1);

	m_pPositions[0] = XMFLOAT3(rightX, leftY, 0.0f); 
	m_pPositions[1] = XMFLOAT3(rightX, rightY, 0.0f);
	m_pPositions[2] = XMFLOAT3(leftX, leftY, 0.0f);
	m_pPositions[3] = XMFLOAT3(leftX, rightY, 0.0f);

	XMFLOAT4 *vColor = new XMFLOAT4[m_nVertices];
	for (int i = 0; i < m_nVertices; i++)
		vColor[i] = color;

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_pd3dColorBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT4), m_nVertices, vColor, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dColorBuffer, "Color");

	delete[] vColor;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dColorBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT4) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
}

CAimMesh::~CAimMesh()
{
}

void CAimMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	float leftX, rightX, leftY, rightY;

	leftX = (2 * (m_ptStartPos.x - m_nWitdh) / (float)FRAME_BUFFER_WIDTH) - 1;
	rightX = (2 * (m_ptEndPos.x + m_nWitdh) / (float)FRAME_BUFFER_WIDTH) - 1;

	leftY = -1 * ((2 * (m_ptStartPos.y - m_nWitdh) / (float)FRAME_BUFFER_HEIGHT) - 1);
	rightY = -1 * ((2 * (m_ptEndPos.y + m_nWitdh) / (float)FRAME_BUFFER_HEIGHT) - 1);

	XMFLOAT3 pPositions[4];
	pPositions[0] = XMFLOAT3(rightX, leftY, 0.0f);
	pPositions[1] = XMFLOAT3(rightX, rightY, 0.0f);
	pPositions[2] = XMFLOAT3(leftX, leftY, 0.0f);
	pPositions[3] = XMFLOAT3(leftX, rightY, 0.0f);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	HR(pd3dDeviceContext->Map(m_pd3dPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource));
	XMFLOAT3 *pf3Position = (XMFLOAT3 *)d3dMappedResource.pData;
	for (int i = 0; i < 4; ++i)
		pf3Position[i] = pPositions[i];
	pd3dDeviceContext->Unmap(m_pd3dPositionBuffer, 0);

	CMesh::Render(pd3dDeviceContext);
}