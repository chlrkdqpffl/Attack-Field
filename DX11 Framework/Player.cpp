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

	// �ӽ�
//	m_fSpeed = 50;
//	m_fSpeed = 30;
	m_fSpeed = 10;	// �ڿ������� �ӵ�
	count = 0;
}

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

void CPlayer::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pCamera) m_pCamera->UpdateShaderVariables(pd3dDeviceContext);
}

void CPlayer::UpdateKeyInput(float fDeltaTime)
{
	// Death Check
	if (m_pCharacter->GetIsDeath())
		return;

	// Keyboard
	XMVECTOR d3dxvShift = XMVectorZero();
	XMVECTOR relativeVelocity = XMVectorZero();


	if (m_wKeyState & static_cast<int>(KeyInput::eForward)) {
		d3dxvShift += XMLoadFloat3(&m_d3dxvLook);
		relativeVelocity += XMVectorSet(0, 0, 1, 0);

	}

	if (m_wKeyState & static_cast<int>(KeyInput::eBackward)) {
		d3dxvShift -= XMLoadFloat3(&m_d3dxvLook);
		relativeVelocity += XMVectorSet(0, 0, -1, 0);

	}

	if (m_wKeyState & static_cast<int>(KeyInput::eLeft)) {
		d3dxvShift -= XMLoadFloat3(&m_d3dxvRight);
		relativeVelocity += XMVectorSet(-1, 0, 0, 0);

	}

	if (m_wKeyState & static_cast<int>(KeyInput::eRight)) {
		d3dxvShift += XMLoadFloat3(&m_d3dxvRight);
		relativeVelocity += XMVectorSet(1, 0, 0, 0);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eReload)) {
		m_pCharacter->SetIsReload(true);
	}
	else
		m_pCharacter->SetIsReload(false);

	if (m_wKeyState & static_cast<int>(KeyInput::eRun)) {
		if (m_pCharacter->GetIsTempRun())		// �ӽ÷� �̷��� �س���. FSM ���� Run ������ ������ �ӷ��� �����ϵ��� - ���� �����ؾ���
			d3dxvShift *= 3;
		m_pCharacter->Running();
	}
	else {
		m_pCharacter->SetIsRun(false);
	}

	// Mouse
	if (m_wKeyState & static_cast<int>(KeyInput::eLeftMouse)) {
		m_pCharacter->SetIsFire(true);

	}
	else {
		m_pCharacter->SetIsFire(false);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eRightMouse)) {
		
	}

	d3dxvShift *= m_fSpeed * fDeltaTime;
	XMStoreFloat3(&m_d3dxvVelocity, XMLoadFloat3(&m_d3dxvVelocity) + d3dxvShift);
	m_pCharacter->SetVelocity(m_d3dxvVelocity);
	m_pCharacter->SetRelativevVelocity(relativeVelocity);
	if (m_pCharacter->IsMoving()) {
		m_pCharacter->Walking();
	}
	
#ifdef	USE_SERVER
	cs_key_input packet;

	packet.type = CS_KEYTYPE;
	packet.size = sizeof(packet);
	packet.key_button = m_wKeyState;
	XMStoreFloat3(&packet.Animation, relativeVelocity);


	packet.x = GetPosition().x;
	packet.y = GetPosition().y;
	packet.z = GetPosition().z;
	packet.FireDirection = m_pCharacter->GetFireDirection();

	packet.Hp = static_cast<BYTE>(GetPlayerLife());


	if ((m_wKeyState != 0) || count == 0)
	{
		SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&packet));
	}
	count++;
#endif
}

void CPlayer::Move(XMVECTOR vTranslate)
{	
	XMVECTOR d3dxvPosition = XMLoadFloat3(&m_d3dxvPosition) + vTranslate;
	XMStoreFloat3(&m_d3dxvPosition, d3dxvPosition);
//	XMVECTOR vPosition = m_pCharacter->GetvPosition() + vTranslate;
//	m_pCharacter->SetPosition(vPosition);
	m_pCamera->Move(vTranslate);
}

