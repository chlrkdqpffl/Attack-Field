#pragma once
#include "SkinnedObject.h"
#include "Weapon.h"

class CCharacterObject : public CSkinnedObject
{
public:
	CCharacterObject();
	virtual ~CCharacterObject();

protected:
	CWeapon*			m_pWeapon;


public:
	virtual void CreateWeapon(ID3D11Device *pd3dDevice);
};

