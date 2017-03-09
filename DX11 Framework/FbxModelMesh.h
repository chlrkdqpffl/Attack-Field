#pragma once
#include "Mesh.h"
#include "FbxMeshData.h"

class CFbxModelMesh : public CMeshTexturedIlluminated
{
protected:
	CFbxMeshData			m_meshData;
	MeshTag					m_meshTag;

	float					m_fModelSize	= 1.0f;

	XMFLOAT3				*m_pNormals		= nullptr;
	XMFLOAT2				*m_pTexCoords	= nullptr;
	XMFLOAT3				*m_pTangents	= nullptr;

	ID3D11Buffer			*m_pd3dTangentBuffer = nullptr;

public:
	CFbxModelMesh(ID3D11Device *pd3dDevice, const MeshTag meshTag, const float size = 1.0f);
	virtual ~CFbxModelMesh();

public:
	virtual void Initialize(ID3D11Device *pd3dDevice);
	
	float GetModelSize() const { return m_fModelSize; }
};