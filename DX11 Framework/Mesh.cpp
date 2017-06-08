//-----------------------------------------------------------------------------
// File: CMesh.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"
#include "Object.h"

CMesh::CMesh()
{
	m_nType = VERTEX_POSITION_ELEMENT;

	m_nVertices = 0;
	m_nBuffers = 0;
	m_pd3dPositionBuffer = NULL;

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_nSlot = 0;
	m_nStartVertex = 0;

	m_pd3dIndexBuffer = nullptr;
	m_nIndices = 0;
	m_nStartIndex = 0;
	m_nBaseVertex = 0;
	m_nIndexOffset = 0;
	m_dxgiIndexFormat = DXGI_FORMAT_R32_UINT;

	m_pPositions = nullptr;
	m_pnIndices = nullptr;

	m_bcBoundingBox.Center = { 0.0f, 0.0f, 0.0f };
	m_bcBoundingBox.Extents = { 0.0f, 0.0f, 0.0f};
}

CMesh::~CMesh()
{
	ReleaseCOM(m_pd3dPositionBuffer);
	ReleaseCOM(m_pd3dIndexBuffer);
	
	SafeDeleteArray(m_ppd3dVertexBuffers);
	SafeDeleteArray(m_pnVertexStrides);
	SafeDeleteArray(m_pnVertexOffsets);
	SafeDeleteArray(m_pPositions);
	SafeDeleteArray(m_pnIndices);
}

ID3D11Buffer *CMesh::CreateBuffer(ID3D11Device *pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = d3dUsage;
	d3dBufferDesc.ByteWidth = nStride * nElements;
	d3dBufferDesc.BindFlags = nBindFlags;
	d3dBufferDesc.CPUAccessFlags = nCPUAccessFlags;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pBufferData;

	ID3D11Buffer *pd3dBuffer = NULL;
	HR(pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &pd3dBuffer));
	return(pd3dBuffer);
}

void CMesh::AssembleToVertexBuffer(int nBuffers, ID3D11Buffer **ppd3dBuffers, UINT *pnBufferStrides, UINT *pnBufferOffsets)
{
	ID3D11Buffer **ppd3dNewVertexBuffers = new ID3D11Buffer*[m_nBuffers + nBuffers];
	UINT *pnNewVertexStrides = new UINT[m_nBuffers + nBuffers];
	UINT *pnNewVertexOffsets = new UINT[m_nBuffers + nBuffers];

	if (m_nBuffers > 0)
	{
		for (int i = 0; i < m_nBuffers; i++)
		{
			ppd3dNewVertexBuffers[i] = m_ppd3dVertexBuffers[i];
			pnNewVertexStrides[i] = m_pnVertexStrides[i];
			pnNewVertexOffsets[i] = m_pnVertexOffsets[i];
		}
		if (m_ppd3dVertexBuffers) delete[] m_ppd3dVertexBuffers;
		if (m_pnVertexStrides) delete[] m_pnVertexStrides;
		if (m_pnVertexOffsets) delete[] m_pnVertexOffsets;
	}

	for (int i = 0; i < nBuffers; i++)
	{
		ppd3dNewVertexBuffers[m_nBuffers + i] = ppd3dBuffers[i];
		pnNewVertexStrides[m_nBuffers + i] = pnBufferStrides[i];
		pnNewVertexOffsets[m_nBuffers + i] = pnBufferOffsets[i];
	}

	m_nBuffers += nBuffers;
	m_ppd3dVertexBuffers = ppd3dNewVertexBuffers;
	m_pnVertexStrides = pnNewVertexStrides;
	m_pnVertexOffsets = pnNewVertexOffsets;
}

void CMesh::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetVertexBuffers(m_nSlot, m_nBuffers, m_ppd3dVertexBuffers, m_pnVertexStrides, m_pnVertexOffsets);
	pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, m_dxgiIndexFormat, m_nIndexOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
}

void CMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		pd3dDeviceContext->Draw(m_nVertices, m_nStartVertex);
}

void CMesh::RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances, int nStartInstance)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstances, m_nStartIndex, m_nBaseVertex, nStartInstance);
	else
		pd3dDeviceContext->DrawInstanced(m_nVertices, nInstances, m_nStartVertex, nStartInstance);
}

int CMesh::CheckRayIntersection(XMVECTOR *pd3dxvRayPosition, XMVECTOR *pd3dxvRayDirection, CollisionInfo *pd3dxIntersectInfo)
{
	int nIntersections = 0;
	BYTE *pbPositions = (BYTE *)m_pPositions + m_pnVertexOffsets[0];

	int nOffset = (m_d3dPrimitiveTopology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) ? 3 : 1;
	int nPrimitives = (m_d3dPrimitiveTopology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) ? (m_nVertices / 3) : (m_nVertices - 2);
	if (m_nIndices > 0) nPrimitives = (m_d3dPrimitiveTopology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) ? (m_nIndices / 3) : (m_nIndices - 2);

	XMFLOAT3 v0, v1, v2;
//	XMVECTOR v0, v1, v2;
	v0 = v1 = v2 = XMFLOAT3(0, 0, 0);
	float fuHitBaryCentric = 0.0f, fvHitBaryCentric = 0.0f, fHitDistance = 0.0f, fNearHitDistance = FLT_MAX;
	for (int i = 0; i < nPrimitives; i++)
	{
		v0 = *(XMFLOAT3 *)(pbPositions + ((m_pnIndices) ? (m_pnIndices[(i*nOffset) + 0]) : ((i*nOffset) + 0)) * m_pnVertexStrides[0]);
		v1 = *(XMFLOAT3 *)(pbPositions + ((m_pnIndices) ? (m_pnIndices[(i*nOffset) + 1]) : ((i*nOffset) + 1)) * m_pnVertexStrides[0]);
		v2 = *(XMFLOAT3 *)(pbPositions + ((m_pnIndices) ? (m_pnIndices[(i*nOffset) + 2]) : ((i*nOffset) + 2)) * m_pnVertexStrides[0]);

	if (D3DXIntersectTri((D3DXVECTOR3 *)&v0, (D3DXVECTOR3 *)&v1, (D3DXVECTOR3 *)&v2, (D3DXVECTOR3 *)pd3dxvRayPosition, (D3DXVECTOR3 *)pd3dxvRayDirection, &fuHitBaryCentric, &fvHitBaryCentric, &fHitDistance))	{ 
			if (fHitDistance < fNearHitDistance){
				fNearHitDistance = fHitDistance;
				if (pd3dxIntersectInfo)	{
					pd3dxIntersectInfo->m_dwFaceIndex = i;
					pd3dxIntersectInfo->m_fU = fuHitBaryCentric;
					pd3dxIntersectInfo->m_fV = fvHitBaryCentric;
					pd3dxIntersectInfo->m_fDistance = fHitDistance;
				
					XMVECTOR edge1 = XMLoadFloat3(&v1) - XMLoadFloat3(&v0);
					XMVECTOR edge2 = XMLoadFloat3(&v2) - XMLoadFloat3(&v0);
					XMVECTOR normal = XMVector3Cross(edge1, edge2);
					normal = XMVector3Normalize(normal);
					XMStoreFloat3(&pd3dxIntersectInfo->m_f3HitNormal, -1 * normal);		// -1을 해줘야 정확하던데 왜그런지 미확인
				}
			}
			nIntersections++;
		}
	}
	return(nIntersections);
}

