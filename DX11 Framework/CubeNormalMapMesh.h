#pragma once
#include "MeshNormalMap.h"

class CCubeNormalMapMesh : public CMeshNormalMap
{
public:
	CCubeNormalMapMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeNormalMapMesh();
};

