#pragma once
#include "Weapon.h"
#include "RifleBullet.h"

class CRifleGunWeapon : public CWeapon
{
public:
	CRifleGunWeapon(CCharacterObject* pOwner);
	virtual ~CRifleGunWeapon();

	virtual void CreateMesh(ID3D11Device *pd3dDevice) override;
	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void CreateMaterial() override;
	virtual void Update(float fDeltaTime) override;

	virtual void FireRecoil() override;
	virtual void UpdateRecoil(float fDeltaTime) override;
};