void CMesh::CalculateBoundingCube()
{
	XMFLOAT3 xmf3Cen = m_bcBoundingBox.Center;
	XMFLOAT3 xmf3Ext = m_bcBoundingBox.Extents;

	XMFLOAT3 xmf3max = {xmf3Cen.x + xmf3Ext.x,	xmf3Cen.y + xmf3Ext.y,	xmf3Cen.z + xmf3Ext.z,	};
	XMFLOAT3 xmf3min = {xmf3Cen.x - xmf3Ext.x,	xmf3Cen.y - xmf3Ext.y,	xmf3Cen.z - xmf3Ext.z,
	};

	for (int i = 0; i < m_nVertices; i++)
	{
		if (m_pPositions[i].x < xmf3min.x) xmf3min.x = m_pPositions[i].x;
		if (m_pPositions[i].x > xmf3max.x) xmf3max.x = m_pPositions[i].x;
		if (m_pPositions[i].y < xmf3min.y) xmf3min.y = m_pPositions[i].y;
		if (m_pPositions[i].y > xmf3max.y) xmf3max.y = m_pPositions[i].y;
		if (m_pPositions[i].z < xmf3min.z) xmf3min.z = m_pPositions[i].z;
		if (m_pPositions[i].z > xmf3max.z) xmf3max.z = m_pPositions[i].z;
	}
	xmf3Cen = 
	{ (xmf3min.x + xmf3max.x) * 0.5f,	(xmf3min.y + xmf3max.y) * 0.5f,	(xmf3min.z + xmf3max.z) * 0.5f,};
	xmf3Ext = 
	{ fabs(xmf3min.x - xmf3max.x) * 0.5f, fabs(xmf3min.y - xmf3max.y) * 0.5f, fabs(xmf3min.z - xmf3max.z) * 0.5f, };
	m_bcBoundingBox.Center = xmf3Cen;
	m_bcBoundingBox.Extents = xmf3Ext;
}

//------------------------------------------------------------------------------------------------
CMeshDiffused::CMeshDiffused()
{
	m_nType |= VERTEX_COLOR_ELEMENT;
	m_pd3dColorBuffer = NULL;
}

CMeshDiffused::~CMeshDiffused()
{
	ReleaseCOM(m_pd3dColorBuffer);
}

//------------------------------------------------------------------------------------------------
CMeshIlluminated::CMeshIlluminated()
{
	m_nType |= VERTEX_NORMAL_ELEMENT;
	m_pd3dNormalBuffer = NULL;
}

CMeshIlluminated::~CMeshIlluminated()
{
	ReleaseCOM(m_pd3dNormalBuffer);
}

XMVECTOR CMeshIlluminated::CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2)
{
	XMVECTOR d3dxvNormal = XMVectorZero();
	XMVECTOR d3dxvP0 = XMLoadFloat3(&m_pPositions[nIndex0]);
	XMVECTOR d3dxvP1 = XMLoadFloat3(&m_pPositions[nIndex1]);
	XMVECTOR d3dxvP2 = XMLoadFloat3(&m_pPositions[nIndex2]);
	XMVECTOR d3dxvEdge1 = d3dxvP1 - d3dxvP0;
	XMVECTOR d3dxvEdge2 = d3dxvP2 - d3dxvP0;
	d3dxvNormal = XMVector3Cross(d3dxvEdge1, d3dxvEdge2);
	d3dxvNormal = XMVector3Normalize(d3dxvNormal);
	return(d3dxvNormal);
}

void CMeshIlluminated::SetTriAngleListVertexNormal(XMFLOAT3 *pd3dxvNormals)
{
	XMVECTOR d3dxvNormal;
	XMVECTOR *pd3dxvPositions = NULL;
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		d3dxvNormal = CalculateTriAngleNormal((i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + (i * 3 + 0);
		XMStoreFloat3(&pd3dxvNormals[i * 3 + 0], d3dxvNormal);
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + (i * 3 + 1);
		XMStoreFloat3(&pd3dxvNormals[i * 3 + 1], d3dxvNormal);
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + (i * 3 + 2);
		XMStoreFloat3(&pd3dxvNormals[i * 3 + 2], d3dxvNormal);
	}
}

void CMeshIlluminated::SetAverageVertexNormal(XMFLOAT3 *pd3dxvNormals, int nPrimitives, int nOffset, bool bStrip)
{
	XMVECTOR d3dxvSumOfNormal = XMVectorZero();
	XMVECTOR *pd3dxvPositions = NULL;
	UINT nIndex0, nIndex1, nIndex2;

	for (int j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfNormal = XMVectorZero();
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (m_pnIndices) nIndex0 = m_pnIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (m_pnIndices) nIndex1 = m_pnIndices[nIndex1];
			nIndex2 = (m_pnIndices) ? m_pnIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) d3dxvSumOfNormal += CalculateTriAngleNormal(nIndex0, nIndex1, nIndex2);
		}
		d3dxvSumOfNormal = XMVector3Normalize(d3dxvSumOfNormal);
		XMStoreFloat3(&pd3dxvNormals[j], d3dxvSumOfNormal);
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + j;

	}
}

