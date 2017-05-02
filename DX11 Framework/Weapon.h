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

	// ���߷�, �ѱ� �ݵ��� ���� ����
	WeaponData::Type		m_Type = WeaponData::Type::eNone;	// �ѱ� Ÿ��
	float					m_fDamage = 0.f;					// ������
	float					m_fBulletMovingSpeed = 0.f;			// �ѱ���
	float					m_fRange = 0.f;						// ��ȿ ��Ÿ�
	UINT					m_uiFireSpeed = 0;					// ���� �ӵ� (1000 - 1��)
	DWORD					m_dwLastAttackTime = 0;				// ������ �߻� �ð�

	UINT					m_nMaxhasBulletCount = 0;			// �� �� ������ ��ź ��
	UINT					m_nhasBulletCount = 0;				// ���� ��ź ��

	vector<CBulletObject*>	m_vecBulletContainer;

public:
	virtual void CreateObjectData(ID3D11Device *pd3dDevice) override;
	virtual void Firing(XMVECTOR direction);
	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	virtual void CreateBulletPool(ID3D11Device* pd3dDevice) = 0;
	
	void CreateFireDirectionLine(XMVECTOR direction);
};