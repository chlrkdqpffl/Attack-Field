#include "stdafx.h"
#include "Player.h"
#include "FirstPersonCamera.h"
#include "SpaceShipCamera.h"
#include "ThirdPersonCamera.h"
#include "protocol.h"

#define InitSpeed 7.0f

CPlayer::CPlayer(CCharacterPlayer* pCharacter) 
	: m_pCharacter(pCharacter)
{
	XMStoreFloat4x4(&m_mtxWorld, XMMatrixIdentity());

	m_fMoveSpeed = InitSpeed;	// 자연스러운 속도
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
	PxCapsuledesc.radius = 0.8f;
	PxCapsuledesc.height = 2.0f;
//	PxCapsuledesc.climbingMode = PxCapsuleClimbingMode::Enum::eCONSTRAINED;
//	PxCapsuledesc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	PxCapsuledesc.stepOffset = 0.1f;	

	// 성능을 향상시키기 위해 캐싱하는 컨트롤러 주변의 공간. 이것은 1.0f보다 커야하지만 너무 크지 않아야하며, 2.0f보다 낮아야함
	PxCapsuledesc.volumeGrowth = 1.9f;
	PxCapsuledesc.slopeLimit = cosf(XMConvertToRadians(40.f));
	
	PxCapsuledesc.upDirection = PxVec3(0, 1, 0);
	PxCapsuledesc.contactOffset = 0.001f;
	PxCapsuledesc.material = pPxMaterial;

	m_pPxCharacterController = pPxControllerManager->createController(PxCapsuledesc);
}

void CPlayer::PhysXUpdate(float fDeltaTime)
{
	// ----- Move Player ----- //
	PxVec3 vMoveVelocity = m_fMoveSpeed * m_fSpeedFactor * PxVec3(m_f3MoveDirection.x, m_f3MoveDirection.y, m_f3MoveDirection.z);
	PxVec3 vAccel = PxVec3(m_f3Accelerate.x, m_f3Accelerate.y, m_f3Accelerate.z);
	vMoveVelocity += vAccel * fDeltaTime;

	m_pPxCharacterController->move(vMoveVelocity * fDeltaTime, 0, fDeltaTime, PxControllerFilters());

	// ----- Apply Gravity ----- //
	m_f3GravityVelocity.x += m_f3GravityAccel.x * fDeltaTime;
	m_f3GravityVelocity.y += m_f3GravityAccel.y * fDeltaTime;
	m_f3GravityVelocity.z += m_f3GravityAccel.z * fDeltaTime;

	PxVec3 vGravityVelocity = PxVec3(m_f3GravityVelocity.x, m_f3GravityVelocity.y, m_f3GravityVelocity.z);
	m_pPxCharacterController->move(vGravityVelocity * fDeltaTime, 0, fDeltaTime, PxControllerFilters());

	// ----- Check Jumping ----- //
	PxControllerState pxState;
	m_pPxCharacterController->getState(pxState);
	if (pxState.collisionFlags == PxControllerCollisionFlag::eCOLLISION_DOWN)
	{	// 바닥과 충돌중
		m_bIsJumping = false;
		m_pCharacter->SetIsJump(false);
		m_f3GravityVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_f3Accelerate = XMFLOAT3(m_f3Accelerate.x, 0, m_f3Accelerate.z);
	}

	// ----- Character Update ----- //
	//float characterCenterOffset = TWBAR_MGR->g_xmf3Offset.x;
	float characterCenterOffset = 0.3f;
	XMFLOAT3 position = XMFLOAT3(m_pPxCharacterController->getPosition().x, m_pPxCharacterController->getPosition().y, m_pPxCharacterController->getPosition().z);

	XMVECTOR vLook = m_pCharacter->GetvLook() * -0.35f;
	XMFLOAT3 look; XMStoreFloat3(&look, vLook);

	m_mtxWorld._41 = position.x + look.x;
	m_mtxWorld._42 = position.y - characterCenterOffset + look.y;	// 포지션은 캐릭터 중심이 기준
	m_mtxWorld._43 = position.z + look.z;
	m_pCharacter->m_mtxWorld = XMLoadFloat4x4(&m_mtxWorld);
}

