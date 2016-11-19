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
	vTangent.x = (m_pTexCoords[nIndex0].x * (m_pPositions[nIndex2].x - m_pPositions[nIndex0].x)) + (m_pTexCoords[nIndex0].y * (m_pPositions[nIndex1].x - m_pPositions[nIndex0].x));
	vTangent.y = (m_pTexCoords[nIndex1].x * (m_pPositions[nIndex2].y - m_pPositions[nIndex0].y)) + (m_pTexCoords[nIndex1].y * (m_pPositions[nIndex1].y - m_pPositions[nIndex0].y));
	vTangent.z = (m_pTexCoords[nIndex2].x * (m_pPositions[nIndex2].z - m_pPositions[nIndex0].z)) + (m_pTexCoords[nIndex2].y * (m_pPositions[nIndex1].z - m_pPositions[nIndex0].z));
	return(XMLoadFloat3(&vTangent));


	XMFLOAT3 vector1, vector2;
	XMFLOAT2 tuVector, tvVector;
	float den;
	float length;

	// Calculate the two vectors for this face.
	vector1.x = m_pPositions[nIndex1].x - m_pPositions[nIndex0].x;
	vector1.y = m_pPositions[nIndex1].y - m_pPositions[nIndex0].y;
	vector1.z = m_pPositions[nIndex1].z - m_pPositions[nIndex0].z;

	vector2.x = m_pPositions[nIndex2].x - m_pPositions[nIndex0].x;
	vector2.y = m_pPositions[nIndex2].y - m_pPositions[nIndex0].y;
	vector2.z = m_pPositions[nIndex2].z - m_pPositions[nIndex0].z;

	// Calculate the tu and tv texture space vectors.
	tuVector.x = m_pTexCoords[nIndex1].x - m_pTexCoords[nIndex0].x;
	tuVector.y = m_pTexCoords[nIndex2].x - m_pTexCoords[nIndex0].x;

	tvVector.x = m_pTexCoords[nIndex1].y - m_pTexCoords[nIndex0].y;
	tvVector.y = m_pTexCoords[nIndex2].y- m_pTexCoords[nIndex0].y;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector.x * tvVector.y - tuVector.y * tvVector.x);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	vTangent.x = (tvVector.y * vector1.x - tvVector.y * vector2.x) * den;
	vTangent.y = (tvVector.y * vector1.y - tvVector.y * vector2.y) * den;
	vTangent.z = (tvVector.y * vector1.z - tvVector.y * vector2.z) * den;

	// Calculate the length of this normal.
	length = sqrt((vTangent.x * vTangent.x) + (vTangent.y * vTangent.y) + (vTangent.z * vTangent.z));

	// Normalize the normal and then store it
	vTangent.x = vTangent.x / length;
	vTangent.y = vTangent.y / length;
	vTangent.z = vTangent.z / length;

	return(XMLoadFloat3(&vTangent));
}

void CMeshNormalMap::CalculateTriAngleTangent2(UINT nIndex0, UINT nIndex1, UINT nIndex2)
{
	XMFLOAT3 edge1, edge2;
	edge1.x = m_pPositions[nIndex1].x - m_pPositions[nIndex0].x;
	edge1.y = m_pPositions[nIndex1].y - m_pPositions[nIndex0].y;
	edge1.z = m_pPositions[nIndex1].z - m_pPositions[nIndex0].z;

	edge2.x = m_pPositions[nIndex2].x - m_pPositions[nIndex0].x;
	edge2.y = m_pPositions[nIndex2].y - m_pPositions[nIndex0].y;
	edge2.z = m_pPositions[nIndex2].z - m_pPositions[nIndex0].z;

	XMFLOAT2 uvEdge1, uvEdge2;
	uvEdge1.x = m_pTexCoords[nIndex1].x - m_pTexCoords[nIndex0].x;
	uvEdge1.y = m_pTexCoords[nIndex1].y - m_pTexCoords[nIndex0].y;

	uvEdge2.x = m_pTexCoords[nIndex2].x - m_pTexCoords[nIndex0].x;
	uvEdge2.y = m_pTexCoords[nIndex2].y - m_pTexCoords[nIndex0].y;

	float det = (uvEdge1.x * uvEdge2.y) - (uvEdge2.x * uvEdge1.y);

	// 범위 안으로 들어가는지 확인
	if (-0.0001f < det && det < 0.0001f)
	{
		m_pTangents[nIndex0].x = ((uvEdge2.y * edge1.x) + ((-1) * uvEdge1.y * edge2.x)) * (1.0f / det);
		m_pTangents[nIndex0].y = ((uvEdge2.y * edge1.y) + ((-1) * uvEdge1.y * edge2.y)) * (1.0f / det);
		m_pTangents[nIndex0].z = ((uvEdge2.y * edge1.z) + ((-1) * uvEdge1.y * edge2.z)) * (1.0f / det);
		float length = sqrt((m_pTangents[nIndex0].x * m_pTangents[nIndex0].x) + (m_pTangents[nIndex0].y * m_pTangents[nIndex0].y) + (m_pTangents[nIndex0].z * m_pTangents[nIndex0].z));

		m_pTangents[nIndex0].x = m_pTangents[nIndex0].x / length;
		m_pTangents[nIndex0].y = m_pTangents[nIndex0].y / length;
		m_pTangents[nIndex0].z = m_pTangents[nIndex0].z / length;
	}
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
		if (m_pnIndices == nullptr)
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
