#pragma once

#include "Object.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
protected:
	XMFLOAT3					m_d3dxvPosition;
	XMFLOAT3					m_d3dxvRight;
	XMFLOAT3					m_d3dxvUp;
	XMFLOAT3					m_d3dxvLook;

	float           			m_fPitch;
	float           			m_fYaw;
	float           			m_fRoll;

	XMFLOAT3					m_d3dxvVelocity;
	XMFLOAT3     				m_d3dxvGravity;
	float           			m_fMaxVelocityXZ;
	float           			m_fMaxVelocityY;
	float           			m_fFriction;

	LPVOID						m_pPlayerUpdatedContext;
	LPVOID						m_pCameraUpdatedContext;

	CCamera						*m_pCamera;

public:
	CPlayer(int nMeshes = 1);
	virtual ~CPlayer();

	XMVECTOR GetvPosition() const { return(XMLoadFloat3(&m_d3dxvPosition)); }
	XMFLOAT3 GetPosition() const {	return m_d3dxvPosition; }
	XMVECTOR GetLookVector() { return(XMLoadFloat3(&m_d3dxvLook)); }
	XMVECTOR GetUpVector() { return(XMLoadFloat3(&m_d3dxvUp)); }
	XMVECTOR GetRightVector() { return(XMLoadFloat3(&m_d3dxvRight)); }

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

	const XMVECTOR GetVelocity() const { return(XMLoadFloat3(&m_d3dxvVelocity)); }
	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }

	CCamera *GetCamera() { return(m_pCamera); }
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }

	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMVECTOR d3dxvShift, bool bVelocity = false);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);

	void Update(float fTimeElapsed);

	virtual void OnPlayerUpdated(float fTimeElapsed);
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	virtual void OnCameraUpdated(float fTimeElapsed);
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);

	CCamera *OnChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode);

	virtual void ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPrepareRender();
	virtual void Animate(float fTimeElapsed, XMMATRIX *pd3dxmtxParent);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
};
