#pragma once
#include "Mesh.h"

class CSpriteImageMesh : public CMeshTextured
{
public:
	CSpriteImageMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, UINT nCount);
	virtual ~CSpriteImageMesh();
};