void CPlayer::Rotate(float x, float y)
{
	XMMATRIX mtxRotate;
	CameraTag nCurrentCameraTag = m_pCamera->GetCameraTag();
	if ((nCurrentCameraTag == CameraTag::eFirstPerson) || (nCurrentCameraTag == CameraTag::eThirdPerson)) {
		if (x != 0.0f) {
			float fPitch = m_pCharacter->GetPitch();
			fPitch += x;
			if (50.0f < fPitch) {
				x -= (fPitch - 50);
				fPitch = 50;
			}
			if (fPitch < -40.0f) {
				x -= (fPitch + 40);
				fPitch = -40;
			}
			m_pCharacter->SetPitch(fPitch);
		}
		if (y != 0.0f) {
			float fYaw = m_pCharacter->GetYaw();
			fYaw += y;
			if (fYaw  > 360.0f) fYaw -= 360.0f;
			if (fYaw  < 0.0f) fYaw += 360.0f;
			m_pCharacter->SetYaw(fYaw);

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

#ifdef	USE_SERVER
	cs_rotate rotate;
	rotate.cx = x;
	rotate.cy = y;
	rotate.size = sizeof(cs_rotate);
	rotate.type = CS_ROTATE;

	SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&rotate));
#endif
}

void CPlayer::Update(float fDeltaTime)
{
	OnApplyGravity(fDeltaTime);
	m_vPrevPosition = GetPosition();
	Move(XMLoadFloat3(&m_d3dxvVelocity));

	// Apply Deceleration 
	float fLength;
	XMVECTOR d3dxvDeceleration = -XMLoadFloat3(&m_d3dxvVelocity);
	d3dxvDeceleration = XMVector3Normalize(d3dxvDeceleration);
	fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_d3dxvVelocity)));
	float fDeceleration = (m_fFriction * fDeltaTime);
	if (fDeceleration > fLength) fDeceleration = fLength;
	XMStoreFloat3(&m_d3dxvVelocity, XMLoadFloat3(&m_d3dxvVelocity) + d3dxvDeceleration * fDeceleration);
	m_pCharacter->SetVelocity(m_d3dxvVelocity);

	// Camera Update
	if (m_pCamera->GetCameraTag() == CameraTag::eThirdPerson) {
		m_pCharacter->SetPitch(10.0f);
		m_pCamera->Update(fDeltaTime);
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

	m_pCharacter->SetPitch(10.0f);		// �̴� 1��Ī���� ���� �� �ٶ󺸴� ���� ������ ��¦ �Ʒ��� �ֱ� ������ offset���� ���� ��

	if (nCurrentCameraTag == CameraTag::eSpaceShip)
	{
		m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_d3dxvRight.y = 0.0f;
		m_d3dxvLook.y = 0.0f;
		XMStoreFloat3(&m_d3dxvRight, XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight)));
		XMStoreFloat3(&m_d3dxvLook, XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook)));
		float fYaw = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat3(&m_d3dxvLook)))));

		if (m_d3dxvLook.x < 0.0f) fYaw = -fYaw;
		m_pCharacter->SetYaw(fYaw);
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
}

void CPlayer::SetKeyDown(KeyInput key)
{
	m_wKeyState |= static_cast<int>(key);

}

void CPlayer::SetKeyUp(KeyInput key)
{
	m_wKeyState ^= static_cast<int>(key);
	count = 0;

}

void CPlayer::SetLook(float x, float y, float z)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(x),
		XMConvertToRadians(y), XMConvertToRadians(z));

	XMFLOAT4X4 mtx; XMStoreFloat4x4(&mtx, mtxRotate);
	XMFLOAT3 position = GetPosition();

	mtx._41 = position.x;
	mtx._42 = position.y;
	mtx._43 = position.z;

	m_pCharacter->m_mtxWorld = XMLoadFloat4x4(&mtx);
}