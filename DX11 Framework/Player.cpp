#include "stdafx.h"
#include "Player.h"
#include "FirstPersonCamera.h"
#include "SpaceShipCamera.h"
#include "ThirdPersonCamera.h"
#include "protocol.h"

CPlayer::CPlayer(CCharacterObject* pCharacter) 
	: m_pCharacter(pCharacter)
{
	XMStoreFloat4x4(&m_mtxWorld, XMMatrixIdentity());

	m_f3Gravity = XMFLOAT3(0.0f, -50.0f, 0.0f);
	
#ifdef DEVELOP_MODE
	m_fInitSpeed = 20.0f;
#else
	m_fInitSpeed = 6.0f;	// 자연스러운 속도
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
	PxCapsuledesc.radius = 0.5f;
	PxCapsuledesc.height = 2.0f;

	//캐릭터가 올라갈 수있는 장애물의 최대 높이를 정의합니다. 
	PxCapsuledesc.stepOffset = 1.f;

	//캐시 된 볼륨 증가.
	//성능을 향상시키기 위해 캐싱하는 컨트롤러 주변의 공간입니다.  이것은 1.0f보다 커야하지만 너무 크지 않아야하며, 2.0f보다 낮아야합니다.
	PxCapsuledesc.volumeGrowth = 1.9f;
	//캐릭터가 걸어 갈 수있는 최대 경사. 
	PxCapsuledesc.slopeLimit = cosf(XMConvertToRadians(15.f));
	//PxCapsuledesc.nonWalkableMode = PxControllerNonWalkableMode::eFORCE_SLIDING;
	PxCapsuledesc.upDirection = PxVec3(0, 1, 0);
	PxCapsuledesc.contactOffset = 0.0001f; //접촉 오프셋-> 요게 타 객체와 부딪혔을때 영향을 주는 변수인듯(높을수록 덜덜떨림)
	PxCapsuledesc.material = pPxMaterial;
	//PxCapsuledesc.behaviorCallback = this;
	//PxCapsuledesc.reportCallback = this;

	m_pPxCharacterController = pPxControllerManager->createController(PxCapsuledesc);
}

void CPlayer::PhysXMove(float fDeltaTime)
{
	m_pPxCharacterController->move(PxVec3(m_f3MoveDirection.x, m_f3MoveDirection.y, m_f3MoveDirection.z) * m_fInitSpeed * m_fSpeedFactor * fDeltaTime, 0, fDeltaTime, PxControllerFilters());
}

void CPlayer::PhysXUpdate(float fDeltaTime)
{
	/*
	PxControllerState   m_pPxState;

	//피직스 객체의 상태값을 m_pPxState에 넣어준다.
	m_pPxCharacterController->getState(m_pPxState);

	//윗쪽 충돌하거나 아랫쪽 충돌하면 m_fFallvelocity = 0.0f
	if (m_pPxState.collisionFlags == PxControllerCollisionFlag::eCOLLISION_DOWN ||
	m_pPxState.collisionFlags == PxControllerCollisionFlag::eCOLLISION_UP)
	m_fFallvelocity = 0.f;
	*/

	m_pPxCharacterController->move(PxVec3(m_f3Gravity.x, m_f3Gravity.y, m_f3Gravity.z) * fDeltaTime, 0, fDeltaTime, PxControllerFilters());

	//현재 PhysX의 값으로 객체의 월드행렬을 만들어준다.
	XMFLOAT3 position = XMFLOAT3(m_pPxCharacterController->getFootPosition().x, m_pPxCharacterController->getFootPosition().y, m_pPxCharacterController->getFootPosition().z);

	// Character Update
	float characterCenterOffset = 2.1f;
	m_mtxWorld._41 = position.x;
	m_mtxWorld._42 = position.y + characterCenterOffset;
	m_mtxWorld._43 = position.z;
	m_pCharacter->m_mtxWorld = XMLoadFloat4x4(&m_mtxWorld);
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
		if (m_pCharacter->GetAlive())
			m_pCharacter->SetOccupy(true);
	}
	else
		m_pCharacter->SetOccupy(false);


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

	if (m_wKeyState & static_cast<int>(KeyInput::eRun)) {
		if (m_pCharacter->GetIsTempRun())	// 임시로 이렇게 해놓음. FSM 에서 Run 상태일 때에만 속력이 증가하도록 - 추후 수정해야함
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

	// ----- Mouse ----- //
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
			m_fPitch = m_pCharacter->GetPitch();
			m_fPitch += x;
		
			if (50.0f < m_fPitch) {
				x -= (m_fPitch - 50);
				m_fPitch = 50;
			}
			if (m_fPitch  < -40.0f) {
				x -= (m_fPitch + 40);
				m_fPitch = -40;
			}
			
			m_pCharacter->SetPitch(m_fPitch);
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
//		m_pCamera->Rotate(x, y, 0);
	}
	else if (nCurrentCameraTag == CameraTag::eSpaceShip)
	{
//		m_pCamera->Rotate(x, y, 0);
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

		m_pCharacter->SetPrevPitch(abs(m_pCharacter->GetPitch()));   //이전값 저장
		m_pCharacter->SetPrevYaw(abs(m_pCharacter->GetYaw()));      //이전값 저장
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

	// 그 차이가 3보다 작다면 변화를 주지 않음
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
	PhysXUpdate(fDeltaTime);
	UpdateKeyInput(fDeltaTime);
	PhysXMove(fDeltaTime);

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
	/*	// 일단 주석처리하고 문제생기면 다시 넣자 - 09. 12
	else if ((nNewCameraTag == CameraTag::eSpaceShip) && m_pCamera)
	{
		cout << "니냐 ddddddddddddd" << endl;
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