#pragma once
#include "Mesh.h"

class CNormalMapCubeMesh : public CMeshNormalMap
{
public:
	CNormalMapCubeMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CNormalMapCubeMesh();
};

