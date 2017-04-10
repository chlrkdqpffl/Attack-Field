//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Player.h"
#include "FirstPersonCamera.h"
#include "SpaceShipCamera.h"
#include "ThirdPersonCamera.h"


CPlayer::CPlayer(CCharacterObject* pCharacter) 
	: m_pCharacter(pCharacter)
{
	m_d3dxvPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_d3dxvVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_d3dxvGravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;


	// 임시
	m_fSpeed = 50;
//	m_fSpeed = 10;	// 자연스러운 속도
}

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

void CPlayer::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
}

void CPlayer::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
//	GetMaterial()->UpdateShaderVariable(pd3dDeviceContext);
	 //UpdateShaderVariable
	if (m_pCamera) m_pCamera->UpdateShaderVariables(pd3dDeviceContext);
//	m_pCharacter->
}

void CPlayer::UpdateKeyInput(float fTimeElapsed)			// FSM으로 제작하여 상호 관계를 확실히 해야함. 일단 임시로 제작
{
	// Keyboard
	XMVECTOR d3dxvShift = XMVectorZero();

	if (m_wKeyState & static_cast<int>(KeyInput::eForward)) {
		d3dxvShift += XMLoadFloat3(&m_d3dxvLook);

		if(m_pCharacter->GetAnimation() != AnimationData::CharacterAnim::eRun)
			m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eBackward)) {
		d3dxvShift -= XMLoadFloat3(&m_d3dxvLook);
		m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eRight)) {
		d3dxvShift += XMLoadFloat3(&m_d3dxvRight);
		m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eLeft)) {
		d3dxvShift -= XMLoadFloat3(&m_d3dxvRight);
		m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eRun)) {
		//d3dxvShift *= 3;		// m_fSpeed 로 변경해야함
		d3dxvShift *= 10;		// m_fSpeed 로 변경해야함
		m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eRun);
	}


	// Mouse
	if (m_wKeyState & static_cast<int>(KeyInput::eLeftMouse)) {
		m_pCharacter->Fire();
	}
	if (m_wKeyState & static_cast<int>(KeyInput::eRightMouse)) {
	
	}
	/*	- FSM 만들고 사용하기
	if ((m_wKeyState & static_cast<int>(KeyInput::eForward))
		&& (m_wKeyState & static_cast<int>(KeyInput::eLeftMouse)))
		m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalkingFire);
		*/
	if (m_wKeyState == 0) {
		m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eIdle);
	}

	d3dxvShift *= m_fSpeed * fTimeElapsed;
	XMStoreFloat3(&m_d3dxvVelocity, XMLoadFloat3(&m_d3dxvVelocity) + d3dxvShift);
}

void CPlayer::Move(XMVECTOR d3dxvShift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		cout << "여기를 안들어오는 것 같다. 추후 지우기";
		cout << "무브";
		ShowXMFloat3(m_d3dxvVelocity);
		XMStoreFloat3(&m_d3dxvVelocity, XMLoadFloat3(&m_d3dxvVelocity) + d3dxvShift);
	}
	else
	{
		XMVECTOR d3dxvPosition = XMLoadFloat3(&m_d3dxvPosition) + d3dxvShift;
		XMStoreFloat3(&m_d3dxvPosition, d3dxvPosition);
		m_pCamera->Move(d3dxvShift);
	}
}

void CPlayer::Rotate(float x, float y, float z)
{
	XMMATRIX mtxRotate;
	CameraTag nCurrentCameraTag = m_pCamera->GetCameraTag();
	if ((nCurrentCameraTag == CameraTag::eFirstPerson) || (nCurrentCameraTag == CameraTag::eThirdPerson))
	{
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
			if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		m_pCamera->Rotate(x, y, z);

		if (y != 0.0f)
		{
			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvUp), XMConvertToRadians(y));
			XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
			XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		}
	}
	else if (nCurrentCameraTag == CameraTag::eSpaceShip)
	{
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvRight), XMConvertToRadians(x));
			XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
			XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		}
		if (y != 0.0f)
		{
			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvUp), XMConvertToRadians(y));
			XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
			XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		}
		if (z != 0.0f)
		{
			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvLook), XMConvertToRadians(z));
			XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
			XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		}
	}

	XMStoreFloat3(&m_d3dxvLook, XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook)));
	XMStoreFloat3(&m_d3dxvRight, XMVector3Cross(XMLoadFloat3(&m_d3dxvUp), XMLoadFloat3(&m_d3dxvLook)));
	XMStoreFloat3(&m_d3dxvRight, XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight)));
	XMStoreFloat3(&m_d3dxvUp, XMVector3Cross(XMLoadFloat3(&m_d3dxvLook), XMLoadFloat3(&m_d3dxvRight)));
	XMStoreFloat3(&m_d3dxvUp, XMVector3Normalize(XMLoadFloat3(&m_d3dxvUp)));
}

