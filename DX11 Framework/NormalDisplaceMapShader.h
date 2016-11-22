#pragma once
#include "Shader.h"

class CNormalDisplaceMapShader : public CShader
{
public:
	CNormalDisplaceMapShader();
	virtual ~CNormalDisplaceMapShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
};

