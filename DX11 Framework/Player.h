#pragma once
//#include "SkinnedObject.h"
#include "Object.h"
#include "Camera.h"
#include "ServerFuntion.h"

class CCharacterObject;
class CPlayer : public ServerFuntion
{
protected:
	XMFLOAT3					m_d3dxvPosition;
	XMFLOAT3					m_d3dxvRight;
	XMFLOAT3					m_d3dxvUp;
	XMFLOAT3					m_d3dxvLook;

	float           			m_fPitch;
	float           			m_fYaw;
	
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


	////////////////이전 로테이션값들/////////////////
	float						m_prev_x;
	float						m_prev_y;
	float						m_prev_z;

public:
	CPlayer(CCharacterObject* pCharacter = nullptr);
	virtual ~CPlayer();

	virtual void OnPlayerUpdated(float fTimeElapsed) {};
	virtual void OnCameraUpdated(float fTimeElapsed) {};
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void ChangeCamera(ID3D11Device *pd3dDevice, CameraTag cameraTag) {};
	
	CCamera *OnChangeCamera(ID3D11Device *pd3dDevice, CameraTag nNewCameraTag, CameraTag nCurrentCameraTag); 
	void Move(const XMVECTOR d3dxvShift);
	void Rotate(float x, float y, float z);
	void Update(float fTimeElapsed);
	void UpdateKeyInput(float fTimeElapsed);
	void RotateToCharacter();

	bool IsMoving() const;
	// ----- Get, Setter ----- //
	XMVECTOR GetvPosition() const { return(XMLoadFloat3(&m_d3dxvPosition)); }
	XMFLOAT3 GetPosition() const {	return m_d3dxvPosition; }
	XMVECTOR GetLookVector() { return(XMLoadFloat3(&m_d3dxvLook)); }
	XMVECTOR GetUpVector() { return(XMLoadFloat3(&m_d3dxvUp)); }
	XMVECTOR GetRightVector() { return(XMLoadFloat3(&m_d3dxvRight)); }
	const XMVECTOR GetVelocity() const { return(XMLoadFloat3(&m_d3dxvVelocity)); }
	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	CCamera *GetCamera() { return(m_pCamera); }
	float GetSpeed() const {return m_fSpeed; }
	bool GetFloorCollision() const { return m_bIsFloorCollision; }

	void SetFloorCollision(bool isCollision) { m_bIsFloorCollision = isCollision; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(XMVECTOR d3dxvGravity) { XMStoreFloat3(&m_d3dxvGravity, d3dxvGravity); }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(XMVECTOR d3dxvVelocity) { XMStoreFloat3(&m_d3dxvVelocity, d3dxvVelocity); }
	void SetPosition(XMVECTOR d3dxvPosition)
	{ 
		XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
		Move((d3dxvPosition - v));
	}
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }
	void SetKeyDown(KeyInput key);
	void SetKeyUp(KeyInput key);
	WORD GetKeyState() const { return m_wKeyState; }
	void SetWorldMatrix(XMMATRIX world);
};