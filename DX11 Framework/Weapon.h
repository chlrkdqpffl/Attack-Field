#pragma once
#include "Object.h"
#include "BulletObject.h"
#include "LineObject.h"

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

	// 명중률, 총기 반동은 추후 구현
	WeaponData::Type		m_Type = WeaponData::Type::eNone;	// 총기 타입
	float					m_fDamage = 0.f;					// 데미지
	float					m_fBulletMovingSpeed = 0.f;			// 총구속
	float					m_fRange = 0.f;						// 유효 사거리
	UINT					m_uiFireSpeed = 0;					// 연사 속도 (1000 - 1초)
	DWORD					m_dwLastAttackTime = 0;				// 마지막 발사 시간

	UINT					m_nMaxhasBulletCount = 0;			// 한 번 장전시 장탄 수
	UINT					m_nhasBulletCount = 0;				// 현재 장탄 수

	vector<CBulletObject*>	m_vecBulletContainer;

public:
	virtual void CreateObjectData(ID3D11Device *pd3dDevice) override;
	virtual void Firing(XMVECTOR direction);
	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	virtual void CreateBulletPool(ID3D11Device* pd3dDevice) = 0;
	
	void CreateFireDirectionLine(XMVECTOR direction);
};