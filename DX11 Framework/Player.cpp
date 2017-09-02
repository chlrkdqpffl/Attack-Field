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
	m_fFriction = 0.0f;

#ifdef DEVELOP_MODE
	m_fSpeed = 20;
#else
	m_fSpeed = 6;	// 자연스러운 속도
#endif
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
		// Bullet Check
		if (m_pCharacter->GetWeaponBulletCount() == m_pCharacter->GetWeaponMaxBulletCount())
			m_wKeyState ^= static_cast<int>(KeyInput::eReload);
		else
			m_pCharacter->SetIsReload(true);
	}
	else
		m_pCharacter->SetIsReload(false);

	if (m_wKeyState & static_cast<int>(KeyInput::eOccupy))
		m_pCharacter->SetOccupy(true);
	else
		m_pCharacter->SetOccupy(false);


	if (m_wKeyState & static_cast<int>(KeyInput::eRun)) {
		if (m_pCharacter->GetIsTempRun()) {		// 임시로 이렇게 해놓음. FSM 에서 Run 상태일 때에만 속력이 증가하도록 - 추후 수정해야함
			d3dxvShift += XMLoadFloat3(&m_d3dxvLook) * 2;

			relativeVelocity += XMVectorSet(0, 0, 1, 0);
		}
		m_pCharacter->Running();
	}
	else {
		m_pCharacter->SetIsRun(false);
	}

	// Mouse
	if (m_wKeyState & static_cast<int>(KeyInput::eLeftMouse))
	{
		m_pCharacter->SetIsFire(true);
#ifdef USE_SERVER
		CS_Fire packet;
		packet.size = sizeof(packet);
		packet.type = 10;
		//packet.fire = true;
		packet.FireDirection = m_pCharacter->GetFireDirection();
		SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&packet));
#endif
	}
	else
		m_pCharacter->SetIsFire(false);

	if (m_wKeyState & static_cast<int>(KeyInput::eRightMouse)) {

	}

	d3dxvShift *= m_fSpeed * fDeltaTime;
	XMStoreFloat3(&m_d3dxvVelocity, XMLoadFloat3(&m_d3dxvVelocity) + d3dxvShift);
	m_pCharacter->SetVelocity(m_d3dxvVelocity);
	m_pCharacter->SetRelativevVelocity(relativeVelocity);

	if (m_pCharacter->IsMoving())
		m_pCharacter->Walking();

#ifdef	USE_SERVER
	cs_key_input packet;

	packet.type = CS_KEYTYPE;
	packet.size = sizeof(packet);
	packet.key_button = m_wKeyState;
	XMStoreFloat3(&packet.Animation, relativeVelocity);


	packet.x = GetPosition().x;
	packet.y = GetPosition().y;
	packet.z = GetPosition().z;


	if ((m_wKeyState != 0) || count == 0) {
		WORD temp = static_cast<int>(KeyInput::eLeftMouse);
		WORD key_button_e = static_cast<int>(KeyInput::eOccupy);
		if (m_wKeyState != temp && key_button_e != m_wKeyState)
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
	// Death Check
	if (m_pCharacter->GetIsDeath())
		return;

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
			if (fYaw > 360.0f) fYaw -= 360.0f;
			if (fYaw < 0.0f) fYaw += 360.0f;
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
	if (abs(m_pCharacter->GetminusPitch()) >= 1.0f || abs(m_pCharacter->GetminusYaw()) >= 1.0f)
	{
		cs_rotate *rotate = reinterpret_cast<cs_rotate *>(SERVER_MGR->GetSendbuffer());
		rotate->cx = m_pCharacter->GetPitch();
		rotate->cy = m_pCharacter->GetYaw();
		rotate->size = sizeof(cs_rotate);
		rotate->type = CS_ROTATE;

		SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(rotate));

		m_pCharacter->Setbfpitch(abs(m_pCharacter->GetPitch()));   //이전값 저장
		m_pCharacter->Setbfyaw(abs(m_pCharacter->GetYaw()));      //이전값 저장
}
#endif
}

void CPlayer::UpdateDOF(float fDeltaTime)
{
	// DOF 거리 조절
	CollisionInfo info;
	float fMinDist = 0.0f;

	CWeapon* pWeapon = m_pCharacter->GetWeapon();

	XMVECTOR fireOffsetPos = pWeapon->GetvPosition() + (-1 * pWeapon->GetvRight() * (pWeapon->GetBoundingBox().Extents.z - 0.6f)) + (-1 * pWeapon->GetvLook() * 0.225) + (pWeapon->GetvUp() * 0.1f);
	XMVECTOR fireDirection = XMLoadFloat3(&m_pCharacter->GetFireDirection());

	if (COLLISION_MGR->RayCastCollision(info, fireOffsetPos, fireDirection))
		fMinDist = info.m_fDistance;

	if (COLLISION_MGR->RayCastCollisionToCharacter(info, fireOffsetPos, fireDirection)) {
		if (info.m_fDistance <= fMinDist)
			fMinDist = info.m_fDistance;
	}
	
	// 가장 가까운 객체가 없는 경우 - 하늘을 보는 경우
	if (fMinDist == 0.0f)
		fMinDist = 300.0f;

	// 그 차이가 5보다 작다면 변화를 주지 않음
	if (abs(fMinDist - TWBAR_MGR->g_OptionHDR.g_fDOFFarStart) < 3)
		return;

	// 1초의 시간을 두고 변화
	if (GetTickCount() - m_dwDOFPreiodTime >= 1000) {
		m_nPrevDOFStart = TWBAR_MGR->g_OptionHDR.g_fDOFFarStart;
		m_dwDOFPreiodTime = GetTickCount();
	}

	float dist = fMinDist - m_nPrevDOFStart;
	TWBAR_MGR->g_OptionHDR.g_fDOFFarStart += dist * fDeltaTime / TWBAR_MGR->g_OptionHDR.g_fDOFChangingTime;
}

void CPlayer::Update(float fDeltaTime)
{
	UpdateDOF(fDeltaTime);
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
	XMFLOAT4X4 mtx;
	XMStoreFloat4x4(&mtx, m_pCharacter->m_mtxWorld);

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

	if(m_pCharacter)
		m_pCharacter->SetPitch(10.0f);		// 이는 1인칭으로 했을 때 바라보는 방향 각도가 살짝 아래에 있기 때문에 offset으로 넣은 값

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