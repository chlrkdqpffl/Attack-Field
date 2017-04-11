#pragma once
#include "Mesh.h"
#include "FbxMeshData.h"

class CFbxModelMesh : public CMeshTexturedIlluminated
{
protected:
	CFbxMeshData			m_meshData;

	XMFLOAT3				m_fModelSize	= XMFLOAT3(1.f, 1.f, 1.f);

	XMFLOAT3				*m_pNormals		= nullptr;
	XMFLOAT2				*m_pTexCoords	= nullptr;
	XMFLOAT3				*m_pTangents	= nullptr;

	ID3D11Buffer			*m_pd3dTangentBuffer = nullptr;

public:
	CFbxModelMesh(ID3D11Device *pd3dDevice, const MeshTag meshTag, const XMFLOAT3 size = XMFLOAT3(1.f, 1.f, 1.f));
	virtual ~CFbxModelMesh();

public:
	virtual void Initialize(ID3D11Device *pd3dDevice, bool isCalcTangent = false);
	
	XMFLOAT3 GetModelSize() const { return m_fModelSize; }
	void CalculateVertexTangent(XMVECTOR *pd3dxvTangents);
	XMVECTOR CalculateTriAngleTangent(UINT nIndex0, UINT nIndex1, UINT nIndex2);
};