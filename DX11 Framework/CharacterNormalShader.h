#pragma once
#include "CharacterShader.h"

class CCharacterNormalShader : public CCharacterShader
{
public:
	CCharacterNormalShader();
	virtual ~CCharacterNormalShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
};

