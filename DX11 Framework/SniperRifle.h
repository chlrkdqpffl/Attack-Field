#pragma once
#include "Weapon.h"

class CSniperRifle : public CWeapon
{
public:
	CSniperRifle(CCharacterObject* pOwner);
	virtual ~CSniperRifle();

	virtual void CreateMesh(ID3D11Device *pd3dDevice) override;
	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void CreateMaterial() override;

	virtual void Update(float fDeltaTime) override;
};