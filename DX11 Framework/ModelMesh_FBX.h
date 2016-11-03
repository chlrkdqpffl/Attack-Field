#pragma once
#include "Mesh.h"


class CModelMesh_FBX : public CMeshTexturedIlluminated
{
	vector<XMFLOAT3> posVector, normalVector, indexVector;
	vector<XMFLOAT2> uvVector;

	XMFLOAT3		*m_pvNormals;
	XMFLOAT2		*m_pvTexCoords;

public:
	CModelMesh_FBX(const string&, float size = 1.0f);
	virtual ~CModelMesh_FBX();

	void LoadFBXfromFile(const string& fileName);
};