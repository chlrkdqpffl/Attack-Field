#include "stdafx.h"
#include "CharacterState.h"

void CState_AnyTime::UpdateUpperBodyState(CCharacterObject* pCharacter)	//살았는지 확인 or 헤드샷 확인.
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);

	if (pCharacter->GetIsDeath()) {
		pUpperFSM->ChangeState(CState_Death::GetInstance());
		pLowerFSM->ChangeState(CState_Death::GetInstance());
		return;
	}
	//else if (pCharacter->GetCollisionParts() == ChracterBoundingBoxParts::eHead) {
	else if(pCharacter->GetIsHeadHit()) {
		pUpperFSM->ChangeState(CState_HeadHit::GetInstance());
		return;
	}
}

// ---------------------------- Idle ---------------------------- //
void CState_Idle::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->SetAnimation(type, AnimationData::CharacterAnim::eIdle);
}

void CState_Idle::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	CStateMachine<CCharacterObject>* pUpperFSM = pCharacter->GetFSM(AnimationData::Parts::UpperBody);
	CStateMachine<CCharacterObject>* pLowerFSM = pCharacter->GetFSM(AnimationData::Parts::LowerBody);

	// Check Reload
#ifdef USE_SERVER
	if (pCharacter->GetIsReload()) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}
#else
	if (pCharacter->GetIsReload()) {
		if (pCharacter->GetWeaponBulletCount() != pCharacter->GetWeaponMaxBulletCount()) {
			pUpperFSM->ChangeState(CState_Reload::GetInstance());
			return;
		}
	}
#endif
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
#ifdef USE_SERVER
	if (pCharacter->GetIsReload()) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}
#else
	if (pCharacter->GetIsReload()) {
		if (pCharacter->GetWeaponBulletCount() != pCharacter->GetWeaponMaxBulletCount()) {
			pUpperFSM->ChangeState(CState_Reload::GetInstance());
			return;
		}
	}
#endif

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
	
	//cout << "하체 : walk" << endl;
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
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_ForwardLeft);
		}
		else if (relativeVelocity.x > 0) {	// Right
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_ForwardRight);
		}
		else {
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_Forward);
		}
	}
	// Backward
	else if (relativeVelocity.z < 0) {
		if (relativeVelocity.x < 0) {		// Left
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_BackwardLeft);
		}
		else if (relativeVelocity.x > 0) {	// Right
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_BackwardRight);
		}
		else {
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_Backward);
		}
	}
	else {
		if (relativeVelocity.x < 0) {		// Left
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_Left);
		}
		else if (relativeVelocity.x > 0) {	// Right
			pCharacter->SetAnimation(AnimationData::CharacterAnim::eWalk_Right);
		}
	}

	if (!pCharacter->IsMoving())
		pLowerFSM->ChangeState(CState_Idle::GetInstance());
}

void CState_Walk::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	SOUND_MGR->StopSound(SoundChannel::eChannel_Walk);
}

// ---------------------------- Reload ---------------------------- //
void CState_Reload::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eReload, 1.6f);
	SOUND_MGR->Play3DSound(SoundTag::eReload, pCharacter->GetPosition(), XMFLOAT3(0, 0, 0), 1, 1);
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
#ifdef USE_SERVER
	if (pCharacter->GetIsReload()) {
		pUpperFSM->ChangeState(CState_Reload::GetInstance());
		return;
	}
#else
	if (pCharacter->GetIsReload()) {
		if (pCharacter->GetWeaponBulletCount() != pCharacter->GetWeaponMaxBulletCount()) {
			pUpperFSM->ChangeState(CState_Reload::GetInstance());
			return;
		}
	}
#endif
	
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
}

// ---------------------------- Run ---------------------------- //
void CState_Run::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	if (type == AnimationData::Parts::LowerBody)
		return;

	SOUND_MGR->Play3DSound(SoundTag::eRun, SoundChannel::eChannel_Walk, pCharacter->GetPosition(), XMFLOAT3(0, 0, 0), 1, 0.7f);
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eRun);
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
	SOUND_MGR->StopSound(SoundChannel::eChannel_Walk);
}

// ---------------------------- Death ---------------------------- //
void CState_Death::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	if (type == AnimationData::Parts::LowerBody)
		return;

	// 리스폰 대기 시간 5초
	m_dwDeathWaitingTime = 5000;
	m_dwDeathStartTime = GetTickCount();

	if (pCharacter->GetIsDeathHead())
		pCharacter->SetAnimation(AnimationData::CharacterAnim::eDeath_Head);
	else 
		pCharacter->SetAnimation(AnimationData::CharacterAnim::eDeath);

#ifndef USE_SERVER
	m_Position = pCharacter->GetvPosition();
#endif

	SOUND_MGR->Play3DSound(SoundTag::eDeath, pCharacter->GetPosition(), XMFLOAT3(0, 0, 0), 1, 1);
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
	if (pCharacter->GetIsRespawn()) {
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

	pCharacter->SetIsDeath(false);
#ifndef USE_SERVER
	pCharacter->Revival(100);
	pCharacter->SetPosition(m_Position);
#endif
}

// ---------------------------- Head Hit ---------------------------- //
void CState_HeadHit::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eHeadHit);
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
