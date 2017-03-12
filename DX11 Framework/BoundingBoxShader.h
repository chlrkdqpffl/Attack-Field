#pragma once
#include "Shader.h"

class CBoundingBoxShader : public CShader
{
public:
	CBoundingBoxShader();
	virtual ~CBoundingBoxShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
};