void CPlayer::Update(float fTimeElapsed)
{
	XMStoreFloat3(&m_d3dxvVelocity, XMLoadFloat3(&m_d3dxvVelocity) + XMLoadFloat3(&m_d3dxvGravity) * fTimeElapsed);
	float fLength = sqrtf(m_d3dxvVelocity.x * m_d3dxvVelocity.x + m_d3dxvVelocity.z * m_d3dxvVelocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ * fTimeElapsed;

	if (fLength > fMaxVelocityXZ)
	{
		m_d3dxvVelocity.x *= (fMaxVelocityXZ / fLength);
		m_d3dxvVelocity.z *= (fMaxVelocityXZ / fLength);
	}
	
	// Apply Gravity
	float fMaxVelocityY = m_fMaxVelocityY * fTimeElapsed;
	fLength = sqrtf(m_d3dxvVelocity.y * m_d3dxvVelocity.y);
	if (fLength > fMaxVelocityY) m_d3dxvVelocity.y *= (fMaxVelocityY / fLength);

	// Character Update
	m_pCharacter->Update(fTimeElapsed);


	Move(XMLoadFloat3(&m_d3dxvVelocity), false);
	if (m_pPlayerUpdatedContext) OnPlayerUpdated(fTimeElapsed);

	

	CameraTag nCurrentCameraTag = m_pCamera->GetCameraTag();
	if (nCurrentCameraTag == CameraTag::eThirdPerson) m_pCamera->Update(XMLoadFloat3(&m_d3dxvPosition), fTimeElapsed);
	if (m_pCameraUpdatedContext) OnCameraUpdated(fTimeElapsed);
	if (nCurrentCameraTag == CameraTag::eThirdPerson) m_pCamera->SetLookAt(XMLoadFloat3(&m_d3dxvPosition));
	m_pCamera->RegenerateViewMatrix();

	// Apply Deceleration 
	XMVECTOR d3dxvDeceleration = -XMLoadFloat3(&m_d3dxvVelocity);
	d3dxvDeceleration = XMVector3Normalize(d3dxvDeceleration);
	fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_d3dxvVelocity)));
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) fDeceleration = fLength;
	XMStoreFloat3(&m_d3dxvVelocity, XMLoadFloat3(&m_d3dxvVelocity) + d3dxvDeceleration * fDeceleration);
}

CCamera *CPlayer::OnChangeCamera(ID3D11Device *pd3dDevice, CameraTag nNewCameraTag, CameraTag nCurrentCameraTag)
{
	CCamera *pNewCamera = NULL;
	switch (nNewCameraTag) {
	case CameraTag::eFirstPerson:
		pNewCamera = new CFirstPersonCamera(m_pCamera);
		break;
	case CameraTag::eSpaceShip:
		pNewCamera = new CSpaceShipCamera(m_pCamera);
		break;
	case CameraTag::eThirdPerson:
		pNewCamera = new CThirdPersonCamera(m_pCamera);
		break;
	}
	if (nCurrentCameraTag == CameraTag::eSpaceShip)
	{
		m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_d3dxvRight.y = 0.0f;
		m_d3dxvLook.y = 0.0f;
		XMStoreFloat3(&m_d3dxvRight, XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight)));
		XMStoreFloat3(&m_d3dxvLook, XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook)));
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat3(&m_d3dxvLook)))));
		if (m_d3dxvLook.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraTag == CameraTag::eSpaceShip) && m_pCamera)
	{
		XMStoreFloat3(&m_d3dxvRight, m_pCamera->GetRightVector());
		XMStoreFloat3(&m_d3dxvUp, m_pCamera->GetUpVector());
		XMStoreFloat3(&m_d3dxvLook, m_pCamera->GetLookVector());
	}

	if (pNewCamera)
	{
		pNewCamera->SetCameraTag(nNewCameraTag);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}

void CPlayer::ChangeCamera(ID3D11Device *pd3dDevice, CameraTag cameraTag, float fTimeElapsed)
{
}

void CPlayer::OnPlayerUpdated(float fTimeElapsed)
{
}

void CPlayer::OnCameraUpdated(float fTimeElapsed)
{
}

void CPlayer::OnPrepareRender()
{
	XMFLOAT4X4 mtx;

	XMStoreFloat4x4(&mtx, m_pCharacter->m_mtxWorld);

	mtx._11 = m_d3dxvRight.x;		mtx._12 = m_d3dxvRight.y;		mtx._13 = m_d3dxvRight.z;
	mtx._21 = m_d3dxvUp.x;			mtx._22 = m_d3dxvUp.y;			mtx._23 = m_d3dxvUp.z;
	mtx._31 = m_d3dxvLook.x;		mtx._32 = m_d3dxvLook.y;		mtx._33 = m_d3dxvLook.z;
	mtx._41 = m_d3dxvPosition.x;	mtx._42 = m_d3dxvPosition.y;	mtx._43 = m_d3dxvPosition.z;

	m_pCharacter->m_mtxWorld = XMLoadFloat4x4(&mtx);
}

void CPlayer::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
//	if (m_pCamera->GetCameraTag() == CameraTag::eThirdPerson)
		m_pCharacter->Render(pd3dDeviceContext, pCamera);
}
