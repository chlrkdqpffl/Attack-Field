#pragma once
#include "Mesh.h"

class CBoundingBoxMesh : public CMesh
{
public:
	CBoundingBoxMesh(ID3D11Device *pd3dDevice, BoundingBox bbox, float fSize = 1.0f);
	CBoundingBoxMesh(ID3D11Device *pd3dDevice, BoundingOrientedBox bbox, float fSize = 1.0f);
	virtual ~CBoundingBoxMesh();
};

