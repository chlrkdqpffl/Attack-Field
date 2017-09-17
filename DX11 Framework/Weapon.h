#pragma once
#include "Object.h"
#include "BulletObject.h"
#include "LineObject.h"

class CCharacterObject;
class CSpriteImageObject;

class CWeapon :	public CGameObject
{
public:
	CWeapon(CCharacterObject* pOwner);
	virtual ~CWeapon();

protected:
	CCharacterObject*		m_pOwner;
	CSpriteImageObject*		m_pMuzzleSpirte = nullptr;

	UINT					m_nBoneIndex = 0;
	XMMATRIX				m_mtxParent;
	XMFLOAT3				m_f3MuzzlePosition = XMFLOAT3(0, 0, 0);

	// ----- �ѱ� �ݵ� ----- //
	bool					m_bIsFire = false;
	UINT					m_nFireBulletCount = 0;				// �ѱ� �ݵ� Ȯ��
	float					m_fInitPitch = 0.0f;				// ���� �ݵ� ��ġ
	float					m_fUserMovePitch = 0.0f;			// �� �߻� �������� ����ڰ� ���� �ø� ����
	const float				m_fMaxPitchGap = 0.0f;				// �ִ� �ݵ� ����
	float					m_fCalcRecoil = 0.0f;				// ���� �ݵ���
	float					m_fNowRecoil = 0.0f;				// ���� �ݵ���	- UI�� ���

	// ------------- ���� ���� ���� ------------ //
	WeaponTag				m_tagWeapon = WeaponTag::eNone;		// �ѱ� Ÿ��
	float					m_fDamage;							// ������
	float					m_fRange = 0.f;						// ��ȿ ��Ÿ�
	float					m_fReCoil = 0.2f;					// �ѱ� �ݵ���
	UINT					m_uiFireSpeed = 0;					// ���� �ӵ� (1000 - 1��)
	DWORD					m_dwLastAttackTime = 0;				// ������ �߻� �ð�

	UINT					m_nMaxhasBulletCount = 0;			// �� �� ������ ��ź ��
	UINT					m_nhasBulletCount = 0;				// ���� ��ź ��

public:
	virtual void Firing(XMVECTOR direction);
	virtual void Update(float fDeltaTime) override;

	void Reloading();

	bool IsExistBullet() const { return ( 0 < m_nhasBulletCount); }
	UINT GetBulletCount() const { return m_nhasBulletCount; }
	UINT GetMaxBulletCount() const { return m_nMaxhasBulletCount; }
	void SetFireBulletCount(UINT set) { m_nFireBulletCount = set; }
	UINT GetFireBulletCount() const { return m_nFireBulletCount;}
	CSpriteImageObject*	GetMuzzleSprite() const { return m_pMuzzleSpirte; }
	float GetCalcRecoil() const { return m_fNowRecoil; }
	void SetUserMovePitch(float pitch) { m_fUserMovePitch = pitch; }
	WeaponTag GetWeaponTag() const { return m_tagWeapon; }

protected:
	void FireEffect();
	void FireRecoil();
	void UpdateRecoil(float fDeltaTime);
};