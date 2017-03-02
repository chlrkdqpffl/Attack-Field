#pragma once
#include "Weapon.h"

class CRifleGunWeapon : public CWeapon
{
public:
	CRifleGunWeapon();
	virtual ~CRifleGunWeapon();

	virtual void CreateMesh(ID3D11Device *pd3dDevice);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateMaterial(ID3D11Device *pd3dDevice);
};

