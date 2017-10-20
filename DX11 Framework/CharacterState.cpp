#include "stdafx.h"
#include "CharacterState.h"

void CState_AnyTime::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(CharacterParts::LowerBody);
	
	if (pCharacter->GetCharacterID() == 0) {
		if (false == SOUND_MGR->IsMaxSound())
			SOUND_MGR->AddVolume(0.002f);
	}

	// 죽은 상태일 때에는 상태 확인 X
	if (pUpperFSM->GetCurrentState() == CState_Death::GetInstance()) {
		if (pLowerFSM->GetCurrentState() == CState_Death::GetInstance())
			return;
	}

	if (pCharacter->GetIsDeath()) {
		pUpperFSM->ChangeState(CState_Death::GetInstance());
		pLowerFSM->ChangeState(CState_Death::GetInstance());
		return;
	}
	else if(pCharacter->GetIsHeadHit()) {
		pUpperFSM->ChangeState(CState_HeadHit::GetInstance());
		return;
	}
	else if (pCharacter->GetLife() <= 30) {
		pCharacter->SetIsDeadly(true);		// Show Deadly UI
	}
}

void CState_Idle::EnterState(CCharacterObject* pCharacter, CharacterParts type)
{
	pCharacter->SetAnimation(type, AnimationTag::eIdle);
}

void CState_Idle::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(CharacterParts::LowerBody);

	// Check ReplaceWeapon
	if (pCharacter->GetIsReplaceWeapon()) {
		pUpperFSM->ChangeState(CState_ReplacementWeapon::GetInstance());
		return;
	}

	// Check Reload
	if (pCharacter->GetIsReload()) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}

	// Check Fire
	if (pCharacter->GetIsFire()) {
		if(pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eRifle)
			pUpperFSM->ChangeState(CState_FireLoop::GetInstance());
		else if (pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eSniperRifle) {
			if (false == pCharacter->GetWeapon()->GetIsFiring())
				pUpperFSM->ChangeState(CState_Fire::GetInstance());
		}
		return;
	}

	// 상체가 Idle 상태일 경우 하체의 움직임을 따라간다.
	AnimationTag lowerAnim = pCharacter->GetAnimationEnum(CharacterParts::LowerBody);
	if (lowerAnim != AnimationTag::eIdle)
		pUpperFSM->ChangeState(pLowerFSM->GetCurrentState());
}

void CState_Idle::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(CharacterParts::LowerBody);

	if (pCharacter->IsMoving()) {
		pLowerFSM->ChangeState(CState_Walk::GetInstance());
		return;
	}

	if (pCharacter->GetIsCrouch()) {
		pLowerFSM->ChangeState(CState_Crouch::GetInstance());
		return;
	}
}

void CState_Idle::ExitState(CCharacterObject* pCharacter, CharacterParts type)
{
}

// ---------------------------- Walking ---------------------------- //
void CState_Walk::EnterState(CCharacterObject* pCharacter, CharacterParts type)
{
	if(type == CharacterParts::UpperBody)
		pCharacter->SetAnimation(CharacterParts::UpperBody, AnimationTag::eWalk_Forward);
}

void CState_Walk::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(CharacterParts::LowerBody);
	
	// Check ReplaceWeapon
	if (pCharacter->GetIsReplaceWeapon()) {
		pUpperFSM->ChangeState(CState_ReplacementWeapon::GetInstance());
		return;
	}

	// Check Reload 
	if (pCharacter->GetIsReload()) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}

	// Check Fire
	if (pCharacter->GetIsFire()) {
		if (pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eRifle)
			pUpperFSM->ChangeState(CState_FireLoop::GetInstance());
		else if (pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eSniperRifle) {
			if (false == pCharacter->GetWeapon()->GetIsFiring())
				pUpperFSM->ChangeState(CState_Fire::GetInstance());
		}
		return;
	}

	AnimationTag lowerAnim = pCharacter->GetAnimationEnum(CharacterParts::LowerBody);
	if (lowerAnim == AnimationTag::eIdle)
		pUpperFSM->ChangeState(pLowerFSM->GetCurrentState());
}

