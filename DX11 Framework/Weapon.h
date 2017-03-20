#pragma once
#include "Object.h"

class CCharacterObject;

class CWeapon :	public CGameObject
{
public:
	CWeapon(CCharacterObject* pOwner);
	virtual ~CWeapon();

protected:
	CCharacterObject*		m_pOwner;

	UINT					m_nBoneIndex = 0;
	XMMATRIX				m_mtxParent;
	XMMATRIX				m_mtxOffset;
	WeaponData::Type		m_Type;

	float					m_fDamage;
	float					m_fBulletSpeed;
	float					m_fFireSpeed;
	float					m_fEffectiveRange;
	ULONGLONG				m_lLastAttackTime;

	// 데미지, 총구속, 총 사거리, 명중률, 연사속도, 장탄 수 
public:
	virtual void Fire();
	virtual void Update(float fTimeElapsed) override;

	// ----- Get, Setter ----- //
};