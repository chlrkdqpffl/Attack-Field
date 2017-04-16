#pragma once
#include "SkinnedObject.h"
#include "Weapon.h"

class CCharacterObject : public CSkinnedObject
{
public:
	CCharacterObject();
	virtual ~CCharacterObject();

protected:
	CPlayer*			m_pPlayer = nullptr;
	CWeapon*			m_pWeapon = nullptr;


protected:
	virtual void CreateWeapon(ID3D11Device *pd3dDevice) = 0;

public:
	virtual void CreateObjectData(ID3D11Device *pd3dDevice) override;
	virtual void OnCollisionCheck();

	virtual void Fire();
	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
};

