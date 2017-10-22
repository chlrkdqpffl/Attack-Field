#pragma once
#include "Object.h"
#include "Camera.h"
#include "CharacterPlayer.h"

class CPlayer
{
public:
	XMFLOAT4X4					m_mtxWorld;

protected:
	CCharacterPlayer			*m_pCharacter = nullptr;
	CCamera						*m_pCamera	= nullptr;

	// PhysX
	PxController*				m_pPxCharacterController = nullptr;

	float						m_fSpeedFactor = 1.0f;
	float						m_fMoveSpeed = 0.0f;
	XMFLOAT3					m_f3Accelerate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_f3MoveDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3     				m_f3GravityVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3     				m_f3GravityAccel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	

	float						m_fUserMovePitch = 0.0f;			// 총 발사 시점부터 사용자가 위로 올린 각도
	WORD						m_wKeyState = 0;
	bool						m_bIsFire = false;
	bool						m_bIsJumping = false;
	bool						m_bIsZoom = false;
	int							m_nFovAngle = 0;
	int							count;

	// Damage Direction
	DamagedInfo					m_infoDamage;

	// DOF Variable
	int							m_nPrevDOFStart = 0;
	DWORD						m_dwDOFPreiodTime = 0;

public:
	CPlayer(CCharacterPlayer* pCharacter = nullptr);
	virtual ~CPlayer();

	virtual void OnApplyGravity(float fDeltaTime) {};
	virtual void OnCameraUpdated(float fDeltaTime) {};
	virtual void ChangeCamera(ID3D11Device *pd3dDevice, CameraTag cameraTag) {};
	
	CCamera *OnChangeCamera(ID3D11Device *pd3dDevice, CameraTag nNewCameraTag, CameraTag nCurrentCameraTag); 
	

	void Rotate(float x, float y);
	void Update(float fDeltaTime);
	void OnKeyInputUpdate(UINT nMessageID, WPARAM wParam);
	void UpdateKeyState(float fDeltaTime);
	void UpdateDOF(float fDeltaTime);

	// ----- PhysX Funtion ----- //
	void InitializePhysXData(PxPhysics* pPxPhysics, PxMaterial *pPxMaterial, PxControllerManager *pPxControllerManager);
	void PhysXUpdate(float fDeltaTime);
	void AddAccel(XMFLOAT3 force);

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

	void SetPosition(XMFLOAT3 pos) 
	{ 
		m_mtxWorld._41 = pos.x, m_mtxWorld._42 = pos.y, m_mtxWorld._43 = pos.z;
		m_pPxCharacterController->setPosition(PxExtendedVec3(pos.x, pos.y, pos.z));
	}
	void SetvPosition(XMVECTOR vPosition)
	{
		XMFLOAT3 pos; XMStoreFloat3(&pos, vPosition);
		SetPosition(pos);
	}

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

	float GetPitch() const { return m_pCharacter->GetPitch(); }
	CCamera *GetCamera() { return(m_pCamera); }

	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	void SetKeyDown(KeyInput key);
	void SetKeyUp(KeyInput key);
	WORD GetKeyState() const { return m_wKeyState; }
	
	UINT GetServerID() const { return m_pCharacter->GetServerID(); }

	// --- Draw UI Text --- //
	UINT GetWeaponBulletCount() const { return m_pCharacter->GetWeaponBulletCount(); }
	void SetWeaponBulletMax() { m_pCharacter->SetWeaponBulletMax(); }
	UINT GetWeaponMaxBulletCount() const { return m_pCharacter->GetWeaponMaxBulletCount(); }
	UINT GetPlayerLife() const { return m_pCharacter->GetLife(); }
	UINT GetPlayerArmorPoint() const { return m_pCharacter->GetArmorPoint(); }
	void SetPlayerlife(UINT hp) { m_pCharacter->SetLife(hp); }
	bool GetAlive() const { return m_pCharacter->GetLife(); }

	bool GetIsZoom() const { return m_bIsZoom; }
	void SetIsZoom(bool set) { m_bIsZoom = set; }

	bool GetIsDamage() const { return m_infoDamage.m_bIsDamage; }
	void SetIsDamage(bool set) { m_infoDamage.m_bIsDamage = set; }
	void SetDamagedInfo(DamagedInfo info) { m_infoDamage = info; }
	DamagedInfo GetDamageInfo() const { return m_infoDamage; }
	bool GetIsDeadly() const { return m_pCharacter->GetIsDeadly(); }
	void SetIsDeadly(bool set) { m_pCharacter->SetIsDeadly(set); }
	bool GetIsDeadlyAttack() const { return m_pCharacter->GetIsDeadlyAttack(); }
	void SetIsDeadlyAttack(bool set) const { m_pCharacter->SetIsDeadlyAttack(set); }
};