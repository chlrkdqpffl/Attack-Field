#include "stdafx.h"
#include "HeightMapGridMesh.h"


CHeightMapGridMesh::CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, XMVECTOR d3dxvScale, void *pContext, D3D11_USAGE d3dUsage) : CMeshDetailTexturedIlluminated(pd3dDevice)
{
	m_nVertices = nWidth * nLength;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_pPositions = new XMFLOAT3[m_nVertices];
	XMFLOAT3 *pd3dxvNormals = new XMFLOAT3[m_nVertices];
	XMFLOAT2 *pd3dxvTexCoords = new XMFLOAT2[m_nVertices];
	XMFLOAT2 *pd3dxvDetailTexCoords = new XMFLOAT2[m_nVertices];

	m_nWidth = nWidth;
	m_nLength = nLength;
	XMStoreFloat3(&m_d3dxvScale, d3dxvScale);

	CHeightMap *pHeightMap = (CHeightMap *)pContext;
	int cxHeightMap = (pHeightMap) ? pHeightMap->GetHeightMapWidth() : nWidth;
	int czHeightMap = (pHeightMap) ? pHeightMap->GetHeightMapLength() : nLength;
	float fHeight = 0.0f, fMinHeight = +FLT_MAX, fMaxHeight = -FLT_MAX;
	for (int i = 0, z = zStart; z < (zStart + nLength); z++)
	{
		for (int x = xStart; x < (xStart + nWidth); x++, i++)
		{
			fHeight = OnGetHeight(x, z, pContext);
			m_pPositions[i] = XMFLOAT3((x*m_d3dxvScale.x), fHeight, (z*m_d3dxvScale.z));
			pd3dxvNormals[i] = (pHeightMap) ? XMFLOAT3(XMVectorGetX(pHeightMap->GetHeightMapNormal(x, z)),
				XMVectorGetY(pHeightMap->GetHeightMapNormal(x, z)),
				XMVectorGetZ(pHeightMap->GetHeightMapNormal(x, z))) : XMFLOAT3(0.0f, 1.0f, 0.0f);
			pd3dxvTexCoords[i] = XMFLOAT2(float(x) / float(cxHeightMap - 1), float(czHeightMap - 1 - z) / float(czHeightMap - 1));
			pd3dxvDetailTexCoords[i] = XMFLOAT2(float(x) / float(m_d3dxvScale.x*0.125f), float(z) / float(m_d3dxvScale.z*0.125f));
			if (fHeight < fMinHeight) fMinHeight = fHeight;
			if (fHeight > fMaxHeight) fMaxHeight = fHeight;
		}
	}

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, d3dUsage, (d3dUsage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, d3dUsage, (d3dUsage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0);
	//m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);	- 물 표현하기 위해 USAGE를 변경한다.
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dDetailTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, pd3dxvDetailTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	if (pd3dxvNormals) delete[] pd3dxvNormals;
	if (pd3dxvTexCoords) delete[] pd3dxvTexCoords;
	if (pd3dxvDetailTexCoords) delete[] pd3dxvDetailTexCoords;

	ID3D11Buffer *pd3dBuffers[4] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer, m_pd3dDetailTexCoordBuffer };
	UINT pnBufferStrides[4] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[4] = { 0, 0, 0, 0 };
	AssembleToVertexBuffer(4, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_nIndices = ((nWidth * 2)*(nLength - 1)) + ((nLength - 1) - 1);
	m_pnIndices = new UINT[m_nIndices];
	for (int j = 0, z = 0; z < nLength - 1; z++)
	{
		if ((z % 2) == 0)
		{
			for (int x = 0; x < nWidth; x++)
			{
				if ((x == 0) && (z > 0)) m_pnIndices[j++] = (UINT)(x + (z * nWidth));
				m_pnIndices[j++] = (UINT)(x + (z * nWidth));
				m_pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
		else
		{
			for (int x = nWidth - 1; x >= 0; x--)
			{
				if (x == (nWidth - 1)) m_pnIndices[j++] = (UINT)(x + (z * nWidth));
				m_pnIndices[j++] = (UINT)(x + (z * nWidth));
				m_pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
	}

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	XMFLOAT3 d3dxvMinimum = XMFLOAT3(xStart*m_d3dxvScale.x, fMinHeight, zStart*m_d3dxvScale.z);
	XMFLOAT3 d3dxvMaximum = XMFLOAT3((xStart + nWidth)*m_d3dxvScale.x, fMaxHeight, (zStart + nLength)*m_d3dxvScale.z);

	m_bcBoundingCube.Center = {
		(d3dxvMinimum.x + d3dxvMaximum.x) * 0.5f,
		(d3dxvMinimum.y + d3dxvMaximum.y) * 0.5f,
		(d3dxvMinimum.z + d3dxvMaximum.z) * 0.5f
	};
	m_bcBoundingCube.Extents = {
		fabs(d3dxvMinimum.x - d3dxvMaximum.x) * 0.5f,
		fabs(d3dxvMinimum.y - d3dxvMaximum.y) * 0.5f,
		fabs(d3dxvMinimum.z - d3dxvMaximum.z) * 0.5f
	};

	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
	DXUT_SetDebugName(m_pd3dDetailTexCoordBuffer, "DetailTexCoord");
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");
}

CHeightMapGridMesh::~CHeightMapGridMesh()
{
}

float CHeightMapGridMesh::OnGetHeight(int x, int z, void *pContext)
{
	float fHeight = 0.0f;
	if (pContext)
	{
		CHeightMap *pHeightMap = (CHeightMap *)pContext;
		BYTE *pHeightMapImage = pHeightMap->GetHeightMapImage();
		XMVECTOR d3dxvScale = pHeightMap->GetScale();
		int nWidth = pHeightMap->GetHeightMapWidth();
		fHeight = pHeightMapImage[x + (z*nWidth)] * XMVectorGetY(d3dxvScale);
	}
	return(fHeight);
}
