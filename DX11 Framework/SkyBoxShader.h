#pragma once
#include "Shader.h"

class CSkyBoxShader : public CShader
{
public:
	CSkyBoxShader();
	virtual ~CSkyBoxShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
};