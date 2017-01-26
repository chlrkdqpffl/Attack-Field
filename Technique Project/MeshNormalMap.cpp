#include "stdafx.h"
#include "MeshNormalMap.h"


CMeshNormalMap::CMeshNormalMap(ID3D11Device *pd3dDevice) : CMeshTexturedIlluminated(pd3dDevice)
{
}


CMeshNormalMap::~CMeshNormalMap()
{
}

XMVECTOR CMeshNormalMap::CalculateTriAngleTangent(UINT nIndex0, UINT nIndex1, UINT nIndex2)
{
	XMFLOAT3 vTangent{ 0.0f, 0.0f, 0.0f };
	XMFLOAT3 vector1, vector2;
	XMFLOAT2 tuVector, tvVector;
	float den;

	vector1.x = m_pPositions[nIndex1].x - m_pPositions[nIndex0].x;
	vector1.y = m_pPositions[nIndex1].y - m_pPositions[nIndex0].y;
	vector1.z = m_pPositions[nIndex1].z - m_pPositions[nIndex0].z;

	vector2.x = m_pPositions[nIndex2].x - m_pPositions[nIndex0].x;
	vector2.y = m_pPositions[nIndex2].y - m_pPositions[nIndex0].y;
	vector2.z = m_pPositions[nIndex2].z - m_pPositions[nIndex0].z;

	tuVector.x = m_pTexCoords[nIndex1].x - m_pTexCoords[nIndex0].x;
	tuVector.y = m_pTexCoords[nIndex1].y - m_pTexCoords[nIndex0].y;
	tvVector.x = m_pTexCoords[nIndex2].x - m_pTexCoords[nIndex0].x;
	tvVector.y = m_pTexCoords[nIndex2].y - m_pTexCoords[nIndex0].y;
	
	den = 1.0f / (tuVector.x * tvVector.y - tuVector.y * tvVector.x);
	
	vTangent.x = (tuVector.x * vector2.x - tvVector.x * vector1.x) * den;
	vTangent.y = (tuVector.x * vector2.y - tvVector.x * vector1.y) * den;
	vTangent.z = (tuVector.x * vector2.z - tvVector.x * vector1.z) * den;

	return(XMVector3Normalize(XMLoadFloat3(&vTangent)));
}

void CMeshNormalMap::SetTriAngleListVertexTangent(XMVECTOR *pd3dxvTangents)
{
	XMVECTOR d3dxvTangent;
	XMVECTOR *pd3dxvPositions = NULL;
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		d3dxvTangent = CalculateTriAngleTangent((i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + (i * 3 + 0);
		pd3dxvTangents[i * 3 + 0] = d3dxvTangent;
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + (i * 3 + 1);
		pd3dxvTangents[i * 3 + 1] = d3dxvTangent;
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + (i * 3 + 2);
		pd3dxvTangents[i * 3 + 2] = d3dxvTangent;
	}
}

void CMeshNormalMap::SetAverageVertexTangent(XMVECTOR *pd3dxvTangents, int nPrimitives, int nOffset, bool bStrip)
{
	XMVECTOR d3dxvSumOfTangent = XMVectorZero();
	XMVECTOR *pd3dxvPositions = NULL;
	UINT nIndex0, nIndex1, nIndex2;

	for (int j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfTangent = XMVectorZero();
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (m_pnIndices) nIndex0 = m_pnIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (m_pnIndices) nIndex1 = m_pnIndices[nIndex1];
			nIndex2 = (m_pnIndices) ? m_pnIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) d3dxvSumOfTangent += CalculateTriAngleTangent(nIndex0, nIndex1, nIndex2);
		}
		d3dxvSumOfTangent = XMVector3Normalize(d3dxvSumOfTangent);
		pd3dxvTangents[j] = d3dxvSumOfTangent;
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + j;

	}
}

void CMeshNormalMap::CalculateVertexTangent(XMVECTOR *pd3dxvTangents)
{
	switch (m_d3dPrimitiveTopology)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
	case D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST:
		if (!m_pnIndices)
			SetTriAngleListVertexTangent(pd3dxvTangents);
		else
			SetAverageVertexTangent(pd3dxvTangents, (m_nIndices / 3), 3, false);
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexTangent(pd3dxvTangents, (m_pnIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}
