#pragma once
#include "Shader.h"
class CNormalMapShader : public CShader
{
public:
	CNormalMapShader(ID3D11Device* pd3dDevice);
	virtual ~CNormalMapShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
};