void CMeshIlluminated::CalculateVertexNormal(XMFLOAT3 *pd3dxvNormals)
{
	switch (m_d3dPrimitiveTopology)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
	case D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST:
		if (!m_pnIndices)
			SetTriAngleListVertexNormal(pd3dxvNormals);
		else
			SetAverageVertexNormal(pd3dxvNormals, (m_nIndices / 3), 3, false);
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexNormal(pd3dxvNormals, (m_pnIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------------------------
CMeshTextured::CMeshTextured()
{
	m_nType |= VERTEX_TEXTURE_ELEMENT_0;
	m_pd3dTexCoordBuffer = NULL;
}

CMeshTextured::~CMeshTextured()
{
	ReleaseCOM(m_pd3dTexCoordBuffer);
}

//------------------------------------------------------------------------------------------------
CMeshDetailTextured::CMeshDetailTextured()
{
	m_nType |= VERTEX_TEXTURE_ELEMENT_1;
	m_pd3dDetailTexCoordBuffer = NULL;
}

CMeshDetailTextured::~CMeshDetailTextured()
{
	ReleaseCOM(m_pd3dDetailTexCoordBuffer);
}

//------------------------------------------------------------------------------------------------
CMeshTexturedIlluminated::CMeshTexturedIlluminated()
{
	m_nType |= VERTEX_TEXTURE_ELEMENT_0;
	m_pd3dTexCoordBuffer = NULL;
}

CMeshTexturedIlluminated::~CMeshTexturedIlluminated()
{
	ReleaseCOM(m_pd3dTexCoordBuffer);
}

//------------------------------------------------------------------------------------------------

CMeshNormalMap::CMeshNormalMap()
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

	den = (tuVector.x * tvVector.y - tuVector.y * tvVector.x);

	vTangent.x = ((tvVector.y * vector1.x) + (-tvVector.x * vector2.x)) / den;
	vTangent.y = ((tvVector.y * vector1.y) + (-tvVector.x * vector2.y)) / den;
	vTangent.z = ((tvVector.y * vector1.z) + (-tvVector.x * vector2.z)) / den;

	return XMVector3Normalize(XMLoadFloat3(&vTangent));
}

void CMeshNormalMap::SetTriAngleListVertexTangent(XMFLOAT3 *pd3dxvTangents)
{
	XMVECTOR vTangent;
	XMVECTOR *pd3dxvPositions = NULL;
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		vTangent = CalculateTriAngleTangent((i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + (i * 3 + 0);
		XMStoreFloat3(&pd3dxvTangents[i * 3 + 0], vTangent);
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + (i * 3 + 1);
		XMStoreFloat3(&pd3dxvTangents[i * 3 + 1], vTangent);
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + (i * 3 + 2);
		XMStoreFloat3(&pd3dxvTangents[i * 3 + 2], vTangent);
	}
}

void CMeshNormalMap::SetAverageVertexTangent(XMFLOAT3 *pd3dxvTangents, int nPrimitives, int nOffset, bool bStrip)
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
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) 
				d3dxvSumOfTangent += CalculateTriAngleTangent(nIndex0, nIndex1, nIndex2);
		}
		d3dxvSumOfTangent = XMVector3Normalize(d3dxvSumOfTangent);
		XMStoreFloat3(&pd3dxvTangents[j], d3dxvSumOfTangent);
		pd3dxvPositions = &XMLoadFloat3(m_pPositions) + j;

	}
}

void CMeshNormalMap::CalculateVertexTangent(XMFLOAT3 *pd3dxvTangents)
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

//------------------------------------------------------------------------------------------------
CMeshDetailTexturedIlluminated::CMeshDetailTexturedIlluminated()
{
	m_nType |= (VERTEX_TEXTURE_ELEMENT_0 | VERTEX_TEXTURE_ELEMENT_1);
	m_pd3dTexCoordBuffer = NULL;
	m_pd3dDetailTexCoordBuffer = NULL;
}

CMeshDetailTexturedIlluminated::~CMeshDetailTexturedIlluminated()
{
	ReleaseCOM(m_pd3dTexCoordBuffer);
	ReleaseCOM(m_pd3dDetailTexCoordBuffer);
}

//------------------------------------------------------------------------------------------------
CCubeMeshDiffused::CCubeMeshDiffused(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth, XMVECTOR d3dxColor)
{
	m_nVertices = 8;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pPositions = new XMFLOAT3[m_nVertices];

	m_pPositions[0] = XMFLOAT3(-fx, +fy, -fz);
	m_pPositions[1] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[2] = XMFLOAT3(+fx, +fy, +fz);
	m_pPositions[3] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[4] = XMFLOAT3(-fx, -fy, -fz);
	m_pPositions[5] = XMFLOAT3(+fx, -fy, -fz);
	m_pPositions[6] = XMFLOAT3(+fx, -fy, +fz);
	m_pPositions[7] = XMFLOAT3(-fx, -fy, +fz);

	XMVECTOR pd3dxColors[8];
	for (int i = 0; i < 8; i++) 
		pd3dxColors[i] = d3dxColor;

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dColorBuffer = CreateBuffer(pd3dDevice, sizeof(XMVECTOR), m_nVertices, pd3dxColors, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dColorBuffer, "Color");

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dColorBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMVECTOR) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

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
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");

	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fx, fy, fz };
}

CCubeMeshDiffused::~CCubeMeshDiffused()
{
}

//------------------------------------------------------------------------------------------------
CSphereMeshDiffused::CSphereMeshDiffused(ID3D11Device *pd3dDevice, float fRadius, int nSlices, int nStacks, XMVECTOR d3dxColor)
{
	m_nVertices = (nSlices * nStacks) * 3 * 2;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pPositions = new XMFLOAT3[m_nVertices];

	float theta_i, theta_ii, phi_j, phi_jj, fRadius_j, fRadius_jj, y_j, y_jj;
	for (int j = 0, k = 0; j < nStacks; j++)
	{
		phi_j = float(D3DX_PI / nStacks) * j;
		phi_jj = float(D3DX_PI / nStacks) * (j + 1);
		fRadius_j = fRadius * sinf(phi_j);
		fRadius_jj = fRadius * sinf(phi_jj);
		y_j = fRadius*cosf(phi_j);
		y_jj = fRadius*cosf(phi_jj);
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = float(2 * D3DX_PI / nSlices) * i;
			theta_ii = float(2 * D3DX_PI / nSlices) * (i + 1);
			m_pPositions[k++] = XMFLOAT3(fRadius_j*cosf(theta_i), y_j, fRadius_j*sinf(theta_i));
			m_pPositions[k++] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			m_pPositions[k++] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
			m_pPositions[k++] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			m_pPositions[k++] = XMFLOAT3(fRadius_jj*cosf(theta_ii), y_jj, fRadius_jj*sinf(theta_ii));
			m_pPositions[k++] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
		}
	}

	XMVECTOR *pd3dxColors = new XMVECTOR[m_nVertices];
	for (int i = 0; i < m_nVertices; i++)
		pd3dxColors[i] = d3dxColor + XMVectorSet(0, 0, ((100 / m_nVertices) * i / m_nVertices), 0);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dColorBuffer = CreateBuffer(pd3dDevice, sizeof(XMCOLOR), m_nVertices, pd3dxColors, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dColorBuffer, "Color");

	delete[] pd3dxColors;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dColorBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMCOLOR) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fRadius, fRadius, fRadius };
}

