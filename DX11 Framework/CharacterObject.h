#pragma once
#include "SkinnedObject.h"
#include "Weapon.h"

class CCharacterObject : public CSkinnedObject
{
public:
	CCharacterObject();
	virtual ~CCharacterObject();

protected:
	CPlayer*				m_pPlayer = nullptr;
	CWeapon*				m_pWeapon = nullptr;

	enum BoundingBoxParts { eBody, eHead, ePartsCount };
	BoundingOrientedBox		m_bcPartsBoundingOBox[ePartsCount];
	XMMATRIX				m_mtxPartsBoundingWorld[ePartsCount];
	CBoundingBoxMesh		*m_pPartsBoundingBoxMesh[ePartsCount];

protected:
	virtual void CreateWeapon(ID3D11Device *pd3dDevice) = 0;

public:
	virtual void CreateObjectData(ID3D11Device *pd3dDevice) override;
	virtual void OnCollisionCheck();

	virtual void Fire();
	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;
	virtual void BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext) override;

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	BoundingOrientedBox GetPartsBoundingOBox(UINT index) const;
};