void CPlayer::AddAccel(XMFLOAT3 force)
{
	m_f3Accelerate.x += force.x;
	m_f3Accelerate.y += force.y;
	m_f3Accelerate.z += force.z;
}

void CPlayer::OnKeyInputUpdate(UINT nMessageID, WPARAM wParam)
{
	// Death Check
	if (m_pCharacter->GetIsDeath()) {
		m_bIsZoom = false;
		m_nFovAngle = 45;
		m_pCamera->GenerateProjectionMatrix(0.05f, 5000.0f, ASPECT_RATIO, m_nFovAngle);
		m_wKeyState = 0;
		return;
	}

	switch (nMessageID) {
	// ----------- Mouse --------- //
	case WM_LBUTTONDOWN:			// Fire
		SetKeyDown(KeyInput::eLeftMouse);
		break;
	case WM_LBUTTONUP:
		SetKeyUp(KeyInput::eLeftMouse);
		break;
	case WM_RBUTTONDOWN:			// Zoom InOut
		if (m_pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eSniperRifle) {
			m_bIsZoom = !m_bIsZoom;

			if (m_bIsZoom)
				m_nFovAngle = 15;
			else 
				m_nFovAngle = 45;

			m_pCamera->GenerateProjectionMatrix(0.05f, 5000.0f, ASPECT_RATIO, m_nFovAngle);
		}
		break;
	case WM_MOUSEWHEEL:
		if (m_bIsZoom) {
			if (m_pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eSniperRifle) {
				if ((SHORT)HIWORD(wParam) > 0)
					m_nFovAngle--;
				else
					m_nFovAngle++;

				m_nFovAngle = clamp(m_nFovAngle, 5, 15);
				//m_nFovAngle = clamp(m_nFovAngle, 1, 15);		// Aim 테스트로 1도까지 올림
				m_pCamera->GenerateProjectionMatrix(0.05f, 5000.0f, ASPECT_RATIO, m_nFovAngle);
			}
		}
		break;

	// ----------- Keyboard --------- //
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
		case VK_CONTROL:
			SetKeyDown(KeyInput::eCrouch);
			break;
		case '1':
		{
			m_bIsZoom = false;
			m_pCamera->GenerateProjectionMatrix(0.05f, 5000.0f, ASPECT_RATIO, 45.0f);

			m_pCharacter->ReplaceWeapon(WeaponTag::eRifle);

#ifdef USE_SERVER
			cs_weapon_type packet;
			packet.size = sizeof(packet);
			packet.type = 13;
			packet.Weapontype = static_cast<BYTE>(WeaponTag::eRifle);

			SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&packet));
#endif
			break;
		}
		case '2':
		{
			m_bIsZoom = false;
			m_pCamera->GenerateProjectionMatrix(0.05f, 5000.0f, ASPECT_RATIO, 45.0f);

			m_pCharacter->ReplaceWeapon(WeaponTag::eSniperRifle);

#ifdef USE_SERVER
			cs_weapon_type packet;
			packet.size = sizeof(packet);
			packet.type = 13;
			packet.Weapontype = static_cast<BYTE>(WeaponTag::eSniperRifle);

			SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&packet));