CSphereMeshDiffused::~CSphereMeshDiffused()
{
}

//------------------------------------------------------------------------------------------------
CCubeMeshIlluminated::CCubeMeshIlluminated(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth)
{
	m_nVertices = 8;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pPositions = new XMFLOAT3[m_nVertices];

	m_pPositions[0] = XMFLOAT3(-fx, +fy, -fz);
	m_pPositions[1] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[2] = XMFLOAT3(+fx, +fy, +fz);
	m_pPositions[3] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[4] = XMFLOAT3(-fx, -fy, -fz);
	m_pPositions[5] = XMFLOAT3(+fx, -fy, -fz);
	m_pPositions[6] = XMFLOAT3(+fx, -fy, +fz);
	m_pPositions[7] = XMFLOAT3(-fx, -fy, +fz);

	m_nIndices = 36;
	m_pnIndices = new UINT[m_nIndices];

	m_pnIndices[0] = 3; m_pnIndices[1] = 1; m_pnIndices[2] = 0;
	m_pnIndices[3] = 2; m_pnIndices[4] = 1; m_pnIndices[5] = 3;
	m_pnIndices[6] = 0; m_pnIndices[7] = 5; m_pnIndices[8] = 4;
	m_pnIndices[9] = 1; m_pnIndices[10] = 5; m_pnIndices[11] = 0;
	m_pnIndices[12] = 3; m_pnIndices[13] = 4; m_pnIndices[14] = 7;
	m_pnIndices[15] = 0; m_pnIndices[16] = 4; m_pnIndices[17] = 3;
	m_pnIndices[18] = 1; m_pnIndices[19] = 6; m_pnIndices[20] = 5;
	m_pnIndices[21] = 2; m_pnIndices[22] = 6; m_pnIndices[23] = 1;
	m_pnIndices[24] = 2; m_pnIndices[25] = 7; m_pnIndices[26] = 6;
	m_pnIndices[27] = 3; m_pnIndices[28] = 7; m_pnIndices[29] = 2;
	m_pnIndices[30] = 6; m_pnIndices[31] = 4; m_pnIndices[32] = 5;
	m_pnIndices[33] = 7; m_pnIndices[34] = 4; m_pnIndices[35] = 6;

	XMFLOAT3 pd3dxvNormals[8];
	CalculateVertexNormal(pd3dxvNormals);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");


	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fx, fy, fz };
}

CCubeMeshIlluminated::~CCubeMeshIlluminated()
{
}

#define _WITH_INDEX_BUFFER