void CState_Walk::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(CharacterParts::LowerBody);
	
	// Check Run
	if (pCharacter->GetIsRun()) {
		if (!pCharacter->GetIsReload()) {
			pUpperFSM->ChangeState(CState_Run::GetInstance());
			pLowerFSM->ChangeState(CState_Run::GetInstance());

			return;
		}
	}

	// Check Walk Direction 
	AnimationTag characterAnim = pCharacter->GetAnimationEnum(CharacterParts::LowerBody);

	XMFLOAT3 relativeVelocity = pCharacter->GetRelativeVelocity();		// 추후 방향만 있는 것이 아닌 속력까지 갖도록 설정하기. 현재는 방향만 존재

	// Forward
	if (relativeVelocity.z > 0) {
		if (relativeVelocity.x < 0) {		// Left
			pCharacter->SetAnimation(AnimationTag::eWalk_ForwardLeft, 1.3f);
		}
		else if (relativeVelocity.x > 0) {	// Right
			pCharacter->SetAnimation(AnimationTag::eWalk_ForwardRight, 1.3f);
		}
		else {
			pCharacter->SetAnimation(AnimationTag::eWalk_Forward, 1.5f);
		}
	}
	// Backward
	else if (relativeVelocity.z < 0) {
		if (relativeVelocity.x < 0) {		// Left
			pCharacter->SetAnimation(AnimationTag::eWalk_BackwardLeft, 1.3f);
		}
		else if (relativeVelocity.x > 0) {	// Right
			pCharacter->SetAnimation(AnimationTag::eWalk_BackwardRight, 1.3f);
		}
		else {
			pCharacter->SetAnimation(AnimationTag::eWalk_Backward, 1.5f);
		}
	}
	else {
		if (relativeVelocity.x < 0) {		// Left
			pCharacter->SetAnimation(AnimationTag::eWalk_Left, 1.3f);
		}
		else if (relativeVelocity.x > 0) {	// Right
			pCharacter->SetAnimation(AnimationTag::eWalk_Right, 1.3f);
		}
	}

	if (pCharacter->GetIsCrouch()) {
		pUpperFSM->ChangeState(CState_Idle::GetInstance());
		pLowerFSM->ChangeState(CState_Crouch::GetInstance());
		return;
	}

	if (!pCharacter->IsMoving())
		pLowerFSM->ChangeState(CState_Idle::GetInstance());
}

void CState_Walk::ExitState(CCharacterObject* pCharacter, CharacterParts type)
{
	SOUND_MGR->StopSound();
}

// ---------------------------- Crouch ---------------------------- //
void CState_Crouch::EnterState(CCharacterObject* pCharacter, CharacterParts type)
{
	if (type == CharacterParts::LowerBody)
		pCharacter->SetAnimation(CharacterParts::LowerBody, AnimationTag::eCrouch);
}

void CState_Crouch::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(CharacterParts::LowerBody);

	// Check ReplaceWeapon
	if (pCharacter->GetIsReplaceWeapon()) {
		pUpperFSM->ChangeState(CState_ReplacementWeapon::GetInstance());
		return;
	}

	// Check Reload
	if (pCharacter->GetIsReload()) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}

	// Check Fire
	if (pCharacter->GetIsFire()) {
		if (pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eRifle)
			pUpperFSM->ChangeState(CState_FireLoop::GetInstance());
		else if (pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eSniperRifle) {
			if (false == pCharacter->GetWeapon()->GetIsFiring())
				pUpperFSM->ChangeState(CState_Fire::GetInstance());
		}
		return;
	}

	if (false == pCharacter->GetIsCrouch()) {
		pUpperFSM->ChangeState(CState_Idle::GetInstance());
		return;
	}
}

void CState_Crouch::UpdateLowerBodyState(CCharacterObject* pCharacter)
{	
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(CharacterParts::LowerBody);

	if (false == pCharacter->GetIsCrouch()) {
		if (pCharacter->IsMoving()) {
			pLowerFSM->ChangeState(CState_Walk::GetInstance());
			return;
		}
		else {
			pLowerFSM->ChangeState(CState_Idle::GetInstance());
			return;
		}
	}
}

void CState_Crouch::ExitState(CCharacterObject* pCharacter, CharacterParts type)
{
}

// ---------------------------- Reload ---------------------------- //
void CState_Reload::EnterState(CCharacterObject* pCharacter, CharacterParts type)
{
	if (type == CharacterParts::LowerBody)
		return;

	if (pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eRifle)
		pCharacter->SetAnimation(AnimationTag::eReload, 1.6f);
	else if (pCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eSniperRifle)
		pCharacter->SetAnimation(AnimationTag::eReload, 1.3f);

	pCharacter->GetWeapon()->SetFireBulletCount(0);
	SOUND_MGR->Play3DSound(SoundTag::eReload, pCharacter->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);

	if (pCharacter->GetCharacterID() == 0) {
		SCENE_MGR->g_pPlayer->SetIsZoom(false);
		if (SCENE_MGR->g_pCamera->GetCameraTag() == CameraTag::eFirstPerson)
			SCENE_MGR->g_pCamera->GenerateProjectionMatrix(0.05f, 5000.0f, ASPECT_RATIO, 45.0f);
		else
			SCENE_MGR->g_pCamera->GenerateProjectionMatrix(0.05f, 5000.0f, ASPECT_RATIO, 60.0f);
	}
}

