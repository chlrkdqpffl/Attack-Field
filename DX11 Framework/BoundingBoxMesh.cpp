#include "stdafx.h"
#include "BoundingBoxMesh.h"

CBoundingBoxMesh::CBoundingBoxMesh(ID3D11Device *pd3dDevice, BoundingBox bbox, float fSize)
{
	m_nVertices = 8;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	float fx = bbox.Extents.x * fSize;
	float fy = bbox.Extents.y * fSize;
	float fz = bbox.Extents.z * fSize;

	m_bcBoundingBox.Center = bbox.Center;
	m_bcBoundingBox.Extents = { fx, fy, fz };

	m_pPositions = new XMFLOAT3[m_nVertices];

	m_pPositions[0] = XMFLOAT3(-fx, +fy, -fz);
	m_pPositions[1] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[2] = XMFLOAT3(+fx, +fy, +fz);
	m_pPositions[3] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[4] = XMFLOAT3(-fx, -fy, -fz);
	m_pPositions[5] = XMFLOAT3(+fx, -fy, -fz);
	m_pPositions[6] = XMFLOAT3(+fx, -fy, +fz);
	m_pPositions[7] = XMFLOAT3(-fx, -fy, +fz);


	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Bbox Position");

	ID3D11Buffer *pd3dBuffers[1] = { m_pd3dPositionBuffer };
	UINT pnBufferStrides[1] = { sizeof(XMFLOAT3)};
	UINT pnBufferOffsets[1] = { 0 };
	AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_nIndices = 18;
	m_pnIndices = new UINT[m_nIndices];

	m_pnIndices[0] = 5; //5,6,4 - cw
	m_pnIndices[1] = 6; //6,4,7 - ccw
	m_pnIndices[2] = 4; //4,7,0 - cw
	m_pnIndices[3] = 7; //7,0,3 - ccw
	m_pnIndices[4] = 0; //0,3,1 - cw
	m_pnIndices[5] = 3; //3,1,2 - ccw
	m_pnIndices[6] = 1; //1,2,2 - cw 
	m_pnIndices[7] = 2; //2,2,3 - ccw
	m_pnIndices[8] = 2; //2,3,3 - cw  - Degenerated Index
	m_pnIndices[9] = 3; //3,3,7 - ccw - Degenerated Index
	m_pnIndices[10] = 3;//3,7,2 - cw  - Degenerated Index
	m_pnIndices[11] = 7;//7,2,6 - ccw
	m_pnIndices[12] = 2;//2,6,1 - cw
	m_pnIndices[13] = 6;//6,1,5 - ccw
	m_pnIndices[14] = 1;//1,5,0 - cw
	m_pnIndices[15] = 5;//5,0,4 - ccw
	m_pnIndices[16] = 0;
	m_pnIndices[17] = 4;

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Bbox Index");
}

CBoundingBoxMesh::~CBoundingBoxMesh()
{
}
