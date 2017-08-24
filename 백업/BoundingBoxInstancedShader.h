#pragma once
#include "BoundingBoxShader.h"

class CBoundingBoxInstancedShader :	public CBoundingBoxShader
{

public:
	CBoundingBoxInstancedShader();
	virtual ~CBoundingBoxInstancedShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
};

