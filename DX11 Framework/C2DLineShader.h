#pragma once
#include "Shader.h"

class C2DLineShader : public CShader
{
public:
	C2DLineShader();
	virtual ~C2DLineShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
};