void CState_Reload::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	if(!pCharacter->GetControllerActive(CharacterParts::UpperBody)) {
		pUpperFSM->ChangeState(CState_Idle::GetInstance());
		pCharacter->SetControllerActive(CharacterParts::UpperBody, true);
		return;
	}
}

void CState_Reload::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_Reload::ExitState(CCharacterObject* pCharacter, CharacterParts type)
{
	pCharacter->Reloading();
	pCharacter->SetIsReload(false);
}

// ---------------------------- Weapon Replacement ---------------------------- //
void CState_ReplacementWeapon::EnterState(CCharacterObject* pCharacter, CharacterParts type)
{
	if (type == CharacterParts::LowerBody)
		return;

	pCharacter->SetAnimation(AnimationTag::eReload, 1.6f);
	pCharacter->GetWeapon()->SetFireBulletCount(0);
	SOUND_MGR->Play3DSound(SoundTag::eReload, pCharacter->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);

	m_dwReplacementTime = GetTickCount();
}

void CState_ReplacementWeapon::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	if (!pCharacter->GetControllerActive(CharacterParts::UpperBody)) {
		pUpperFSM->ChangeState(CState_Idle::GetInstance());
		pCharacter->SetControllerActive(CharacterParts::UpperBody, true);
		return;
	}

	// 0.7초 뒤 무기 교체
	if (GetTickCount() - m_dwReplacementTime > 700) {
		pCharacter->SetWeaponNumber(pCharacter->GetNextWeaponNumber());
		pCharacter->Reloading();
	}
}

void CState_ReplacementWeapon::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_ReplacementWeapon::ExitState(CCharacterObject* pCharacter, CharacterParts type)
{
	pCharacter->SetIsReplaceWeapon(false);
}

// ---------------------------- Fire ---------------------------- //
void CState_Fire::EnterState(CCharacterObject* pCharacter, CharacterParts type)
{
	pCharacter->SetAnimation(AnimationTag::eFire);
}

void CState_Fire::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	if (!pCharacter->GetControllerActive(CharacterParts::UpperBody)) {
		pUpperFSM->ChangeState(CState_Idle::GetInstance());
		pCharacter->SetControllerActive(CharacterParts::UpperBody, true);
		return;
	}
	
	// Check BulletCount
	if (pCharacter->GetWeaponBulletCount() == 0) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}
	
	pCharacter->Firing();
}

void CState_Fire::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_Fire::ExitState(CCharacterObject* pCharacter, CharacterParts type)
{
	SPRITE_MGR->DisableSprite(pCharacter->GetMuzzleSprite());
}

// ---------------------------- Fire Loop ---------------------------- //
void CState_FireLoop::EnterState(CCharacterObject* pCharacter, CharacterParts type)
{
	pCharacter->SetAnimation(AnimationTag::eFireLoop);
}

void CState_FireLoop::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);

	// Check ReplaceWeapon
	if (pCharacter->GetIsReplaceWeapon()) {
		pUpperFSM->ChangeState(CState_ReplacementWeapon::GetInstance());
		return;
	}

	// Check BulletCount
	if (pCharacter->GetWeaponBulletCount() == 0) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}

	// Check Reload
	if (pCharacter->GetIsReload()) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}

	// Check not Fire
	if (!pCharacter->GetIsFire()) {
		pUpperFSM->ChangeState(CState_Idle::GetInstance());
		return;
	}

	pCharacter->Firing();
}

void CState_FireLoop::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_FireLoop::ExitState(CCharacterObject* pCharacter, CharacterParts type)
{
	SPRITE_MGR->DisableSprite(pCharacter->GetMuzzleSprite());
}

// ---------------------------- Run ---------------------------- //
void CState_Run::EnterState(CCharacterObject* pCharacter, CharacterParts type)
{
	if (type == CharacterParts::LowerBody)
		return;

	SOUND_MGR->Play3DSound(SoundTag::eRun, pCharacter->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);
	pCharacter->SetAnimation(AnimationTag::eRun, 1.4f);
}

void CState_Run::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
}

void CState_Run::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(CharacterParts::LowerBody);

	if (!pCharacter->GetIsRun()) {
		if (pCharacter->IsMoving()) {
			pUpperFSM->ChangeState(CState_Walk::GetInstance());
			pLowerFSM->ChangeState(CState_Walk::GetInstance());
			return;
		}
		else {
			pUpperFSM->ChangeState(CState_Idle::GetInstance());
			pLowerFSM->ChangeState(CState_Idle::GetInstance());
			return;
		}
	}
}

void CState_Run::ExitState(CCharacterObject* pCharacter, CharacterParts type)
{
	if (type == CharacterParts::LowerBody)
		return;

	SOUND_MGR->StopSound();
}

