#pragma once
#include "CharacterObject.h"

class CCharacterPlayer : public CCharacterObject
{
private:
	CPlayer*	m_pPlayer = nullptr;

public:
	CCharacterPlayer(TeamType team);
	virtual ~CCharacterPlayer();

	virtual void Revival() override;
	virtual void RotateFiringPos() override;

	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	float GetWeaponCalcRecoil() const { return m_pWeapon->GetCalcRecoil(); }
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* GetPlayer() const { return m_pPlayer; }
};

