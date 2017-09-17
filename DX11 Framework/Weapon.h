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

	// ----- 총기 반동 ----- //
	bool					m_bIsFire = false;
	UINT					m_nFireBulletCount = 0;				// 총기 반동 확인
	float					m_fInitPitch = 0.0f;				// 최초 반동 위치
	float					m_fUserMovePitch = 0.0f;			// 총 발사 시점부터 사용자가 위로 올린 각도
	const float				m_fMaxPitchGap = 0.0f;				// 최대 반동 각도
	float					m_fCalcRecoil = 0.0f;				// 계산된 반동력
	float					m_fNowRecoil = 0.0f;				// 현재 반동력	- UI에 사용

	// ------------- 무기 세부 정보 ------------ //
	WeaponTag				m_tagWeapon = WeaponTag::eNone;		// 총기 타입
	float					m_fDamage;							// 데미지
	float					m_fRange = 0.f;						// 유효 사거리
	float					m_fReCoil = 0.2f;					// 총기 반동력
	UINT					m_uiFireSpeed = 0;					// 연사 속도 (1000 - 1초)
	DWORD					m_dwLastAttackTime = 0;				// 마지막 발사 시간

	UINT					m_nMaxhasBulletCount = 0;			// 한 번 장전시 장탄 수
	UINT					m_nhasBulletCount = 0;				// 현재 장탄 수

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