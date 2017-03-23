#pragma once
#include "Weapon.h"

class CRifleGunWeapon : public CWeapon
{
public:
	CRifleGunWeapon(ID3D11Device* pd3dDevice, CCharacterObject* pOwner);
	virtual ~CRifleGunWeapon();

	virtual void CreateMesh(ID3D11Device *pd3dDevice);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateMaterial();
};