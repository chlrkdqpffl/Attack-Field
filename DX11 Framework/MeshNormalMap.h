#pragma once
#include "Mesh.h"

class CMeshNormalMap : public CMeshTexturedIlluminated
{
public:
	CMeshNormalMap(ID3D11Device *pd3dDevice);
	virtual ~CMeshNormalMap();

	XMVECTOR CalculateTriAngleTangent(UINT nIndex0, UINT nIndex1, UINT nIndex2);

	void SetTriAngleListVertexTangent(XMVECTOR *pd3dxvTangents);
	void SetAverageVertexTangent(XMVECTOR *pd3dxvTangents, int nPrimitives, int nOffset, bool bStrip);
	void CalculateVertexTangent(XMVECTOR *pd3dxvTangents);

protected:
	XMFLOAT2				*m_pTexCoords = nullptr;
	XMFLOAT3				*m_pNormals = nullptr;
	XMFLOAT3				*m_pTangents = nullptr;

	ID3D11Buffer			*m_pd3dTangentBuffer = nullptr;
};