//------------------------------------------------------------------------------------------------
CSphereMeshIlluminated::CSphereMeshIlluminated(ID3D11Device *pd3dDevice, float fRadius, int nSlices, int nStacks)
{
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fDeltaPhi = float(D3DX_PI / nStacks);
	float fDeltaTheta = float((2.0f * D3DX_PI) / nSlices);
	int k = 0;

#ifdef _WITH_INDEX_BUFFER
	m_nVertices = 2 + (nSlices * (nStacks - 1));
	m_pPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT3 *pd3dxvNormals = new XMFLOAT3[m_nVertices];

	m_pPositions[k] = XMFLOAT3(0.0f, +fRadius, 0.0f);
	XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pPositions[k]))); k++;
	float theta_i, phi_j;
	for (int j = 1; j < nStacks; j++)
	{
		phi_j = fDeltaPhi * j;
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = fDeltaTheta * i;
			m_pPositions[k] = XMFLOAT3(fRadius*sinf(phi_j)*cosf(theta_i), fRadius*cosf(phi_j), fRadius*sinf(phi_j)*sinf(theta_i));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pPositions[k]))); k++;
		}
	}
	m_pPositions[k] = XMFLOAT3(0.0f, -fRadius, 0.0f);
	XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pPositions[k]))); k++;

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	if (pd3dxvNormals) delete[] pd3dxvNormals;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	k = 0;
	m_nIndices = (nSlices * 3) * 2 + (nSlices * (nStacks - 2) * 3 * 2);
	m_pnIndices = new UINT[m_nIndices];
	for (int i = 0; i < nSlices; i++)
	{
		m_pnIndices[k++] = 0;
		m_pnIndices[k++] = 1 + ((i + 1) % nSlices);
		m_pnIndices[k++] = 1 + i;
	}
	for (int j = 0; j < nStacks - 2; j++)
	{
		for (int i = 0; i < nSlices; i++)
		{
			m_pnIndices[k++] = 1 + (i + (j * nSlices));
			m_pnIndices[k++] = 1 + (((i + 1) % nSlices) + (j * nSlices));
			m_pnIndices[k++] = 1 + (i + ((j + 1) * nSlices));
			m_pnIndices[k++] = 1 + (i + ((j + 1) * nSlices));
			m_pnIndices[k++] = 1 + (((i + 1) % nSlices) + (j * nSlices));
			m_pnIndices[k++] = 1 + (((i + 1) % nSlices) + ((j + 1) * nSlices));
		}
	}
	for (int i = 0; i < nSlices; i++)
	{
		m_pnIndices[k++] = (m_nVertices - 1);
		m_pnIndices[k++] = ((m_nVertices - 1) - nSlices) + i;
		m_pnIndices[k++] = ((m_nVertices - 1) - nSlices) + ((i + 1) % nSlices);
	}

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
#else
	m_nVertices = (nSlices * 3) * 2 + (nSlices * (nStacks - 2) * 3 * 2);
	m_pPositions = new XMVECTOR[m_nVertices];
	XMVECTOR *pd3dxvNormals = new XMVECTOR[m_nVertices];

	float theta_i, theta_ii, phi_j = 0.0f, phi_jj = fDeltaPhi;
	for (int i = 0; i < nSlices; i++)
	{
		theta_i = fDeltaTheta * i;
		theta_ii = fDeltaTheta * (i + 1);
		m_pPositions[k] = XMVECTOR(0.0f, +fRadius, 0.0f);
		D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
		m_pPositions[k] = XMVECTOR(fRadius*cosf(theta_i)*sinf(phi_jj), fRadius*cosf(phi_jj), fRadius*sinf(theta_i)*sinf(phi_jj));
		D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
		m_pPositions[k] = XMVECTOR(fRadius*cosf(theta_ii)*sinf(phi_jj), fRadius*cosf(phi_jj), fRadius*sinf(theta_ii)*sinf(phi_jj));
		D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
	}

	for (int j = 1; j < nStacks - 1; j++)
	{
		phi_j = fDeltaPhi * j;
		phi_jj = fDeltaPhi * (j + 1);
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = fDeltaTheta * i;
			theta_ii = fDeltaTheta * (i + 1);
			m_pPositions[k] = XMVECTOR(fRadius*cosf(theta_i)*sinf(phi_j), fRadius*cosf(phi_j), fRadius*sinf(theta_i)*sinf(phi_j));
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
			m_pPositions[k] = XMVECTOR(fRadius*cosf(theta_i)*sinf(phi_jj), fRadius*cosf(phi_jj), fRadius*sinf(theta_i)*sinf(phi_jj));
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
			m_pPositions[k] = XMVECTOR(fRadius*cosf(theta_ii)*sinf(phi_j), fRadius*cosf(phi_j), fRadius*sinf(theta_ii)*sinf(phi_j));
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
			m_pPositions[k] = XMVECTOR(fRadius*cosf(theta_i)*sinf(phi_jj), fRadius*cosf(phi_jj), fRadius*sinf(theta_i)*sinf(phi_jj));
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
			m_pPositions[k] = XMVECTOR(fRadius*cosf(theta_ii)*sinf(phi_jj), fRadius*cosf(phi_jj), fRadius*sinf(theta_ii)*sinf(phi_jj));
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
			m_pPositions[k] = XMVECTOR(fRadius*cosf(theta_ii)*sinf(phi_j), fRadius*cosf(phi_j), fRadius*sinf(theta_ii)*sinf(phi_j));
			D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
		}
	}
	phi_j = fDeltaPhi * (nStacks - 1);
	for (int i = 0; i < nSlices; i++)
	{
		theta_i = fDeltaTheta * i;
		theta_ii = fDeltaTheta * (i + 1);
		m_pPositions[k] = XMVECTOR(0.0f, -fRadius, 0.0f);
		D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
		m_pPositions[k] = XMVECTOR(fRadius*cosf(theta_ii)*sinf(phi_j), fRadius*cosf(phi_j), fRadius*sinf(theta_ii)*sinf(phi_j));
		D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
		m_pPositions[k] = XMVECTOR(fRadius*cosf(theta_i)*sinf(phi_j), fRadius*cosf(phi_j), fRadius*sinf(theta_i)*sinf(phi_j));
		D3DXVec3Normalize(&pd3dxvNormals[k], &m_pPositions[k]); k++;
	}

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMVECTOR), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMVECTOR), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	if (pd3dxvNormals) delete[] pd3dxvNormals;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMVECTOR), sizeof(XMVECTOR) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
#endif
	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");

	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fRadius, fRadius, fRadius };
}

CSphereMeshIlluminated::~CSphereMeshIlluminated()
{
}

//------------------------------------------------------------------------------------------------
CCubeMeshTextured::CCubeMeshTextured(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth)
{
	m_nVertices = 36;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pPositions = new XMFLOAT3[m_nVertices];
	int i = 0;
	m_pPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, -fz);

	m_pPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	m_pPositions[i++] = XMFLOAT3(-fx, -fy, -fz);

	m_pPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, -fz);

	m_pPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(-fx, +fy, -fz);

	m_pPositions[i++] = XMFLOAT3(-fx, -fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, +fz);

	m_pPositions[i++] = XMFLOAT3(-fx, -fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(-fx, +fy, +fz);

	m_pPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, +fz);

	m_pPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pPositions[i++] = XMFLOAT3(-fx, -fy, +fz);

	m_pPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(-fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(-fx, -fy, -fz);

	m_pPositions[i++] = XMFLOAT3(-fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(-fx, -fy, -fz);
	m_pPositions[i++] = XMFLOAT3(-fx, -fy, +fz);

	m_pPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, +fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, +fz);

	m_pPositions[i++] = XMFLOAT3(+fx, +fy, -fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, +fz);
	m_pPositions[i++] = XMFLOAT3(+fx, -fy, -fz);
	
	XMFLOAT2 pd3dxvTexCoords[36];
	i = 0;
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);

	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 0.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(1.0f, 1.0f);
	pd3dxvTexCoords[i++] = XMFLOAT2(0.0f, 1.0f);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fx, fy, fz };
	
	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");
}

CCubeMeshTextured::~CCubeMeshTextured()
{
}

//------------------------------------------------------------------------------------------------
CSphereMeshTextured::CSphereMeshTextured(ID3D11Device *pd3dDevice, float fRadius, int nSlices, int nStacks)
{
	m_nVertices = (nSlices * nStacks) * 3 * 2;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	float theta_i, theta_ii, phi_j, phi_jj, fRadius_j, fRadius_jj, y_j, y_jj;
	for (int j = 0, k = 0; j < nStacks; j++)
	{
		phi_j = float(D3DX_PI / nStacks) * j;
		phi_jj = float(D3DX_PI / nStacks) * (j + 1);
		fRadius_j = fRadius * sinf(phi_j);
		fRadius_jj = fRadius * sinf(phi_jj);
		y_j = fRadius*cosf(phi_j);
		y_jj = fRadius*cosf(phi_jj);
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = float(2 * D3DX_PI / nSlices) * i;
			theta_ii = float(2 * D3DX_PI / nSlices) * (i + 1);
			m_pPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_i), y_j, fRadius_j*sinf(theta_i));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i) / float(nSlices), float(j) / float(nStacks));
			m_pPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i) / float(nSlices), float(j + 1) / float(nStacks));
			m_pPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i + 1) / float(nSlices), float(j) / float(nStacks));
			m_pPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i) / float(nSlices), float(j + 1) / float(nStacks));
			m_pPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_ii), y_jj, fRadius_jj*sinf(theta_ii));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i + 1) / float(nSlices), float(j + 1) / float(nStacks));
			m_pPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
			pd3dxvTexCoords[k++] = XMFLOAT2(float(i + 1) / float(nSlices), float(j) / float(nStacks));
		}
	}

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fRadius, fRadius, fRadius };
	
	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");
}

