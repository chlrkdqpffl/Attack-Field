#include "stdafx.h"
#include "Player.h"
#include "FirstPersonCamera.h"
#include "SpaceShipCamera.h"
#include "ThirdPersonCamera.h"
#include "protocol.h"

CPlayer::CPlayer(CCharacterPlayer* pCharacter) 
	: m_pCharacter(pCharacter)
{
	XMStoreFloat4x4(&m_mtxWorld, XMMatrixIdentity());

	m_f3Gravity = XMFLOAT3(0.0f, -50.0f, 0.0f);
	
#ifdef DEVELOP_MODE
	m_fInitSpeed = 20.0f;
#else
	m_fInitSpeed = 6.0f;	// �ڿ������� �ӵ�
#endif
	count = 0;
}

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

void CPlayer::InitializePhysXData(PxPhysics* pPxPhysics, PxMaterial *pPxMaterial, PxControllerManager *pPxControllerManager)
{
	PxCapsuleControllerDesc	PxCapsuledesc;
	PxCapsuledesc.position = PxExtendedVec3(0, 0, 0);
	PxCapsuledesc.radius = 1.2f;
	PxCapsuledesc.height = 2.0f;

	//ĳ���Ͱ� �ö� ���ִ� ��ֹ��� �ִ� ���̸� �����մϴ�. 
	PxCapsuledesc.stepOffset = 0.8f;

	//ĳ�� �� ���� ����.
	//������ ����Ű�� ���� ĳ���ϴ� ��Ʈ�ѷ� �ֺ��� �����Դϴ�.  �̰��� 1.0f���� Ŀ�������� �ʹ� ũ�� �ʾƾ��ϸ�, 2.0f���� ���ƾ��մϴ�.
	PxCapsuledesc.volumeGrowth = 1.9f;
	//ĳ���Ͱ� �ɾ� �� ���ִ� �ִ� ���. 
	PxCapsuledesc.slopeLimit = cosf(XMConvertToRadians(15.f));
	//PxCapsuledesc.nonWalkableMode = PxControllerNonWalkableMode::eFORCE_SLIDING;
	PxCapsuledesc.upDirection = PxVec3(0, 1, 0);
	PxCapsuledesc.contactOffset = 0.0001f; //���� ������-> ��� Ÿ ��ü�� �ε������� ������ �ִ� �����ε�(�������� ��������)
	PxCapsuledesc.material = pPxMaterial;
	//PxCapsuledesc.behaviorCallback = this;
	//PxCapsuledesc.reportCallback = this;

	m_pPxCharacterController = pPxControllerManager->createController(PxCapsuledesc);
}

void CPlayer::PhysXUpdate(float fDeltaTime)
{
	/*
	PxControllerState   m_pPxState;

	//������ ��ü�� ���°��� m_pPxState�� �־��ش�.
	m_pPxCharacterController->getState(m_pPxState);

	//���� �浹�ϰų� �Ʒ��� �浹�ϸ� m_fFallvelocity = 0.0f
	if (m_pPxState.collisionFlags == PxControllerCollisionFlag::eCOLLISION_DOWN ||
	m_pPxState.collisionFlags == PxControllerCollisionFlag::eCOLLISION_UP)
	m_fFallvelocity = 0.f;
	*/

	PxVec3 vGravityVelocity = PxVec3(m_f3Gravity.x, m_f3Gravity.y, m_f3Gravity.z);
	PxVec3 vMoveVelocity = m_fInitSpeed * m_fSpeedFactor * PxVec3(m_f3MoveDirection.x, m_f3MoveDirection.y, m_f3MoveDirection.z);

	PxVec3 vCalcMoveVelocity = vGravityVelocity + vMoveVelocity;
	m_pPxCharacterController->move(vCalcMoveVelocity * fDeltaTime, 0, fDeltaTime, PxControllerFilters());

	m_fJumpSpeed = TWBAR_MGR->g_xmf3Offset.y;

	// Character Update
	float characterCenterOffset = 1.6f;
	XMFLOAT3 footPosition = XMFLOAT3(m_pPxCharacterController->getFootPosition().x, m_pPxCharacterController->getFootPosition().y, m_pPxCharacterController->getFootPosition().z);
	m_mtxWorld._41 = footPosition.x;
	m_mtxWorld._42 = footPosition.y + characterCenterOffset;
	m_mtxWorld._43 = footPosition.z;
	m_pCharacter->m_mtxWorld = XMLoadFloat4x4(&m_mtxWorld);
}

void CPlayer::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pCamera) m_pCamera->UpdateShaderVariables(pd3dDeviceContext);
}