#endif
		}
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
		case VK_CONTROL:
			SetKeyUp(KeyInput::eCrouch);
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

	XMVECTOR vMoveDirection = XMVectorZero();
	XMVECTOR relativeVelocity = XMVectorZero();
	// =============================== Keyboard ============================== //
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

	if (m_wKeyState & static_cast<int>(KeyInput::eRun)) {
		m_pCharacter->SetIsRun(true);
		if (m_pCharacter->GetIsRunningSpeed()) {
			vMoveDirection = GetvLook();
			m_fSpeedFactor = 1.8f;
			m_pCharacter->Running();
		}
	}
	else {
		m_pCharacter->SetIsRun(false);
		m_fSpeedFactor = 1.0f;
	}

	// ------------------------------------------------------------ //
	if (m_wKeyState & static_cast<int>(KeyInput::eReload)) {
		// Bullet Check
		if (m_pCharacter->GetWeaponBulletCount() == m_pCharacter->GetWeaponMaxBulletCount())
			m_wKeyState ^= static_cast<int>(KeyInput::eReload);
		else
			m_pCharacter->SetIsReload(true);
	}
	
	if (m_wKeyState & static_cast<int>(KeyInput::eOccupy))
		m_pCharacter->SetOccupy(true);
	else
		m_pCharacter->SetOccupy(false);

	if (m_wKeyState & static_cast<int>(KeyInput::eJump)) {	
		if (false == m_bIsJumping) {
			m_bIsJumping = true;
		
			UINT nJumpPower = SCENE_MGR->g_pGameTimer->GetFrameRate() * 15 + 15;
			AddAccel(XMFLOAT3(0.0f, nJumpPower, 0.0f));
			relativeVelocity += XMVectorSet(0, 1, 0, 0);
			m_pCharacter->SetIsJump(true);
		}
	}

	if (m_wKeyState & static_cast<int>(KeyInput::eCrouch)) {
		m_pCharacter->SetIsCrouch(true);
		m_pPxCharacterController->resize(0.5f);
	}
	else {
		m_pCharacter->SetIsCrouch(false);
		m_pPxCharacterController->resize(2.0f);
	}

	// 저격총 들고 있으면 속도 감소
	if (m_pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eSniperRifle)
		m_fSpeedFactor *= 0.8f;

	// 앉기시 이동 금지 or 조금 이동
	if (m_wKeyState & static_cast<int>(KeyInput::eCrouch)) 
		m_fSpeedFactor *= 0.2f;

	XMStoreFloat3(&m_f3MoveDirection, vMoveDirection);
	m_pCharacter->SetRelativevVelocity(relativeVelocity);

	if (m_pCharacter->IsMoving()) {
		m_pCharacter->Walking();
		m_pCharacter->SetOccupy(false);
	}

	// =============================== Mouse ============================== //
	if (m_wKeyState & static_cast<int>(KeyInput::eLeftMouse)) {
		m_pCharacter->SetIsFire(true);
		if (false == m_bIsFire) {
			m_fUserMovePitch = 0.0f;
			m_bIsFire = true;
		}
	}
	else {
		m_pCharacter->GetWeapon()->SetUserMovePitch(m_fUserMovePitch);
		m_pCharacter->SetIsFire(false);
		m_bIsFire = false;
	}
	
#ifdef	USE_SERVER
	cs_key_input packet;

	packet.type = CS_KEYTYPE;
	packet.size = sizeof(packet);
	packet.key_button = m_wKeyState;
	packet.playerPosition = GetPosition();

	if ((m_wKeyState != 0) || count <= 1) {
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

	if (m_bIsZoom) {
		x *= (float) m_nFovAngle / 45;
		y *= (float) m_nFovAngle / 45;
	}

	// 발사하면서 위로 올린 Offset
	if (m_pCharacter->GetWeapon()->GetIsFiring())
		m_fUserMovePitch += x;

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

			if (fYaw > 360.0f) fYaw -= 360.0f;
			if (fYaw < 0.0f) fYaw += 360.0f;
			m_pCharacter->SetYaw(fYaw);

			mtxRotate = XMMatrixRotationAxis(GetvUp(), XMConvertToRadians(y));
			SetvLook(XMVector3TransformNormal(GetvLook(), mtxRotate));
			SetvRight(XMVector3TransformNormal(GetvRight(), mtxRotate));
		}
	}
	else if (nCurrentCameraTag == CameraTag::eFreeCam) {
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
		m_pCharacter->SetPitch(0.0f);

#ifdef	USE_SERVER
	if (abs(m_pCharacter->GetminusPitch()) >= 3.0f || abs(m_pCharacter->GetminusYaw()) >= 3.0f)
	{
		cs_rotate *rotate = reinterpret_cast<cs_rotate *>(SERVER_MGR->GetSendbuffer());
		rotate->mouseRotate.x = m_pCharacter->GetPitch();
		rotate->mouseRotate.y = m_pCharacter->GetYaw();
		rotate->size = sizeof(cs_rotate);
		rotate->type = CS_ROTATE;
		rotate->FireDirection = SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->GetFireDirection();
		SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(rotate));

		m_pCharacter->SetPrevPitch(abs(m_pCharacter->GetPitch()));   //이전값 저장
		m_pCharacter->SetPrevYaw(abs(m_pCharacter->GetYaw()));      //이전값 저장
	}