CSphereMeshTextured::~CSphereMeshTextured()
{
}

//------------------------------------------------------------------------------------------------
CCubeMeshTexturedIlluminated::CCubeMeshTexturedIlluminated(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth)
{
	m_nVertices = 24;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT2* pTexCoords = new XMFLOAT2[m_nVertices];
	
	// Fill in the front face vertex data.
	m_pPositions[0] = XMFLOAT3(-fx, -fy, -fz); pTexCoords[0] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[1] = XMFLOAT3(-fx, +fy, -fz); pTexCoords[1] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[2] = XMFLOAT3(+fx, +fy, -fz); pTexCoords[2] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[3] = XMFLOAT3(+fx, -fy, -fz); pTexCoords[3] = XMFLOAT2(1.0f, 1.0f);
											   
	// Fill in the back face vertex data.	   
	m_pPositions[4] = XMFLOAT3(-fx, -fy, +fz); pTexCoords[4] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[5] = XMFLOAT3(+fx, -fy, +fz); pTexCoords[5] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[6] = XMFLOAT3(+fx, +fy, +fz); pTexCoords[6] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[7] = XMFLOAT3(-fx, +fy, +fz); pTexCoords[7] = XMFLOAT2(1.0f, 0.0f);

	// Fill in the top face vertex data.
	m_pPositions[8] = XMFLOAT3(-fx, +fy, -fz); pTexCoords[8] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[9] = XMFLOAT3(-fx, +fy, +fz); pTexCoords[9] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[10] = XMFLOAT3(+fx, +fy, +fz); pTexCoords[10] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[11] = XMFLOAT3(+fx, +fy, -fz); pTexCoords[11] = XMFLOAT2(1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	m_pPositions[12] = XMFLOAT3(-fx, -fy, -fz); pTexCoords[12] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[13] = XMFLOAT3(+fx, -fy, -fz); pTexCoords[13] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[14] = XMFLOAT3(+fx, -fy, +fz); pTexCoords[14] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[15] = XMFLOAT3(-fx, -fy, +fz); pTexCoords[15] = XMFLOAT2(1.0f, 0.0f);

	// Fill in the left face vertex data.
	m_pPositions[16] = XMFLOAT3(-fx, -fy, +fz); pTexCoords[16] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[17] = XMFLOAT3(-fx, +fy, +fz); pTexCoords[17] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[18] = XMFLOAT3(-fx, +fy, -fz); pTexCoords[18] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[19] = XMFLOAT3(-fx, -fy, -fz); pTexCoords[19] = XMFLOAT2(1.0f, 1.0f);

	// Fill in the right face vertex data.
	m_pPositions[20] = XMFLOAT3(+fx, -fy, -fz); pTexCoords[20] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[21] = XMFLOAT3(+fx, +fy, -fz); pTexCoords[21] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[22] = XMFLOAT3(+fx, +fy, +fz); pTexCoords[22] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[23] = XMFLOAT3(+fx, -fy, +fz); pTexCoords[23] = XMFLOAT2(1.0f, 1.0f);
											   
											   
	m_nIndices = 36;						   
	m_pnIndices = new UINT[m_nIndices];

	// Fill in the front face index data
	m_pnIndices[0] = 0; m_pnIndices[1] = 1; m_pnIndices[2] = 2;
	m_pnIndices[3] = 0; m_pnIndices[4] = 2; m_pnIndices[5] = 3;

	// Fill in the back face index data
	m_pnIndices[6] = 4; m_pnIndices[7] = 5; m_pnIndices[8] = 6;
	m_pnIndices[9] = 4; m_pnIndices[10] = 6; m_pnIndices[11] = 7;

	// Fill in the top face index data
	m_pnIndices[12] = 8; m_pnIndices[13] = 9; m_pnIndices[14] = 10;
	m_pnIndices[15] = 8; m_pnIndices[16] = 10; m_pnIndices[17] = 11;

	// Fill in the bottom face index data
	m_pnIndices[18] = 12; m_pnIndices[19] = 13; m_pnIndices[20] = 14;
	m_pnIndices[21] = 12; m_pnIndices[22] = 14; m_pnIndices[23] = 15;

	// Fill in the left face index data
	m_pnIndices[24] = 16; m_pnIndices[25] = 17; m_pnIndices[26] = 18;
	m_pnIndices[27] = 16; m_pnIndices[28] = 18; m_pnIndices[29] = 19;

	// Fill in the right face index data
	m_pnIndices[30] = 20; m_pnIndices[31] = 21; m_pnIndices[32] = 22;
	m_pnIndices[33] = 20; m_pnIndices[34] = 22; m_pnIndices[35] = 23;

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");

	XMFLOAT3 pd3dxvNormals[36];
	CalculateVertexNormal(pd3dxvNormals);
	
	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");

	ID3D11Buffer *pd3dBuffers[] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	
	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fx, fy, fz };

	delete pTexCoords;
}

CCubeMeshTexturedIlluminated::~CCubeMeshTexturedIlluminated()
{
}

