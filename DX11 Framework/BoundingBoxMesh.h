#pragma once
#include "Mesh.h"

class CBoundingBoxMesh : public CMesh
{
public:
	CBoundingBoxMesh(ID3D11Device *pd3dDevice, BoundingBox bbox, float fSize = 1.0f);
	virtual ~CBoundingBoxMesh();
};