#endif
}

void CPlayer::UpdateDOF(float fDeltaTime)
{
	if (false == GLOBAL_MGR->g_bActivateDOF) {
		TWBAR_MGR->g_OptionHDR.g_fDOFFarStart = 300.0f;
		TWBAR_MGR->g_OptionHDR.g_fDOFFarRange = 0.0f;
		return;
	}

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

	// 그 차이가 3보다 작다면 변화를 주지 않음
	if (abs(fMinDist - TWBAR_MGR->g_OptionHDR.g_fDOFFarStart) < 3)
		return;

	// 0.8초의 시간을 두고 변화
	if (GetTickCount() - m_dwDOFPreiodTime >= 800) {
		m_nPrevDOFStart = TWBAR_MGR->g_OptionHDR.g_fDOFFarStart;
		m_dwDOFPreiodTime = GetTickCount();
	}

	float dist = fMinDist - m_nPrevDOFStart;
	TWBAR_MGR->g_OptionHDR.g_fDOFFarStart += dist * fDeltaTime / TWBAR_MGR->g_OptionHDR.g_fDOFChangingTime;
}

void CPlayer::Update(float fDeltaTime)
{
//	UpdateDOF(fDeltaTime);		 임시로 DOF Off
	UpdateKeyState(fDeltaTime);
	PhysXUpdate(fDeltaTime);
	
	// Camera Update
	m_pCamera->OnCollisionCheck();
	m_pCamera->Update(fDeltaTime);
	m_pCamera->UpdateOffset();
	m_pCamera->RegenerateViewMatrix();
	m_pCamera->UpdateShaderVariables(STATEOBJ_MGR->g_pd3dImmediateDeviceContext);
}

CCamera *CPlayer::OnChangeCamera(ID3D11Device *pd3dDevice, CameraTag nNewCameraTag, CameraTag nCurrentCameraTag)
{
	CCamera *pNewCamera = NULL;
	switch (nNewCameraTag) {
		case CameraTag::eFirstPerson:
			pNewCamera = new CFirstPersonCamera(m_pCamera);

			//m_f3GravityAccel = XMFLOAT3(0.0f, -1.0f * TWBAR_MGR->g_xmf3Quaternion.y, 0.0f);
			m_f3GravityAccel = XMFLOAT3(0.0f, -55.0f, 0.0f);
			m_fMoveSpeed = InitSpeed;
			break;
		case CameraTag::eFreeCam:
			pNewCamera = new CSpaceShipCamera(m_pCamera);

			m_f3GravityAccel = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_fMoveSpeed = InitSpeed * 4;
			break;
		case CameraTag::eThirdPerson:
			pNewCamera = new CThirdPersonCamera(m_pCamera);

			m_f3GravityAccel = XMFLOAT3(0.0f, -55.0f, 0.0f);
			m_fMoveSpeed = InitSpeed;
			break;
	}

	if (nCurrentCameraTag == CameraTag::eFreeCam)
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
	if (pNewCamera) {
		pNewCamera->SetCameraTag(nNewCameraTag);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	SCENE_MGR->g_pCamera = pNewCamera;
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