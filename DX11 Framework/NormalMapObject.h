#pragma once
#include "Object.h"
class CNormalMapObject : public CGameObject
{
public:
	CNormalMapObject();
	virtual ~CNormalMapObject();

	void CreateMaterial(ID3D11Device *pd3dDevice);
};

