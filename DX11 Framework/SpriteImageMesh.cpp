#include "stdafx.h"
#include "SpriteImageMesh.h"


CSpriteImageMesh::CSpriteImageMesh(ID3D11Device *pd3dDevice, TextureTag tag, int frame, POINT perSize, float lifeTime, float totalTime)
	: m_TagTexture(tag), m_nFrame(frame), m_nPerSize(perSize), m_fLifeTime(lifeTime), m_fTotalTime(totalTime)
{
	/*
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	float fx = fWidth*0.5f, fy = fHeight*0.5f;

	m_pPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	m_pPositions[0] = XMFLOAT3(-fx, -fy, 0.9f);		pd3dxvTexCoords[0] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[1] = XMFLOAT3(-fx, +fy, 0.9f);		pd3dxvTexCoords[1] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[2] = XMFLOAT3(+fx, -fy, 0.9f);		pd3dxvTexCoords[2] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[3] = XMFLOAT3(+fx, +fy, 0.9f);		pd3dxvTexCoords[3] = XMFLOAT2(1.0f, 0.0f);

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
	*/
	
	

}

CSpriteImageMesh::~CSpriteImageMesh()
{
}

void CSpriteImageMesh::Update(const float& fDeltaTime)
{
	/*
	m_fTotalTime += fDeltaTime;

	if (m_fTotalTime >= m_fLifeTime)
		m_fTotalTime = 0;
		*/
	
	m_nFrame = int((m_fTotalTime / m_fLifeTime) * (m_nPerSize.x * m_nPerSize.y));


	if (m_fTotalTime <= m_fLifeTime) {
		m_fTotalTime += fDeltaTime;
	}
}

void CSpriteImageMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{

}