#pragma once
#include "Object.h"
#include "Camera.h"
#include "CharacterObject.h"

class CPlayer
{
public:
	XMFLOAT4X4					m_mtxWorld;

protected:
	/*
	XMFLOAT3					m_d3dxvPosition;
	XMFLOAT3					m_d3dxvRight;
	XMFLOAT3					m_d3dxvUp;
	XMFLOAT3					m_d3dxvLook;
	*/
	float						m_fPitch = 0.0f;
	float						m_fSpeed = 0;
	XMFLOAT3					m_d3dxvVelocity;
	XMFLOAT3					m_f3MoveDirection = XMFLOAT3(0, 0, 0);
	XMFLOAT3     				m_f3Gravity = XMFLOAT3(0, 0, 0);
	float           			m_fFriction;

	LPVOID						m_pPlayerUpdatedContext = nullptr;

	CCamera						*m_pCamera	= nullptr;
	WORD						m_wKeyState = 0;
	WORD						m_Befroe_KeyState = 0;
	CCharacterObject			*m_pCharacter = nullptr;
	bool						m_bIsFloorCollision = false;
	float						m_fGravityTimeElapsed  = 0.0f;
	float						m_fGravityAcceleration = 0.0f;

	int							count;

	// Damage Direction
	DamagedInfo					m_infoDamage;

	// DOF Variable
	int							m_nPrevDOFStart = 0;
	DWORD						m_dwDOFPreiodTime = 0;


	//PhysX
	PxController*				m_pPxCharacterController = nullptr;

public:
	CPlayer(CCharacterObject* pCharacter = nullptr);
	virtual ~CPlayer();

	virtual void OnApplyGravity(float fDeltaTime) {};
	virtual void OnCameraUpdated(float fDeltaTime) {};
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void ChangeCamera(ID3D11Device *pd3dDevice, CameraTag cameraTag) {};
	
	CCamera *OnChangeCamera(ID3D11Device *pd3dDevice, CameraTag nNewCameraTag, CameraTag nCurrentCameraTag); 
	void Move(const XMVECTOR d3dxvShift);
	void Rotate(float x, float y);
	void Update(float fDeltaTime);
	void UpdateKeyInput(float fDeltaTime);
	void UpdateDOF(float fDeltaTime);
	void InitializePhysXData(PxPhysics* pPxPhysics, PxMaterial *pPxMaterial, PxControllerManager *pPxControllerManager);


	// ----- PhysX Funtion ----- //
	void PhysXMove(float fDeltaTime);
	void PhysXUpdateKeyInput(float fDeltaTime);
	void PhysXUpdate(float fDeltaTime);


	// ----- Get, Setter ----- //
	XMFLOAT3 GetRight() const { return XMFLOAT3(m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13); }
	XMFLOAT3 GetUp() const { return XMFLOAT3(m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23); }
	XMFLOAT3 GetLook() const { return XMFLOAT3(m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33); }
	XMFLOAT3 GetPosition() const {	return XMFLOAT3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43); }

	XMVECTOR GetvRight() const { return(XMLoadFloat3(&XMFLOAT3(m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13))); }
	XMVECTOR GetvUp() const { return(XMLoadFloat3(&XMFLOAT3(m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23))); }
	XMVECTOR GetvLook() const { return(XMLoadFloat3(&XMFLOAT3(m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33))); }
	XMVECTOR GetvPosition() const { return(XMLoadFloat3(&XMFLOAT3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43))); }

	void SetRight(XMFLOAT3 right) { m_mtxWorld._11 = right.x, m_mtxWorld._12 = right.y, m_mtxWorld._13 = right.z; }
	void SetUp(XMFLOAT3 up) { m_mtxWorld._21 = up.x, m_mtxWorld._22 = up.y, m_mtxWorld._23 = up.z; }
	void SetLook(XMFLOAT3 look) { m_mtxWorld._31 = look.x, m_mtxWorld._32 = look.y, m_mtxWorld._33 = look.z; }
	void SetPosition(XMFLOAT3 pos) { m_mtxWorld._41 = pos.x, m_mtxWorld._42 = pos.y, m_mtxWorld._43 = pos.z; }

	void SetvRight(XMVECTOR vRight) { 
		XMFLOAT3 right; XMStoreFloat3(&right, vRight);
		SetRight(right);
	}
	void SetvUp(XMVECTOR vUp) {
		XMFLOAT3 up; XMStoreFloat3(&up, vUp);
		SetUp(up);
	}
	void SetvLook(XMVECTOR vLook) {
		XMFLOAT3 look; XMStoreFloat3(&look, vLook);
		SetLook(look);
	}


	void SetvPosition(XMVECTOR vPosition)
	{
		XMVECTOR v = GetvPosition();
		Move((vPosition - v));

		XMFLOAT3 pos; XMStoreFloat3(&pos, vPosition);
		m_pPxCharacterController->setFootPosition(PxExtendedVec3(pos.x, pos.y, pos.z));
	}


	float GetPitch() const { return m_fPitch; }
	XMVECTOR GetvVelocity() const { return(XMLoadFloat3(&m_d3dxvVelocity)); }
	XMFLOAT3 GetVelocity() const { return m_d3dxvVelocity; }
	CCamera *GetCamera() { return(m_pCamera); }
	float GetSpeed() const {return m_fSpeed; }
	bool GetFloorCollision() const { return m_bIsFloorCollision; }
	void SetFloorCollision(bool isCollision) { m_bIsFloorCollision = isCollision; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetFriction(float fFriction) { m_fFriction = fFriction; }

	void SetvVelocity(XMVECTOR d3dxvVelocity) { XMStoreFloat3(&m_d3dxvVelocity, d3dxvVelocity); }
	void SetVelocity(XMFLOAT3 d3dxvVelocity) { m_d3dxvVelocity = d3dxvVelocity; }
	
	void SetGravityTimeElpased(float time) { m_fGravityTimeElapsed = time; }
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }
	void SetKeyDown(KeyInput key);
	void SetKeyUp(KeyInput key);
	WORD GetKeyState() const { return m_wKeyState; }
	
	void SetDamagedInfo(DamagedInfo info) { m_infoDamage = info; }
	UINT GetServerID() const { return m_pCharacter->GetServerID(); }

	// --- Draw UI Text --- //
	UINT GetWeaponBulletCount() const { return m_pCharacter->GetWeaponBulletCount(); }
	void SetWeaponBulletMax() { m_pCharacter->SetWeaponBulletMax(); }
	UINT GetWeaponMaxBulletCount() const { return m_pCharacter->GetWeaponMaxBulletCount(); }
	UINT GetPlayerLife() const { return m_pCharacter->GetLife(); }
	UINT GetPlayerArmorPoint() const { return m_pCharacter->GetArmorPoint(); }
	void SetPlayerlife(UINT hp) { m_pCharacter->SetLife(hp); }

	bool GetIsDamage() const { return m_infoDamage.m_bIsDamage; }
	void SetIsDamage(bool set) { m_infoDamage.m_bIsDamage = set; }
	DamagedInfo GetDamageInfo() const { return m_infoDamage; }
	bool GetIsDeadly() const { return m_pCharacter->GetIsDeadly(); }
	void SetIsDeadly(bool set) { m_pCharacter->SetIsDeadly(set); }
	bool GetIsDeadlyAttack() const { return m_pCharacter->GetIsDeadlyAttack(); }
	void SetIsDeadlyAttack(bool set) const { m_pCharacter->SetIsDeadlyAttack(set); }
};