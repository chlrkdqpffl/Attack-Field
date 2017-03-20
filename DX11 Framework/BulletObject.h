#pragma once
#include "Object.h"

class CBulletObject : public CGameObject
{
public:
	CBulletObject();
	virtual ~CBulletObject();

	virtual void CreateMesh(ID3D11Device *pd3dDevice);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateMaterial(ID3D11Device *pd3dDevice);
};

