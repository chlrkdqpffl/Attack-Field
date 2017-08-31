#pragma once
#include "Shader.h"

class CSpriteImageShader : public CShader
{
public:
	CSpriteImageShader();
	virtual ~CSpriteImageShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
};