void CPlayer::OnKeyboardUpdate(UINT nMessageID, WPARAM wParam)
{
	switch (nMessageID) {
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_W:
			SetKeyDown(KeyInput::eForward);
			break;
		case VK_S:
			SetKeyDown(KeyInput::eBackward);
			break;
		case VK_A:
			SetKeyDown(KeyInput::eLeft);
			break;
		case VK_D:
			SetKeyDown(KeyInput::eRight);
			break;
		case VK_SHIFT:
			SetKeyDown(KeyInput::eRun);
			break;
		case VK_R:
			SetKeyDown(KeyInput::eReload);
			break;
		case VK_E:
			SetKeyDown(KeyInput::eOccupy);
			break;
		case VK_SPACE:
			SetKeyDown(KeyInput::eJump);
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam) {
		case VK_W:
			SetKeyUp(KeyInput::eForward);
			break;
		case VK_S:
			SetKeyUp(KeyInput::eBackward);
			break;
		case VK_A:
			SetKeyUp(KeyInput::eLeft);
			break;
		case VK_D:
			SetKeyUp(KeyInput::eRight);
			break;
		case VK_R:
			SetKeyUp(KeyInput::eReload);
			break;
		case VK_SHIFT:
			SetKeyUp(KeyInput::eRun);
			break;
		case VK_E:
			SetKeyUp(KeyInput::eOccupy);
			break;
		case VK_SPACE:
			SetKeyUp(KeyInput::eJump);
			break;
		}
		break;
	}
}

void CPlayer::UpdateKeyState(float fDeltaTime)
{
	// Death Check
	if (m_pCharacter->GetIsDeath()) {
		m_fSpeedFactor = 0.0f;
		return;
	}

	// =============================== Keyboard ============================== //
	if (m_wKeyState & static_cast<int>(KeyInput::eReload)) {
		// Bullet Check
		if (m_pCharacter->GetWeaponBulletCount() == m_pCharacter->GetWeaponMaxBulletCount())
			m_wKeyState ^= static_cast<int>(KeyInput::eReload);
		else
			m_pCharacter->SetIsReload(true);
	}
	else
		m_pCharacter->SetIsReload(false);

	if (m_wKeyState & static_cast<int>(KeyInput::eOccupy)) {
//		if (m_pCharacter->GetAlive())
			m_pCharacter->SetOccupy(true);
	}
	else
		m_pCharacter->SetOccupy(false);

	// ------------ Moving ----------- //
	XMVECTOR vMoveDirection = XMVectorZero();
	XMVECTOR relativeVelocity = XMVectorZero();


	if (m_wKeyState & static_cast<int>(KeyInput::eForward))
	{
		vMoveDirection += GetvLook();
		relativeVelocity += XMVectorSet(0, 0, 1, 0);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eBackward))
	{
		vMoveDirection -= GetvLook();
		relativeVelocity += XMVectorSet(0, 0, -1, 0);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eLeft)) {
		vMoveDirection -= GetvRight();
		relativeVelocity += XMVectorSet(-1, 0, 0, 0);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eRight)) {
		vMoveDirection += GetvRight();
		relativeVelocity += XMVectorSet(1, 0, 0, 0);
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eJump)) {
		if (false == m_bIsJumping) {
			cout << "������" << endl;
			m_bIsJumping = true;
			vMoveDirection += GetvUp() * TWBAR_MGR->g_xmf3Offset.y;

			//relativeVelocity += XMVectorSet(0, 1, 0, 0);
			m_pCharacter->SetIsJump(true);
		}
	}
	else {
		m_pCharacter->SetIsJump(false);
		m_bIsJumping = false;
	}
	


	if (m_wKeyState & static_cast<int>(KeyInput::eRun)) {
		if (m_pCharacter->GetIsTempRun())	// �ӽ÷� �̷��� �س���. FSM ���� Run ������ ������ �ӷ��� �����ϵ��� - ���� �����ؾ���
			vMoveDirection = GetvLook();
		m_fSpeedFactor = 2.0f;
		m_pCharacter->Running();
	}
	else {
		m_pCharacter->SetIsRun(false);
		m_fSpeedFactor = 1.0f;
	}


	XMStoreFloat3(&m_f3MoveDirection, vMoveDirection);
	m_pCharacter->SetRelativevVelocity(relativeVelocity);

	// =============================== Mouse ============================== //
	if (m_wKeyState & static_cast<int>(KeyInput::eLeftMouse))
		m_pCharacter->SetIsFire(true);
	else 
		m_pCharacter->SetIsFire(false);

	if (m_pCharacter->IsMoving())
		m_pCharacter->Walking();


