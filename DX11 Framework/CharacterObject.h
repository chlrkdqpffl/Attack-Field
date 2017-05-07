#pragma once
#include "SkinnedObject.h"
#include "Weapon.h"
#include "StateMachine.h"

class CCharacterObject : public CSkinnedObject
{
public:
	CCharacterObject();
	virtual ~CCharacterObject();

protected:
	CStateMachine<CCharacterObject>*	m_pStateUpper = nullptr;
	CStateMachine<CCharacterObject>*	m_pStateLower = nullptr;

	CPlayer*				m_pPlayer = nullptr;
	CWeapon*				m_pWeapon = nullptr;
	XMFLOAT3				m_f3FiringDirection = XMFLOAT3(0, 0, 1);
	float					m_fPitch = 0.0f;
	float					m_fYaw = 0.0f;
	XMFLOAT3				m_f3Velocity = XMFLOAT3(0, 0, 0);
	XMFLOAT3				m_f3RelativeVelocity = XMFLOAT3(0, 0, 0);

	// ----- State Variable ----- //
	bool					m_bIsFire = false;
	bool					m_bIsJump = false;
	bool					m_bIsReload = false;
	bool					m_bIsRun = false;
	bool					m_bIsDeath = false;

	// ----- Game System Variable ----- //
	UINT					m_nLife = 0;

	// ----- Parts Collision Variable ----- // 

	BoundingOrientedBox		m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::ePartsCount)];
	XMMATRIX				m_mtxPartsBoundingWorld[static_cast<int>(ChracterBoundingBoxParts::ePartsCount)];
	CBoundingBoxMesh		*m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::ePartsCount)];

protected:
	virtual void CreateWeapon(ID3D11Device *pd3dDevice) = 0;
	virtual void CreateStateMachine();

public:
	virtual void CreateObjectData(ID3D11Device *pd3dDevice) override;
	virtual void OnCollisionCheck();

	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;
	virtual void BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext) override;

	virtual void SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal = false) override;
	virtual void SetRotate(XMFLOAT3 fAngle, bool isLocal = false) override;
	virtual void SetRotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal = false) override;

	void RotateFiringPos();

	// ----- Game System Function ----- //
	void Firing();
	void Running();
	void Reloading();
	void Revival();

	void DamagedCharacter(UINT damage);


	// ----- Get, Setter ----- // 
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	BoundingOrientedBox GetPartsBoundingOBox(UINT index) const;
	CStateMachine<CCharacterObject>* GetFSM(AnimationData::Parts parts) const {
		if (parts == AnimationData::Parts::UpperBody)
			return m_pStateUpper;
		else
			return m_pStateLower;
	}
	void SetVelocity(XMFLOAT3 velocity) { m_f3Velocity = velocity; }
	XMFLOAT3 GetVelocity()const { return m_f3Velocity; }
	void SetRelativeVelocity(XMVECTOR velocity) { XMStoreFloat3(&m_f3RelativeVelocity, velocity); }
	XMFLOAT3 GetRelativeVelocity()const { return m_f3RelativeVelocity; }

	void SetYaw(float yaw) { m_fYaw = yaw; }
	float GetYaw() const { return m_fYaw; }
	void SetPitch(float pitch) { m_fPitch = pitch; }
	float GetPitch() const { return m_fPitch; }

	// ----- State Function ----- //
	bool IsMoving() const { return (m_f3Velocity.x != 0 || m_f3Velocity.y != 0 || m_f3Velocity.z != 0); }
	void SetIsRun(bool set) { m_bIsRun = set; }
	bool GetIsRun() const { return  m_bIsRun; }
	void SetIsFire(bool set) { m_bIsFire = set; }
	bool GetIsFire() const { return  m_bIsFire; }
	void SetIsReload(bool set) { m_bIsReload = set; }
	bool GetIsReload() const { return m_bIsReload; }
	void SetIsDeath(bool set) { m_bIsDeath = set; }
	bool GetIsDeath() const { return m_bIsDeath; }

	// ----- Game System Function ----- //
	void SetLife(UINT life) { m_nLife = life; }
	UINT GetLife() const { return m_nLife; }
};