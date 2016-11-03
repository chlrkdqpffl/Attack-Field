#include "stdafx.h"
#include "WaterGridMesh.h"

CWaterGridMesh::CWaterGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, XMVECTOR d3dxvScale, void *pContext, D3D11_USAGE d3dUsage) : CHeightMapGridMesh(pd3dDevice, xStart, zStart, nWidth, nLength, d3dxvScale, pContext, d3dUsage)
{
	m_xStart = xStart;
	m_zStart = zStart;

	m_pd3dxvPreviousPositions = NULL;
	m_pd3dxvCurrentPositions = NULL;
	m_pd3dxvNormalVectors = NULL;

	OnPrepareAnimation(0.00375f, 1.0f, 10.0f, 0.00001f);
//	OnPrepareAnimation(0.03f, 1.0f, 5.0f, 0.3f);
}

CWaterGridMesh::~CWaterGridMesh()
{
	if (m_pd3dxvPreviousPositions) delete[] m_pd3dxvPreviousPositions;
	if (m_pd3dxvCurrentPositions) delete[] m_pd3dxvCurrentPositions;
	if (m_pd3dxvNormalVectors) delete[] m_pd3dxvNormalVectors;
}

void CWaterGridMesh::OnPrepareAnimation(float fTimeDelta, float fSpatialDelta, float fSpeed, float fDamping)
{
	m_fTimeDelta = fTimeDelta;
	m_fSpatialDelta = fSpatialDelta;

	m_fAccumulatedTime = 0.0f;

	float d = (fDamping * m_fTimeDelta) + 2.0f;
	float e = (fSpeed * fSpeed) * (m_fTimeDelta * m_fTimeDelta) / (m_fSpatialDelta * m_fSpatialDelta);
	m_fK1 = ((fDamping * m_fTimeDelta) - 2.0f) / d;
	m_fK2 = (4.0f - (8.0f * e)) / d;
	m_fK3 = (2.0f * e) / d;

	m_pd3dxvPreviousPositions = new XMFLOAT3[m_nLength*m_nWidth];
	m_pd3dxvCurrentPositions = new XMFLOAT3[m_nLength*m_nWidth];
	m_pd3dxvNormalVectors = new XMFLOAT3[m_nLength*m_nWidth];

	for (int i = 0, z = m_zStart; z < (m_zStart + m_nLength); i++, z++)
	{
		for (int j = 0, x = m_xStart; x < (m_xStart + m_nWidth); j++, x++)
		{
			m_pd3dxvPreviousPositions[(i*m_nWidth) + j] = XMFLOAT3((x*m_d3dxvScale.x), 0.0f, (z*m_d3dxvScale.z));
			m_pd3dxvCurrentPositions[(i*m_nWidth) + j] = XMFLOAT3((x*m_d3dxvScale.x), 0.0f, (z*m_d3dxvScale.z));
			m_pd3dxvNormalVectors[(i*m_nWidth) + j] = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}
	}

	// Create Wave in RandomPosition - 일정 주기로 랜덤한 위치에 계속 생성하는 것이 맞다. 타이머 구현 후 업데이트로 이동하기
	for (int k = 0; k < 30; k++)
	{
		int i = 5 + (rand() % (m_nLength - 10));
		int j = 5 + (rand() % (m_nWidth - 10));

		float fMagnitude = RAND_FLOAT(3.0f, 5.0f);

		SetDisturbingForce(i, j, fMagnitude);
	}
}

void CWaterGridMesh::SetDisturbingForce(int i, int j, float fMagnitude)
{
	if (((i > 1) && (i < m_nLength - 2)) && ((j > 1) && (j < m_nWidth - 2)))
	{
		m_pd3dxvCurrentPositions[(i*m_nWidth) + j].y += fMagnitude;
		m_pd3dxvCurrentPositions[(i*m_nWidth) + j + 1].y += fMagnitude * 0.5f;
		m_pd3dxvCurrentPositions[(i*m_nWidth) + j - 1].y += fMagnitude * 0.5f;
		m_pd3dxvCurrentPositions[((i + 1)*m_nWidth) + j].y += fMagnitude * 0.5f;
		m_pd3dxvCurrentPositions[((i - 1)*m_nWidth) + j].y += fMagnitude * 0.5f;
	}
}

