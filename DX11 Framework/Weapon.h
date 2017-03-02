#pragma once
#include "Object.h"
//#include "CharacterObject.h"

class CWeapon :	public CGameObject
{
public:
	CWeapon();
	virtual ~CWeapon();

protected:
	WeaponData::Type	m_Type;
	float				m_fDamage;
	float				m_fBulletSpeed;
	float				m_fFireSpeed;
	float				m_fEffectiveRange;
	ULONGLONG			m_lLastAttackTime;
	//Unit* user;

	// 데미지, 총구속, 총 사거리, 명중률, 연사속도, 장탄 수 
public:
	virtual void Fire();
};

