#pragma once
#include "Mesh.h"


class CModelMesh_FBX : public CMeshTexturedIlluminated
{
	vector<XMFLOAT3>	posVector, normalVector, indexVector;
	vector<XMFLOAT2>	uvVector;

	XMFLOAT3			*m_pNormals	= nullptr;
	XMFLOAT2			*m_pTexCoords	= nullptr;
	XMFLOAT3			*m_pTangents	= nullptr;

	ID3D11Buffer		*m_pd3dTangentBuffer = nullptr;
public:
	CModelMesh_FBX(const string&, float size = 1.0f);
	virtual ~CModelMesh_FBX();

	bool LoadFBXfromFile(const string& fileName);
};