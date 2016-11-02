#pragma once
#include "Mesh.h"


class CModelMesh_FBX : public CMeshTexturedIlluminated
{
	vector<XMFLOAT3> posVector, normalVector, indexVector;
	vector<XMFLOAT2> uvVector;
public:
	CModelMesh_FBX(const string&, float size = 1.0f);
	virtual ~CModelMesh_FBX();

	void LoadFBXfromFile(const string& fileName);
};