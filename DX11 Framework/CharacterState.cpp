#include "stdafx.h"
#include "CharacterState.h"

void CState_AnyTime::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);

	// 죽은 상태일 때에는 상태 확인 X
	if (pUpperFSM->GetCurrentState() == CState_Death::GetInstance())
		return;

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

void CState_Idle::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->SetAnimation(type, AnimationData::CharacterAnim::eIdle);
}

void CState_Idle::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);

	// Check Reload
	if (pCharacter->GetIsReload()) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}

	// Check Fire
	if (pCharacter->GetIsFire()) {
		pUpperFSM->ChangeState(CState_Fire::GetInstance());
		return;
	}

	// 상체가 Idle 상태일 경우 하체의 움직임을 따라간다.
	AnimationData::CharacterAnim lowerAnim = pCharacter->GetAnimationEnum(AnimationData::Parts::LowerBody);
	if (lowerAnim != AnimationData::CharacterAnim::eIdle)
		pUpperFSM->ChangeState(pLowerFSM->GetCurrentState());
}

void CState_Idle::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);

	if (pCharacter->IsMoving()) {
		pLowerFSM->ChangeState(CState_Walk::GetInstance());
		return;
	}

	if (pCharacter->GetIsCrouch()) {
		pLowerFSM->ChangeState(CState_Crouch::GetInstance());
		return;
	}
}

void CState_Idle::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
}

// ---------------------------- Walking ---------------------------- //
void CState_Walk::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	if(type == AnimationData::Parts::UpperBody)
		pCharacter->SetAnimation(AnimationData::Parts::UpperBody, AnimationData::CharacterAnim::eWalk_Forward);
}

void CState_Walk::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);
	
	// Check Reload 
	if (pCharacter->GetIsReload()) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}
	// Check Fire
	if (pCharacter->GetIsFire()) {
		pUpperFSM->ChangeState(CState_Fire::GetInstance());
		return;
	}

	AnimationData::CharacterAnim lowerAnim = pCharacter->GetAnimationEnum(AnimationData::Parts::LowerBody);
	if (lowerAnim == AnimationData::CharacterAnim::eIdle)
		pUpperFSM->ChangeState(pLowerFSM->GetCurrentState());
}

void CState_Walk::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);
	
	// Check Run
	if (pCharacter->GetIsRun()) {
		if (!pCharacter->GetIsReload()) {
			pUpperFSM->ChangeState(CState_Run::GetInstance());
			pLowerFSM->ChangeState(CState_Run::GetInstance());

			return;
		}
	}

	// Check Walk Direction 
	AnimationData::CharacterAnim characterAnim = pCharacter->GetAnimationEnum(AnimationData::Parts::LowerBody);

	XMFLOAT3 relativeVelocity = pCharacter->GetRelativeVelocity();		// 추후 방향만 있는 것이 아닌 속력까지 갖도록 설정하기. 현재는 방향만 존재

	// Forward
	if (relativeVelocity.z > 0) {
		if (relativeVelocity.x < 0) {		// Left
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_ForwardLeft, 1.3f);
		}
		else if (relativeVelocity.x > 0) {	// Right
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_ForwardRight, 1.3f);
		}
		else {
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_Forward, 1.5f);
		}
	}
	// Backward
	else if (relativeVelocity.z < 0) {
		if (relativeVelocity.x < 0) {		// Left
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_BackwardLeft, 1.3f);
		}
		else if (relativeVelocity.x > 0) {	// Right
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_BackwardRight, 1.3f);
		}
		else {
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_Backward, 1.5f);
		}
	}
	else {
		if (relativeVelocity.x < 0) {		// Left
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_Left, 1.3f);
		}
		else if (relativeVelocity.x > 0) {	// Right
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_Right, 1.3f);
		}
	}

	if (!pCharacter->IsMoving())
		pLowerFSM->ChangeState(CState_Idle::GetInstance());
}

void CState_Walk::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	SOUND_MGR->StopSound();
}

// ---------------------------- Crouch ---------------------------- //
void CState_Crouch::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	if (type == AnimationData::Parts::LowerBody)
		pCharacter->SetAnimation(AnimationData::Parts::LowerBody, AnimationData::CharacterAnim::eCrouch);
}

