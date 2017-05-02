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
	XMFLOAT3				m_f3FiringDirection = XMFLOAT3(0, 0, 1);
	CGameObject				Object;
	float					m_fPitch = 0.0f;


	// ----- Parts Collision Variable ----- // 
	enum BoundingBoxParts { eBody, eHead, ePartsCount };
	BoundingOrientedBox		m_bcPartsBoundingOBox[ePartsCount];
	XMMATRIX				m_mtxPartsBoundingWorld[ePartsCount];
	CBoundingBoxMesh		*m_pPartsBoundingBoxMesh[ePartsCount];

protected:
	virtual void CreateWeapon(ID3D11Device *pd3dDevice) = 0;

public:
	virtual void CreateObjectData(ID3D11Device *pd3dDevice) override;
	virtual void OnCollisionCheck();

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;
	virtual void BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext) override;

	virtual void SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal = false) override;
	virtual void SetRotate(XMFLOAT3 fAngle, bool isLocal = false) override;
	virtual void SetRotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal = false) override;



	void RotateFiringPos();
	void Firing();
	void Running();

	// ----- Get, Setter ----- // 
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	BoundingOrientedBox GetPartsBoundingOBox(UINT index) const;
};