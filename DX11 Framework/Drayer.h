#pragma once
#include "SkinnedObject.h"


class CDrayer :	public CSkinnedObject
{
public:
	CDrayer();
	virtual ~CDrayer();

	virtual void CreateMaterial(ID3D11Device *pd3dDevice);
	virtual void CreateMesh(ID3D11Device *pd3dDevice);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateAnimation();
};

