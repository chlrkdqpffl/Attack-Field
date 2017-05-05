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
	CStateMachine<CCharacterObject>*	m_pStateMachine = nullptr;
	CPlayer*				m_pPlayer = nullptr;
	CWeapon*				m_pWeapon = nullptr;
	XMFLOAT3				m_f3FiringDirection = XMFLOAT3(0, 0, 1);
	float					m_fPitch = 0.0f;
	float					m_fYaw = 0.0f;
	XMFLOAT3				m_f3Velocity = XMFLOAT3(0,0,0);
	XMFLOAT3				m_f3RelativeVelocity = XMFLOAT3(0, 0, 0);

	// ----- State Variable ----- //
	bool					m_bIsFire = false;
	bool					m_bIsJump = false;
	bool					m_bIsReload = false;
	bool					m_bIsRun = false;

	// ----- Parts Collision Variable ----- // 
	enum BoundingBoxParts { eBody, eHead, eLeftUpArm, eLeftDownArm, eRightUpArm, eRightDownArm, 
		eLeftUpLeg, eLeftDownLeg, eRightUpLeg, eRightDownLeg, ePartsCount };
	BoundingOrientedBox		m_bcPartsBoundingOBox[ePartsCount];
	XMMATRIX				m_mtxPartsBoundingWorld[ePartsCount];
	CBoundingBoxMesh		*m_pPartsBoundingBoxMesh[ePartsCount];

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
	void Firing();
	void Running();

	// ----- Get, Setter ----- // 
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	BoundingOrientedBox GetPartsBoundingOBox(UINT index) const;
	CStateMachine<CCharacterObject>* GetFSM() const { return m_pStateMachine; }
	void SetVelocity(XMFLOAT3 velocity) { m_f3Velocity = velocity; }
	XMFLOAT3 GetVelocity()const { return m_f3Velocity; }
	void SetRelativeVelocity(XMVECTOR velocity) { XMStoreFloat3(&m_f3RelativeVelocity, velocity); }
	XMFLOAT3 GetRelativeVelocity()const { return m_f3RelativeVelocity; }


	void SetYaw(float yaw) { m_fYaw = yaw; }
	float GetYaw() const { return m_fYaw; }
	void SetPitch(float pitch) { m_fPitch = pitch; }
	float GetPitch() const { return m_fPitch; }
	
	bool IsMoving() const {	return (m_f3Velocity.x != 0 || m_f3Velocity.y != 0 || m_f3Velocity.z != 0); }

};