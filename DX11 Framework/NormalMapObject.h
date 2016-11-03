#pragma once
#include "Object.h"
class CNormalMapObject : public CGameObject
{
public:
	CNormalMapObject();
	virtual ~CNormalMapObject();

	void CreateTexture(ID3D11Device *pd3dDevice);
};