void CState_Crouch::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);

	// Check Reload
	if (pCharacter->GetIsReload()) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}

	// Check Fire
	if (pCharacter->GetIsFire()) {
		pUpperFSM->ChangeState(CState_Fire::GetInstance());
		return;
	}

	if (false == pCharacter->GetIsCrouch()) {
		pUpperFSM->ChangeState(CState_Idle::GetInstance());
		return;
	}
}

void CState_Crouch::UpdateLowerBodyState(CCharacterObject* pCharacter)
{	
	if (false == pCharacter->GetIsCrouch()) {
		CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);
		pLowerFSM->ChangeState(CState_Idle::GetInstance());
		return;
	}
}

void CState_Crouch::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
}

// ---------------------------- Reload ---------------------------- //
void CState_Reload::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eReload, 1.6f);
	pCharacter->GetWeapon()->SetFireBulletCount(0);
	SOUND_MGR->Play3DSound(SoundTag::eReload, pCharacter->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);
}

void CState_Reload::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	if(!pCharacter->GetControllerActive(AnimationData::Parts::UpperBody)) {
		pUpperFSM->ChangeState(CState_Idle::GetInstance());
		pCharacter->SetControllerActive(AnimationData::Parts::UpperBody, true);
		return;
	}
}

void CState_Reload::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_Reload::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->Reloading();
	pCharacter->SetIsReload(false);
}

// ---------------------------- Fire ---------------------------- //
void CState_Fire::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eFire);
}

void CState_Fire::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);

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

void CState_Fire::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_Fire::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	SPRITE_MGR->DisableSprite(pCharacter->GetMuzzleSprite());
}

// ---------------------------- Run ---------------------------- //
void CState_Run::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	if (type == AnimationData::Parts::LowerBody)
		return;

	SOUND_MGR->Play3DSound(SoundTag::eRun, pCharacter->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eRun, 1.5f);
	pCharacter->SetIsTempRun(true);
}

void CState_Run::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
}

void CState_Run::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);

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

void CState_Run::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	if (type == AnimationData::Parts::LowerBody)
		return;

	pCharacter->SetIsTempRun(false);
	SOUND_MGR->StopSound();
}

// ---------------------------- Death ---------------------------- //
void CState_Death::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	if (type == AnimationData::Parts::LowerBody)
		return;

	// 리스폰 대기 시간 5초
	m_dwDeathWaitingTime = RESPAWN_TIME;
	m_dwDeathStartTime = GetTickCount();

	if (pCharacter->GetIsDeathHead())
		pCharacter->SetAnimation(AnimationData::CharacterAnim::eDeath_Head);
	else 
		pCharacter->SetAnimation(AnimationData::CharacterAnim::eDeath);

#ifndef USE_SERVER
	m_Position = pCharacter->GetvPosition();
#endif

	SOUND_MGR->Play3DSound(SoundTag::eDeath, pCharacter->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);
}

void CState_Death::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);
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
		pCharacter->SetControllerActive(AnimationData::Parts::UpperBody, true);
		pCharacter->SetControllerActive(AnimationData::Parts::LowerBody, true);

		return;
	}
#else
	if (!pCharacter->GetControllerActive(AnimationData::Parts::UpperBody)) {
		if (m_dwDeathWaitingTime < timeElapsed) {
			pUpperFSM->ChangeState(CState_Idle::GetInstance());
			pLowerFSM->ChangeState(CState_Idle::GetInstance());
			pCharacter->SetControllerActive(AnimationData::Parts::UpperBody, true);
			pCharacter->SetControllerActive(AnimationData::Parts::LowerBody, true);

			return;
		}
	}
#endif
}

void CState_Death::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_Death::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	if (type == AnimationData::Parts::LowerBody)
		return;

	pCharacter->SetIsDeadlyAttack(false);
	pCharacter->SetIsDeadly(false);
	pCharacter->Revival();
#ifndef USE_SERVER
	pCharacter->SetPosition(m_Position);
#endif
}

// ---------------------------- Head Hit ---------------------------- //
void CState_HeadHit::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eHeadHit);
	pCharacter->SetIsDeadlyAttack(true);
}

void CState_HeadHit::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);

	if (!pCharacter->GetControllerActive(AnimationData::Parts::UpperBody)) {
		pUpperFSM->ChangeState(pUpperFSM->GetPreviousState());
		pCharacter->SetControllerActive(AnimationData::Parts::UpperBody, true);
		return;
	}
}

void CState_HeadHit::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_HeadHit::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->SetIsHeadHit(false);
}