//------------------------------------------------------------------------------------------------
CSphereMeshTexturedIlluminated::CSphereMeshTexturedIlluminated(ID3D11Device *pd3dDevice, float fRadius, int nSlices, int nStacks)
{
	m_nVertices = (nSlices * nStacks) * 3 * 2;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT3 *pd3dxvNormals = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];

	float theta_i, theta_ii, phi_j, phi_jj, fRadius_j, fRadius_jj, y_j, y_jj;
	for (int j = 0, k = 0; j < nStacks; j++)
	{
		phi_j = float(D3DX_PI / nStacks) * j;
		phi_jj = float(D3DX_PI / nStacks) * (j + 1);
		fRadius_j = fRadius * sinf(phi_j);
		fRadius_jj = fRadius * sinf(phi_jj);
		y_j = fRadius*cosf(phi_j);
		y_jj = fRadius*cosf(phi_jj);
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = float(2 * D3DX_PI / nSlices) * i;
			theta_ii = float(2 * D3DX_PI / nSlices) * (i + 1);
			m_pPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_i), y_j, fRadius_j*sinf(theta_i));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i) / float(nSlices), float(j) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pPositions[k]))); k++;
			m_pPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i) / float(nSlices), float(j + 1) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pPositions[k]))); k++;
			m_pPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i + 1) / float(nSlices), float(j) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pPositions[k]))); k++;
			m_pPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i) / float(nSlices), float(j + 1) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pPositions[k]))); k++;
			m_pPositions[k] = XMFLOAT3(fRadius_jj*cosf(theta_ii), y_jj, fRadius_jj*sinf(theta_ii));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i + 1) / float(nSlices), float(j + 1) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pPositions[k]))); k++;
			m_pPositions[k] = XMFLOAT3(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii));
			pd3dxvTexCoords[k] = XMFLOAT2(float(i + 1) / float(nSlices), float(j) / float(nStacks));
			XMStoreFloat3(&pd3dxvNormals[k], XMVector3Normalize(XMLoadFloat3(&m_pPositions[k]))); k++;
		}
	}

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pd3dxvNormals;
	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	
	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fRadius, fRadius, fRadius };

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");
}

CSphereMeshTexturedIlluminated::~CSphereMeshTexturedIlluminated()
{
}

//------------------------------------------------------------------------------------------------

