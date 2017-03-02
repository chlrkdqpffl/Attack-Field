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

	// ������, �ѱ���, �� ��Ÿ�, ���߷�, ����ӵ�, ��ź �� 
public:
	virtual void Fire();
};

