#pragma once
//#include "SkinnedObject.h"
#include "Object.h"
#include "Camera.h"
#include "ServerFuntion.h"

class CCharacterObject;
class CPlayer : public ServerFuntion
{
protected:
	XMFLOAT3					m_vPrevPosition = XMFLOAT3(0,0,0);
	XMFLOAT3					m_d3dxvPosition;
	XMFLOAT3					m_d3dxvRight;
	XMFLOAT3					m_d3dxvUp;
	XMFLOAT3					m_d3dxvLook;

	float						m_fSpeed = 0;
	XMFLOAT3					m_d3dxvVelocity;
	XMFLOAT3     				m_d3dxvGravity;
	float           			m_fMaxVelocityXZ;
	float           			m_fMaxVelocityY;
	float           			m_fFriction;

	LPVOID						m_pPlayerUpdatedContext = nullptr;

	CCamera						*m_pCamera	= nullptr;
	WORD						m_wKeyState = 0;
	CCharacterObject			*m_pCharacter = nullptr;
	bool						m_bIsFloorCollision = false;
	float						m_fGravityTimeElapsed  = 0.0f;
	float						m_fGravityAcceleration = 0.0f;

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

	// ----- Get, Setter ----- //
	XMFLOAT3 GetPrevPosition() const { return m_vPrevPosition; }
	XMVECTOR GetvPrevPosition() const { return XMLoadFloat3(&m_vPrevPosition); }
	XMVECTOR GetvPosition() const { return(XMLoadFloat3(&m_d3dxvPosition)); }
	XMFLOAT3 GetPosition() const {	return m_d3dxvPosition; }
	XMFLOAT3 GetLook() const { return m_d3dxvLook; }
	XMFLOAT3 GetUp() const { return m_d3dxvUp; }
	XMFLOAT3 GetRight() const { return m_d3dxvRight; }
	XMVECTOR GetvLook() const { return(XMLoadFloat3(&m_d3dxvLook)); }
	XMVECTOR GetvUp() const { return(XMLoadFloat3(&m_d3dxvUp)); }
	XMVECTOR GetvRight() const { return(XMLoadFloat3(&m_d3dxvRight)); }

	XMVECTOR GetvVelocity() const { return(XMLoadFloat3(&m_d3dxvVelocity)); }
	XMFLOAT3 GetVelocity() const { return m_d3dxvVelocity; }
	CCamera *GetCamera() { return(m_pCamera); }
	float GetSpeed() const {return m_fSpeed; }
	bool GetFloorCollision() const { return m_bIsFloorCollision; }

//	void setradian(float x, float y) { m_fPitch = x;  m_fYaw = y; }

	void SetFloorCollision(bool isCollision) { m_bIsFloorCollision = isCollision; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetvVelocity(XMVECTOR d3dxvVelocity) { XMStoreFloat3(&m_d3dxvVelocity, d3dxvVelocity); }
	void SetVelocity(XMFLOAT3 d3dxvVelocity) { m_d3dxvVelocity = d3dxvVelocity; }
	void SetPosition(XMVECTOR d3dxvPosition)
	{ 
		XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
		Move((d3dxvPosition - v));
	}
	void SetGravityTimeElpased(float time) { m_fGravityTimeElapsed = time; }
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }
	void SetKeyDown(KeyInput key);
	void SetKeyUp(KeyInput key);
	WORD GetKeyState() const { return m_wKeyState; }
	void SetLook(float x, float y, float z);
};