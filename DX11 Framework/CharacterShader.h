#pragma once
#include "Shader.h"

class CCharacterShader : public CShader
{
public:
	CCharacterShader();
	virtual ~CCharacterShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
};

