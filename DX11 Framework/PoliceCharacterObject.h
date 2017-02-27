#pragma once
#include "SkinnedObject.h"
#include "CharacterNormalShader.h"

class CPoliceCharacterObject : public CSkinnedObject
{
public:
	CPoliceCharacterObject();
	virtual ~CPoliceCharacterObject();

	virtual void CreateMaterial(ID3D11Device *pd3dDevice);
	virtual void CreateMesh(ID3D11Device *pd3dDevice);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateAnimation();
};