void CWaterGridMesh::Update(float fTimeElapsed)
{
	m_fAccumulatedTime += fTimeElapsed;
	if (m_fAccumulatedTime >= m_fTimeDelta)
	{
	
		for (int i = 1; i < m_nLength - 1; i++)
		{
			for (int j = 1; j < m_nWidth - 1; j++)
			{
				m_pd3dxvPreviousPositions[(i*m_nWidth) + j].y = m_fK1 * m_pd3dxvPreviousPositions[(i*m_nWidth) + j].y +
					m_fK2 * m_pd3dxvCurrentPositions[(i*m_nWidth) + j].y +
					m_fK3 * (m_pd3dxvCurrentPositions[((i + 1)*m_nWidth) + j].y + 
						m_pd3dxvCurrentPositions[((i - 1)*m_nWidth) + j].y + 
						m_pd3dxvCurrentPositions[(i*m_nWidth) + j + 1].y +
						m_pd3dxvCurrentPositions[(i*m_nWidth) + j - 1].y);
			}
		}
		m_fAccumulatedTime = 0.0f;

		std::swap(m_pd3dxvPreviousPositions, m_pd3dxvCurrentPositions);
		XMFLOAT3 d3dxvPosition(0.0f, 1.0f, 0.0f), d3dxvNormal(0.0f, 1.0f, 0.0f);

		for (int i = 1; i < m_nLength - 1; i++)
		{
			for (int j = 1; j < m_nWidth - 1; j++)
			{
				// Find two tangent vectors, one in +x and one in -z direction. Then take cross product to get normal. Use finite difference to compute tangents.  
				float l = m_pd3dxvCurrentPositions[(i*m_nWidth) + j - 1].y;
				float r = m_pd3dxvCurrentPositions[(i*m_nWidth) + j + 1].y;
				float t = m_pd3dxvCurrentPositions[((i - 1)*m_nWidth) + j - 1].y;
				float b = m_pd3dxvCurrentPositions[((i + 1)*m_nWidth) + j + 1].y;
				d3dxvNormal = XMFLOAT3((-r + l), (2.0f * m_fSpatialDelta), (b - t));

				XMStoreFloat3(&d3dxvNormal, XMVector3Normalize(XMLoadFloat3(&d3dxvNormal)));
				m_pd3dxvNormalVectors[(i*m_nWidth) + j] = d3dxvNormal;
			}
		}
	}
}

void CWaterGridMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	// Update the wave vertex buffer with the new position.
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	// 노말, 텍스쳐 좌표도 업데이트 해야 하는듯
	XMFLOAT3 *pd3dxvPositions = (XMFLOAT3 *)d3dMappedResource.pData;
	//D3DXVECTOR2 d3dxvTexture1, d3dxvTexture2;
	for (int i = 0, z = m_zStart; z < (m_zStart + m_nLength); z++, i++)
	{
		for (int j = 0, x = m_xStart; x < (m_xStart + m_nWidth); x++, j++)
		{
			pd3dxvPositions[(i*m_nWidth) + j] = m_pd3dxvCurrentPositions[(i*m_nWidth) + j];
		}
	}
	pd3dDeviceContext->Unmap(m_pd3dPositionBuffer, 0);

	// Normal Buffer
	pd3dDeviceContext->Map(m_pd3dNormalBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMFLOAT3 *pNormal = (XMFLOAT3 *)d3dMappedResource.pData;
	for (int i = 0, z = m_zStart; z < (m_zStart + m_nLength); z++, i++)
	{
		for (int j = 0, x = m_xStart; x < (m_xStart + m_nWidth); x++, j++)
		{
			pNormal[(i*m_nWidth) + j] = m_pd3dxvNormalVectors[(i*m_nWidth) + j];
		}
	}
	pd3dDeviceContext->Unmap(m_pd3dNormalBuffer, 0);
	
	CHeightMapGridMesh::Render(pd3dDeviceContext);
}