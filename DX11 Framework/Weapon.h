#pragma once
#include "Object.h"

class CCharacterObject;

class CWeapon :	public CGameObject
{
public:
	CWeapon();
	virtual ~CWeapon();

protected:
	XMMATRIX				m_mtxParent;
	CFbxModelSkinnedMesh*	m_pSkinnedMesh;
	CCharacterObject*		m_pCharacterObject;
	WeaponData::Type		m_Type;

	float					m_fDamage;
	float					m_fBulletSpeed;
	float					m_fFireSpeed;
	float					m_fEffectiveRange;
	ULONGLONG				m_lLastAttackTime;
	//Unit* user;

	// ������, �ѱ���, �� ��Ÿ�, ���߷�, ����ӵ�, ��ź �� 
public:
	virtual void Fire();
	virtual void Update(float fTimeElapsed) override;

	// ----- Get, Setter ----- //
	void SetParentMtx(XMMATRIX mtx) { m_mtxParent = mtx; }
	void SetSkinnedMesh(CFbxModelSkinnedMesh* pMesh) { m_pSkinnedMesh = pMesh; }
	void SetCharacterObject(CCharacterObject* pObj) { m_pCharacterObject = pObj; }
};