#ifdef	USE_SERVER
	cs_key_input packet;

	packet.type = CS_KEYTYPE;
	packet.size = sizeof(packet);
	packet.key_button = m_wKeyState;
	packet.playerPosition = GetPosition();

	if ((m_wKeyState != 0) || count == 0) {
		//WORD temp = static_cast<int>(KeyInput::eLeftMouse);
		WORD key_button_e = static_cast<int>(KeyInput::eOccupy);
		if (key_button_e != m_wKeyState)
			SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&packet));
	}
	count++;
#endif
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
			fPitch = clamp(fPitch, -40.0f, 50.0f);
			m_pCharacter->SetPitch(fPitch);
		}
		if (y != 0.0f) {
			float fYaw = m_pCharacter->GetYaw();
			fYaw += y;
			fYaw = clamp(fYaw, 0.0f, 360.0f);
			m_pCharacter->SetYaw(fYaw);

			mtxRotate = XMMatrixRotationAxis(GetvUp(), XMConvertToRadians(y));
			SetvLook(XMVector3TransformNormal(GetvLook(), mtxRotate));
			SetvRight(XMVector3TransformNormal(GetvRight(), mtxRotate));
		}
	}
	else if (nCurrentCameraTag == CameraTag::eSpaceShip) {
		if (x != 0.0f) {
			mtxRotate = XMMatrixRotationAxis(GetvRight(), XMConvertToRadians(x));
			SetvUp(XMVector3TransformNormal(GetvUp(), mtxRotate));
			SetvLook(XMVector3TransformNormal(GetvLook(), mtxRotate));
		}
		if (y != 0.0f) {
			mtxRotate = XMMatrixRotationAxis(GetvUp(), XMConvertToRadians(y));
			SetvRight(XMVector3TransformNormal(GetvRight(), mtxRotate));
			SetvLook(XMVector3TransformNormal(GetvLook(), mtxRotate));
		}
	}

	SetvLook(XMVector3Normalize(GetvLook()));
	SetvRight(XMVector3Normalize(XMVector3Cross(GetvUp(), GetvLook())));
	SetvUp(XMVector3Normalize(XMVector3Cross(GetvLook(), GetvRight())));

	if (nCurrentCameraTag == CameraTag::eThirdPerson)
		m_pCharacter->SetPitch(-10.0f);

#ifdef	USE_SERVER
	if (abs(m_pCharacter->GetminusPitch()) >= 1.0f || abs(m_pCharacter->GetminusYaw()) >= 1.0f)
	{
		cs_rotate *rotate = reinterpret_cast<cs_rotate *>(SERVER_MGR->GetSendbuffer());
		rotate->mouseRotate.x = m_pCharacter->GetPitch();
		rotate->mouseRotate.y = m_pCharacter->GetYaw();
		rotate->size = sizeof(cs_rotate);
		rotate->type = CS_ROTATE;
		rotate->FireDirection = SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->GetFireDirection();
		SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(rotate));

		m_pCharacter->Setbfpitch(abs(m_pCharacter->GetPitch()));   //������ ����
		m_pCharacter->Setbfyaw(abs(m_pCharacter->GetYaw()));      //������ ����
	}
#endif
}

void CPlayer::UpdateDOF(float fDeltaTime)
{
	// DOF �Ÿ� ����
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
	
	// ���� ����� ��ü�� ���� ��� - �ϴ��� ���� ���
	if (fMinDist == 0.0f)
		fMinDist = 300.0f;

	// �� ���̰� 3���� �۴ٸ� ��ȭ�� ���� ����
	if (abs(fMinDist - TWBAR_MGR->g_OptionHDR.g_fDOFFarStart) < 3)
		return;

	// 1���� �ð��� �ΰ� ��ȭ
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
	UpdateKeyState(fDeltaTime);
	PhysXUpdate(fDeltaTime);
	
	// Camera Update
	m_pCamera->Update(fDeltaTime);
	m_pCamera->RegenerateViewMatrix();
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
		SetUp(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_mtxWorld._12 = 0.0f;
		m_mtxWorld._32 = 0.0f;
		SetvRight(XMVector3Normalize(GetvRight()));
		SetvLook(XMVector3Normalize(GetvLook()));
		float fYaw = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), GetvLook()))));

		if (m_mtxWorld._31 < 0.0f) fYaw = -fYaw;
		m_pCharacter->SetYaw(fYaw);
	}
	/*	// �ϴ� �ּ�ó���ϰ� ��������� �ٽ� ���� - 09. 12
	else if ((nNewCameraTag == CameraTag::eSpaceShip) && m_pCamera)
	{
		cout << "�ϳ� ddddddddddddd" << endl;
		SetRight(m_pCamera->GetRight());
		SetUp(m_pCamera->GetUp());
		SetLook(m_pCamera->GetLook());
	}
	*/

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