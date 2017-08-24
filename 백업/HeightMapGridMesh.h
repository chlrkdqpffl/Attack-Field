#pragma once
#include "Mesh.h"

class CHeightMapGridMesh : public CMeshDetailTexturedIlluminated
{
protected:
	int							m_nWidth;
	int							m_nLength;
	XMFLOAT3					m_d3dxvScale;

public:
	CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, XMVECTOR d3dxvScale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), void *pContext = NULL, D3D11_USAGE d3dUsage = D3D11_USAGE_DEFAULT);
	virtual ~CHeightMapGridMesh();

	XMVECTOR& GetScale() { return(XMLoadFloat3(&m_d3dxvScale)); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	virtual float OnGetHeight(int x, int z, void *pContext);
};