// ---------------------------- Death ---------------------------- //
void CState_Death::EnterState(CCharacterObject* pCharacter, CharacterParts type)
{
	if (type == CharacterParts::LowerBody)
		return;

	// 리스폰 대기 시간 5초
	m_dwDeathWaitingTime = RESPAWN_TIME;
	m_dwDeathStartTime = GetTickCount();

	if (pCharacter->GetIsDeathHead())
		pCharacter->SetAnimation(AnimationTag::eDeath_Head);
	else 
		pCharacter->SetAnimation(AnimationTag::eDeath);

#ifndef USE_SERVER
	m_Position = pCharacter->GetvPosition();
#endif

	SOUND_MGR->Play3DSound(SoundTag::eDeath, pCharacter->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);
}

void CState_Death::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(CharacterParts::LowerBody);
	DWORD timeElapsed = GetTickCount() - m_dwDeathStartTime;

	// Death 애니메이션 위치 Offset 맞추기 용도
	if (pCharacter->GetIsDeathHead()) {
		if (900 < timeElapsed && timeElapsed < 1600) {
			XMVECTOR prevPos = pCharacter->GetvPosition();
			prevPos -= XMVectorSet(0, pCharacter->GetBoundingBox().Extents.y * SCENE_MGR->g_fDeltaTime, 0, 0);
			pCharacter->SetPosition(prevPos);
		}
	}
	else {
		if (1400 < timeElapsed && timeElapsed < 1600) {
			XMVECTOR prevPos = pCharacter->GetvPosition();
			prevPos -= XMVectorSet(0, pCharacter->GetBoundingBox().Extents.y * SCENE_MGR->g_fDeltaTime, 0, 0);
			pCharacter->SetPosition(prevPos);
		}
		if (1600 < timeElapsed && timeElapsed < 2000) {
			XMVECTOR prevPos = pCharacter->GetvPosition();
			prevPos -= XMVectorSet(0, 1.5f * pCharacter->GetBoundingBox().Extents.y * SCENE_MGR->g_fDeltaTime, 0, 0);
			pCharacter->SetPosition(prevPos);
		}
}

#ifdef USE_SERVER
	if (pCharacter->GetAlive()) {
		pUpperFSM->ChangeState(CState_Idle::GetInstance());
		pLowerFSM->ChangeState(CState_Idle::GetInstance());
		pCharacter->SetControllerActive(CharacterParts::UpperBody, true);
		pCharacter->SetControllerActive(CharacterParts::LowerBody, true);

		return;
	}
#else

	if (!pCharacter->GetControllerActive(CharacterParts::UpperBody)) {
		if (m_dwDeathWaitingTime < timeElapsed) {
			pUpperFSM->ChangeState(CState_Idle::GetInstance());
			pLowerFSM->ChangeState(CState_Idle::GetInstance());
			pCharacter->SetControllerActive(CharacterParts::UpperBody, true);
			pCharacter->SetControllerActive(CharacterParts::LowerBody, true);

			return;
		}
	}
#endif
}

void CState_Death::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_Death::ExitState(CCharacterObject* pCharacter, CharacterParts type)
{
	if (type == CharacterParts::LowerBody)
		return;

	pCharacter->Revival();
#ifndef USE_SERVER
	pCharacter->SetPosition(m_Position);
#endif
}

// ---------------------------- Head Hit ---------------------------- //
void CState_HeadHit::EnterState(CCharacterObject* pCharacter, CharacterParts type)
{
	pCharacter->SetAnimation(AnimationTag::eHeadHit);
	pCharacter->SetIsDeadlyAttack(true);

	m_dwHeadHitStartTime = GetTickCount();

	if (pCharacter->GetCharacterID() == 0) {
		SOUND_MGR->Play2DSound(SoundTag::eBeep);
		SOUND_MGR->AddVolume(-0.9f);
	}
}

void CState_HeadHit::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(CharacterParts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(CharacterParts::LowerBody);

	if (pCharacter->GetIsDeath()) {
		pUpperFSM->ChangeState(CState_Death::GetInstance());
		pLowerFSM->ChangeState(CState_Death::GetInstance());
		return;
	}
	else if (!pCharacter->GetControllerActive(CharacterParts::UpperBody)) {
		pUpperFSM->ChangeState(pUpperFSM->GetPreviousState());
		pCharacter->SetControllerActive(CharacterParts::UpperBody, true);
		return;
	}

	if (pCharacter->GetCharacterID() == 0) {
		DWORD timeElapsed = GetTickCount() - m_dwHeadHitStartTime;
		if (300 < timeElapsed)
			SOUND_MGR->AddVolume(0.002f);
	}
}

void CState_HeadHit::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_HeadHit::ExitState(CCharacterObject* pCharacter, CharacterParts type)
{
	pCharacter->SetIsHeadHit(false);
}