CCubeMeshTexturedTangenteIlluminated::CCubeMeshTexturedTangenteIlluminated(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth)
{
	m_nVertices = 24;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;
	
	m_pPositions = new XMFLOAT3[m_nVertices];
	m_pTexCoords = new XMFLOAT2[m_nVertices];

	// Fill in the front face vertex data.
	m_pPositions[0] = XMFLOAT3(-fx, -fy, -fz); m_pTexCoords[0] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[1] = XMFLOAT3(-fx, +fy, -fz); m_pTexCoords[1] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[2] = XMFLOAT3(+fx, +fy, -fz); m_pTexCoords[2] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[3] = XMFLOAT3(+fx, -fy, -fz); m_pTexCoords[3] = XMFLOAT2(1.0f, 1.0f);

	// Fill in the back face vertex data.
	m_pPositions[4] = XMFLOAT3(-fx, -fy, +fz); m_pTexCoords[4] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[5] = XMFLOAT3(+fx, -fy, +fz); m_pTexCoords[5] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[6] = XMFLOAT3(+fx, +fy, +fz); m_pTexCoords[6] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[7] = XMFLOAT3(-fx, +fy, +fz); m_pTexCoords[7] = XMFLOAT2(1.0f, 0.0f);

	// Fill in the top face vertex data.
	m_pPositions[8] = XMFLOAT3(-fx, +fy, -fz); m_pTexCoords[8] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[9] = XMFLOAT3(-fx, +fy, +fz); m_pTexCoords[9] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[10] = XMFLOAT3(+fx, +fy, +fz); m_pTexCoords[10] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[11] = XMFLOAT3(+fx, +fy, -fz); m_pTexCoords[11] = XMFLOAT2(1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	m_pPositions[12] = XMFLOAT3(-fx, -fy, -fz); m_pTexCoords[12] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[13] = XMFLOAT3(+fx, -fy, -fz); m_pTexCoords[13] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[14] = XMFLOAT3(+fx, -fy, +fz); m_pTexCoords[14] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[15] = XMFLOAT3(-fx, -fy, +fz); m_pTexCoords[15] = XMFLOAT2(1.0f, 0.0f);

	// Fill in the left face vertex data.
	m_pPositions[16] = XMFLOAT3(-fx, -fy, +fz); m_pTexCoords[16] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[17] = XMFLOAT3(-fx, +fy, +fz); m_pTexCoords[17] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[18] = XMFLOAT3(-fx, +fy, -fz); m_pTexCoords[18] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[19] = XMFLOAT3(-fx, -fy, -fz); m_pTexCoords[19] = XMFLOAT2(1.0f, 1.0f);

	// Fill in the right face vertex data.
	m_pPositions[20] = XMFLOAT3(+fx, -fy, -fz); m_pTexCoords[20] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[21] = XMFLOAT3(+fx, +fy, -fz); m_pTexCoords[21] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[22] = XMFLOAT3(+fx, +fy, +fz); m_pTexCoords[22] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[23] = XMFLOAT3(+fx, -fy, +fz); m_pTexCoords[23] = XMFLOAT2(1.0f, 1.0f);

	
	m_nIndices = 36;
	m_pnIndices = new UINT[m_nIndices];

	// Fill in the front face index data
	m_pnIndices[0] = 0; m_pnIndices[1] = 1; m_pnIndices[2] = 2;
	m_pnIndices[3] = 0; m_pnIndices[4] = 2; m_pnIndices[5] = 3;

	// Fill in the back face index data
	m_pnIndices[6] = 4; m_pnIndices[7] = 5; m_pnIndices[8] = 6;
	m_pnIndices[9] = 4; m_pnIndices[10] = 6; m_pnIndices[11] = 7;

	// Fill in the top face index data
	m_pnIndices[12] = 8; m_pnIndices[13] = 9; m_pnIndices[14] = 10;
	m_pnIndices[15] = 8; m_pnIndices[16] = 10; m_pnIndices[17] = 11;
	
	// Fill in the bottom face index data
	m_pnIndices[18] = 12; m_pnIndices[19] = 13; m_pnIndices[20] = 14;
	m_pnIndices[21] = 12; m_pnIndices[22] = 14; m_pnIndices[23] = 15;

	// Fill in the left face index data
	m_pnIndices[24] = 16; m_pnIndices[25] = 17; m_pnIndices[26] = 18;
	m_pnIndices[27] = 16; m_pnIndices[28] = 18; m_pnIndices[29] = 19;

	// Fill in the right face index data
	m_pnIndices[30] = 20; m_pnIndices[31] = 21; m_pnIndices[32] = 22;
	m_pnIndices[33] = 20; m_pnIndices[34] = 22; m_pnIndices[35] = 23;

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");

	m_pNormals = new XMFLOAT3[m_nVertices];
	m_pTangents = new XMFLOAT3[m_nVertices];
	CalculateVertexNormal(m_pNormals);
	CalculateVertexTangent(m_pTangents);
	
	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTangentBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pTangents, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	DXUT_SetDebugName(m_pd3dTangentBuffer, "Tangent");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");

	ID3D11Buffer *pd3dBuffers[] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTangentBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[] = { 0, 0, 0, 0 };
	AssembleToVertexBuffer(4, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fx, fy, fz };

	delete[] m_pTexCoords;
	delete[] m_pNormals;
	delete[] m_pTangents;
}

CCubeMeshTexturedTangenteIlluminated::~CCubeMeshTexturedTangenteIlluminated()
{
}

CCubePatchMesh::CCubePatchMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth)
{
	m_nVertices = 24;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pPositions = new XMFLOAT3[m_nVertices];
	m_pTexCoords = new XMFLOAT2[m_nVertices];

	// Fill in the front face vertex data.
	m_pPositions[0] = XMFLOAT3(-fx, -fy, -fz); m_pTexCoords[0] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[1] = XMFLOAT3(-fx, +fy, -fz); m_pTexCoords[1] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[2] = XMFLOAT3(+fx, +fy, -fz); m_pTexCoords[2] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[3] = XMFLOAT3(+fx, -fy, -fz); m_pTexCoords[3] = XMFLOAT2(1.0f, 1.0f);

	// Fill in the back face vertex data.
	m_pPositions[4] = XMFLOAT3(-fx, -fy, +fz); m_pTexCoords[4] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[5] = XMFLOAT3(+fx, -fy, +fz); m_pTexCoords[5] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[6] = XMFLOAT3(+fx, +fy, +fz); m_pTexCoords[6] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[7] = XMFLOAT3(-fx, +fy, +fz); m_pTexCoords[7] = XMFLOAT2(1.0f, 0.0f);

	// Fill in the top face vertex data.
	m_pPositions[8] = XMFLOAT3(-fx, +fy, -fz); m_pTexCoords[8] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[9] = XMFLOAT3(-fx, +fy, +fz); m_pTexCoords[9] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[10] = XMFLOAT3(+fx, +fy, +fz); m_pTexCoords[10] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[11] = XMFLOAT3(+fx, +fy, -fz); m_pTexCoords[11] = XMFLOAT2(1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	m_pPositions[12] = XMFLOAT3(-fx, -fy, -fz); m_pTexCoords[12] = XMFLOAT2(1.0f, 1.0f);
	m_pPositions[13] = XMFLOAT3(+fx, -fy, -fz); m_pTexCoords[13] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[14] = XMFLOAT3(+fx, -fy, +fz); m_pTexCoords[14] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[15] = XMFLOAT3(-fx, -fy, +fz); m_pTexCoords[15] = XMFLOAT2(1.0f, 0.0f);

	// Fill in the left face vertex data.
	m_pPositions[16] = XMFLOAT3(-fx, -fy, +fz); m_pTexCoords[16] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[17] = XMFLOAT3(-fx, +fy, +fz); m_pTexCoords[17] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[18] = XMFLOAT3(-fx, +fy, -fz); m_pTexCoords[18] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[19] = XMFLOAT3(-fx, -fy, -fz); m_pTexCoords[19] = XMFLOAT2(1.0f, 1.0f);

	// Fill in the right face vertex data.
	m_pPositions[20] = XMFLOAT3(+fx, -fy, -fz); m_pTexCoords[20] = XMFLOAT2(0.0f, 1.0f);
	m_pPositions[21] = XMFLOAT3(+fx, +fy, -fz); m_pTexCoords[21] = XMFLOAT2(0.0f, 0.0f);
	m_pPositions[22] = XMFLOAT3(+fx, +fy, +fz); m_pTexCoords[22] = XMFLOAT2(1.0f, 0.0f);
	m_pPositions[23] = XMFLOAT3(+fx, -fy, +fz); m_pTexCoords[23] = XMFLOAT2(1.0f, 1.0f);


	m_nIndices = 36;
	m_pnIndices = new UINT[m_nIndices];

	// Fill in the front face index data
	m_pnIndices[0] = 0; m_pnIndices[1] = 1; m_pnIndices[2] = 2;
	m_pnIndices[3] = 0; m_pnIndices[4] = 2; m_pnIndices[5] = 3;

	// Fill in the back face index data
	m_pnIndices[6] = 4; m_pnIndices[7] = 5; m_pnIndices[8] = 6;
	m_pnIndices[9] = 4; m_pnIndices[10] = 6; m_pnIndices[11] = 7;

	// Fill in the top face index data
	m_pnIndices[12] = 8; m_pnIndices[13] = 9; m_pnIndices[14] = 10;
	m_pnIndices[15] = 8; m_pnIndices[16] = 10; m_pnIndices[17] = 11;

	// Fill in the bottom face index data
	m_pnIndices[18] = 12; m_pnIndices[19] = 13; m_pnIndices[20] = 14;
	m_pnIndices[21] = 12; m_pnIndices[22] = 14; m_pnIndices[23] = 15;

	// Fill in the left face index data
	m_pnIndices[24] = 16; m_pnIndices[25] = 17; m_pnIndices[26] = 18;
	m_pnIndices[27] = 16; m_pnIndices[28] = 18; m_pnIndices[29] = 19;

	// Fill in the right face index data
	m_pnIndices[30] = 20; m_pnIndices[31] = 21; m_pnIndices[32] = 22;
	m_pnIndices[33] = 20; m_pnIndices[34] = 22; m_pnIndices[35] = 23;

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");

	m_pNormals = new XMFLOAT3[m_nVertices];
	m_pTangents = new XMFLOAT3[m_nVertices];
	CalculateVertexNormal(m_pNormals);
	CalculateVertexTangent(m_pTangents);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTangentBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pTangents, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	DXUT_SetDebugName(m_pd3dTangentBuffer, "Tangent");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");

	ID3D11Buffer *pd3dBuffers[] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTangentBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[] = { 0, 0, 0, 0 };
	AssembleToVertexBuffer(4, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	m_bcBoundingBox.Extents = { fx, fy, fz };

	delete[] m_pTexCoords;
	delete[] m_pNormals;
	delete[] m_pTangents;
}

CCubePatchMesh::~CCubePatchMesh()
{
}