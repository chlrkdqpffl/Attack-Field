#pragma once
#include "Mesh.h"
#include "FbxMeshData.h"

class CFbxModelMesh : public CMeshNormalMap
{
protected:
	CFbxMeshData			m_meshData;

	XMFLOAT3				m_fModelSize	= XMFLOAT3(1.f, 1.f, 1.f);
public:
	CFbxModelMesh() {}
	CFbxModelMesh(ID3D11Device *pd3dDevice, const MeshTag meshTag, const XMFLOAT3 size = XMFLOAT3(1.f, 1.f, 1.f));
	virtual ~CFbxModelMesh();

public:
	virtual void Initialize(ID3D11Device *pd3dDevice, bool isCalcTangent = false);
	
	XMFLOAT3 GetModelSize() const { return m_fModelSize; }
};