#pragma once
//#include "SkinnedObject.h"
#include "Object.h"
#include "Camera.h"

class CCharacterObject;
class CPlayer 
	//: public CGameObject
//class CPlayer : public CSkinnedObject
{
protected:
	XMFLOAT3					m_d3dxvPosition;
	XMFLOAT3					m_d3dxvRight;
	XMFLOAT3					m_d3dxvUp;
	XMFLOAT3					m_d3dxvLook;

	float           			m_fPitch;
	float           			m_fYaw;
	float           			m_fRoll;

	float						m_fSpeed = 0;
	XMFLOAT3					m_d3dxvVelocity;
	XMFLOAT3     				m_d3dxvGravity;
	float           			m_fMaxVelocityXZ;
	float           			m_fMaxVelocityY;
	float           			m_fFriction;

	LPVOID						m_pPlayerUpdatedContext = nullptr;
	LPVOID						m_pCameraUpdatedContext = nullptr;

	CCamera						*m_pCamera	= nullptr;
	WORD						m_wKeyState = 0;
	CameraTag					m_tagCamera = CameraTag::eNone;
	CCharacterObject			*m_pCharacter = nullptr;

public:
	CPlayer(CCharacterObject* pCharacter);
	virtual ~CPlayer();

	virtual void OnPlayerUpdated(float fTimeElapsed);
	virtual void OnCameraUpdated(float fTimeElapsed);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void ChangeCamera(ID3D11Device *pd3dDevice, CameraTag cameraTag, float fTimeElapsed);
	virtual void OnPrepareRender();
	virtual void Animate(float fTimeElapsed, XMMATRIX *pd3dxmtxParent);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	CCamera *OnChangeCamera(ID3D11Device *pd3dDevice, CameraTag nNewCameraTag, CameraTag nCurrentCameraTag);
	void Move(const XMVECTOR d3dxvShift, bool bVelocity = false);
	void Rotate(float x, float y, float z);
	void Update(float fTimeElapsed);
	void UpdateKeyInput(float fTimeElapsed);

	// ----- Get, Setter ----- //
	XMVECTOR GetvPosition() const { return(XMLoadFloat3(&m_d3dxvPosition)); }
	XMFLOAT3 GetPosition() const {	return m_d3dxvPosition; }
	XMVECTOR GetLookVector() { return(XMLoadFloat3(&m_d3dxvLook)); }
	XMVECTOR GetUpVector() { return(XMLoadFloat3(&m_d3dxvUp)); }
	XMVECTOR GetRightVector() { return(XMLoadFloat3(&m_d3dxvRight)); }
	const XMVECTOR GetVelocity() const { return(XMLoadFloat3(&m_d3dxvVelocity)); }
	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }
	CCamera *GetCamera() { return(m_pCamera); }
	float GetSpeed() const {return m_fSpeed; }

	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(XMVECTOR d3dxvGravity) { XMStoreFloat3(&m_d3dxvGravity, d3dxvGravity); }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(XMVECTOR d3dxvVelocity) { XMStoreFloat3(&m_d3dxvVelocity, d3dxvVelocity); }
	void SetPosition(XMVECTOR d3dxvPosition)
	{ 
		XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
		Move((d3dxvPosition - v), false);
	}
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }
	void SetKeyDown(KeyInput key) { m_wKeyState |= static_cast<int>(key); }
	void SetKeyUp(KeyInput key) { m_wKeyState ^= static_cast<int>(key); }
};