#pragma once
#include "Object.h"
#include "BulletObject.h"

class CCharacterObject;

class CWeapon :	public CGameObject
{
public:
	CWeapon(ID3D11Device* pd3dDevice, CCharacterObject* pOwner);
	virtual ~CWeapon();

protected:
	ID3D11Device*			m_pd3dDevice;
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

	vector<CBulletObject*>	m_vecBulletContainer;

	// ������, �ѱ���, �� ��Ÿ�, ���߷�, ����ӵ�, ��ź �� 
public:
	virtual void Fire();
	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	// ----- Get, Setter ----- //

};