#include "stdafx.h"
#include "Player.h"
#include "FirstPersonCamera.h"
#include "SpaceShipCamera.h"
#include "ThirdPersonCamera.h"
#include "protocol.h"

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
	m_fYaw = 0.0f;

	// 임시
	m_fSpeed = 50;
//	m_fSpeed = 10;	// 자연스러운 속도
}

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

bool CPlayer::IsMoving() const
{
	return (m_d3dxvVelocity.x != 0 || m_d3dxvVelocity.y != 0 || m_d3dxvVelocity.z != 0);
}

void CPlayer::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
}

void CPlayer::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pCamera) m_pCamera->UpdateShaderVariables(pd3dDeviceContext);
}

void CPlayer::UpdateKeyInput(float fTimeElapsed)			// FSM으로 제작하여 상호 관계를 확실히 해야함. 일단 임시로 제작
{
	// Keyboard
	XMVECTOR d3dxvShift = XMVectorZero();

	if (m_wKeyState & static_cast<int>(KeyInput::eForward)) {
//		d3dxvShift += XMLoadFloat3(&m_d3dxvLook);

		if(m_pCharacter->GetAnimation() != AnimationData::CharacterAnim::eRun)
			m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eBackward)) {
	//	d3dxvShift -= XMLoadFloat3(&m_d3dxvLook);
		m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eLeft)) {
		//d3dxvShift -= XMLoadFloat3(&m_d3dxvRight);
		m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eRight)) {
		//d3dxvShift += XMLoadFloat3(&m_d3dxvRight);
		m_pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eRun)) {
		d3dxvShift *= 10;		// m_fSpeed 로 변경해야함
//		d3dxvShift *= 3;		// m_fSpeed 로 변경해야함
		m_pCharacter->Running();
	}

	// Mouse
	if (m_wKeyState & static_cast<int>(KeyInput::eLeftMouse)) {
		m_pCharacter->Firing();
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

void CPlayer::Move(XMVECTOR d3dxvShift)
{	
	XMVECTOR d3dxvPosition = XMLoadFloat3(&m_d3dxvPosition) + d3dxvShift;
	XMStoreFloat3(&m_d3dxvPosition, d3dxvPosition);
	m_pCamera->Move(d3dxvShift);
}

void CPlayer::Rotate(float x, float y)
{
	XMMATRIX mtxRotate;
	CameraTag nCurrentCameraTag = m_pCamera->GetCameraTag();
	if ((nCurrentCameraTag == CameraTag::eFirstPerson) || (nCurrentCameraTag == CameraTag::eThirdPerson)) {
		if (x != 0.0f) {
			m_fPitch += x;
			if (50.0f < m_fPitch) {
				x -= (m_fPitch - 50);
				m_fPitch = 50;
			}
			if (m_fPitch < -40.0f) {
				x -= (m_fPitch + 40);
				m_fPitch = -40;
			}
		}
		if (y != 0.0f) {
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;

			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvUp), XMConvertToRadians(y));
			XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
			XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		}
		m_pCamera->Rotate(x, y, 0);
	}
	else if (nCurrentCameraTag == CameraTag::eSpaceShip)
	{
		m_pCamera->Rotate(x, y, 0);
		if (x != 0.0f) {
			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvRight), XMConvertToRadians(x));
			XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
			XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		}
		if (y != 0.0f) {
			mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvUp), XMConvertToRadians(y));
			XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
			XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		}
	}

	XMStoreFloat3(&m_d3dxvLook, XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook)));
	XMStoreFloat3(&m_d3dxvRight, XMVector3Cross(XMLoadFloat3(&m_d3dxvUp), XMLoadFloat3(&m_d3dxvLook)));
	XMStoreFloat3(&m_d3dxvRight, XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight)));
	XMStoreFloat3(&m_d3dxvUp, XMVector3Cross(XMLoadFloat3(&m_d3dxvLook), XMLoadFloat3(&m_d3dxvRight)));
	XMStoreFloat3(&m_d3dxvUp, XMVector3Normalize(XMLoadFloat3(&m_d3dxvUp)));

	cs_rotate rotate;
	rotate.cx = x;
	rotate.cy = y;
	rotate.size = sizeof(cs_rotate);
	rotate.type = CS_ROTATE;

	Sendpacket(reinterpret_cast<unsigned char *>(&rotate));

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
//	m_d3dxvVelocity.y = 0;		// 임시 고정

	Move(XMLoadFloat3(&m_d3dxvVelocity));
	if (m_bIsFloorCollision) OnPlayerUpdated(fTimeElapsed);
	
	// Apply Deceleration 
	XMVECTOR d3dxvDeceleration = -XMLoadFloat3(&m_d3dxvVelocity);
	d3dxvDeceleration = XMVector3Normalize(d3dxvDeceleration);
	fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_d3dxvVelocity)));
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) fDeceleration = fLength;
	XMStoreFloat3(&m_d3dxvVelocity, XMLoadFloat3(&m_d3dxvVelocity) + d3dxvDeceleration * fDeceleration);

	// Camera Update
	if (m_pCamera->GetCameraTag() == CameraTag::eThirdPerson) {
		m_fPitch = 10.0f;
		m_pCamera->Update(fTimeElapsed);
	}
	
	m_pCamera->RegenerateViewMatrix();

	// Character Update
	XMFLOAT4X4 mtx;	XMStoreFloat4x4(&mtx, m_pCharacter->m_mtxWorld);

	mtx._11 = m_d3dxvRight.x;		mtx._12 = m_d3dxvRight.y;		mtx._13 = m_d3dxvRight.z;
	mtx._21 = m_d3dxvUp.x;			mtx._22 = m_d3dxvUp.y;			mtx._23 = m_d3dxvUp.z;
	mtx._31 = m_d3dxvLook.x;		mtx._32 = m_d3dxvLook.y;		mtx._33 = m_d3dxvLook.z;
	mtx._41 = m_d3dxvPosition.x;	mtx._42 = m_d3dxvPosition.y;	mtx._43 = m_d3dxvPosition.z;

	m_pCharacter->m_mtxWorld = XMLoadFloat4x4(&mtx);
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

	m_fPitch = 10.0f;		// 이는 1인칭으로 했을 때 바라보는 방향 각도가 살짝 아래에 있기 때문에 offset으로 넣은 값

	if (nCurrentCameraTag == CameraTag::eSpaceShip)
	{
		m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_d3dxvRight.y = 0.0f;
		m_d3dxvLook.y = 0.0f;
		XMStoreFloat3(&m_d3dxvRight, XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight)));
		XMStoreFloat3(&m_d3dxvLook, XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook)));
		m_fYaw = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat3(&m_d3dxvLook)))));
		if (m_d3dxvLook.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraTag == CameraTag::eSpaceShip) && m_pCamera)
	{
		m_d3dxvRight = m_pCamera->GetRight();
		m_d3dxvUp = m_pCamera->GetUp();
		m_d3dxvLook = m_pCamera->GetLook();
	}

	if (pNewCamera)
	{
		pNewCamera->SetCameraTag(nNewCameraTag);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}void CPlayer::SetKeyDown(KeyInput key)
{
	m_wKeyState |= static_cast<int>(key);


	cs_key_input Key_button;
	Key_button.type = CS_KEYTYPE;
	Key_button.size = sizeof(cs_key_input);
	Key_button.key_button = m_wKeyState;
	Key_button.fDistance = 50.0f;

	Sendpacket(reinterpret_cast<unsigned char *>(&Key_button));

}

void CPlayer::SetKeyUp(KeyInput key)
{
	m_wKeyState ^= static_cast<int>(key);


	cs_key_input Key_button;
	Key_button.type = CS_KEYTYPE;
	Key_button.size = sizeof(cs_key_input);
	Key_button.key_button = m_wKeyState;
	Key_button.fDistance = 5.0f;

	Sendpacket(reinterpret_cast<unsigned char *>(&Key_button));
}
void CPlayer::SetWorldMatrix(XMMATRIX world)
{
	m_pCharacter->m_mtxWorld = world;
}