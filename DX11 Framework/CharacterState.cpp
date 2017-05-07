#include "stdafx.h"
#include "CharacterState.h"

void CState_AnyTime::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	if (pCharacter->GetIsDeath()) {
		pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Death::GetInstance());
		pCharacter->GetFSM(AnimationData::Parts::LowerBody)->ChangeState(CState_Death::GetInstance());
	}
}

// ---------------------------- Idle ---------------------------- //
void CState_Idle::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->SetAnimation(type, AnimationData::CharacterAnim::eIdle);
}

void CState_Idle::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
//	cout << "Idle" << endl;
	if (pCharacter->GetIsReload())
		pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Reload::GetInstance());
	else if(pCharacter->GetIsFire())
		pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Fire::GetInstance());

	// 상체가 Idle 상태일 경우 하체의 움직임을 따라간다.
	AnimationData::CharacterAnim lowerAnim = pCharacter->GetAnimationEnum(AnimationData::Parts::LowerBody);
	if(lowerAnim != AnimationData::CharacterAnim::eIdle)
		pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(pCharacter->GetFSM(AnimationData::Parts::LowerBody)->GetCurrentState());

}

void CState_Idle::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
//	cout << "Idle" << endl;
	if (pCharacter->IsMoving())
		pCharacter->GetFSM(AnimationData::Parts::LowerBody)->ChangeState(CState_Walk::GetInstance());
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
	// Check Reload 
	if (pCharacter->GetIsReload())
		pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Reload::GetInstance());

	AnimationData::CharacterAnim lowerAnim = pCharacter->GetAnimationEnum(AnimationData::Parts::LowerBody);
	if (lowerAnim == AnimationData::CharacterAnim::eIdle)
		pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(pCharacter->GetFSM(AnimationData::Parts::LowerBody)->GetCurrentState());
}

void CState_Walk::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
	// Check Run
	if (pCharacter->GetIsRun()) {
		pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Run::GetInstance());
		pCharacter->GetFSM(AnimationData::Parts::LowerBody)->ChangeState(CState_Run::GetInstance());
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
		pCharacter->GetFSM(AnimationData::Parts::LowerBody)->ChangeState(CState_Idle::GetInstance());

}

void CState_Walk::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
}

// ---------------------------- Reload ---------------------------- //
void CState_Reload::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eReload);
}

void CState_Reload::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	if(!pCharacter->GetControllerActive(AnimationData::Parts::UpperBody)) {
		pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Idle::GetInstance());
		pCharacter->SetControllerActive(AnimationData::Parts::UpperBody, true);
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
	if (pCharacter->GetIsReload())
		pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Reload::GetInstance());
	else if (!pCharacter->GetIsFire())
		pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Idle::GetInstance());

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
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eRun);
}

void CState_Run::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
}

void CState_Run::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
	if (!pCharacter->GetIsRun()) {
		if (pCharacter->IsMoving()) {
			pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Walk::GetInstance());
			pCharacter->GetFSM(AnimationData::Parts::LowerBody)->ChangeState(CState_Walk::GetInstance());
		}
		else {
			pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Idle::GetInstance());
			pCharacter->GetFSM(AnimationData::Parts::LowerBody)->ChangeState(CState_Idle::GetInstance());
		}
	}
}

void CState_Run::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
}

// ---------------------------- Run ---------------------------- //
void CState_Death::EnterState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	// 리스폰 대기 시간 4초
	m_dwDeathWaitingTime = 4000;			
	m_dwDeathStartTime = GetTickCount();

	// 맞은 위치 확인하여 죽는 모션 다르게 하기
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eDeath_Head);
}

void CState_Death::UpdateUpperBodyState(CCharacterObject* pCharacter)
{
	if (!pCharacter->GetControllerActive(AnimationData::Parts::UpperBody)) {
		if (GetTickCount() - m_dwDeathStartTime > m_dwDeathWaitingTime) {
			pCharacter->GetFSM(AnimationData::Parts::UpperBody)->ChangeState(CState_Idle::GetInstance());
			pCharacter->GetFSM(AnimationData::Parts::LowerBody)->ChangeState(CState_Idle::GetInstance());
			pCharacter->SetControllerActive(AnimationData::Parts::UpperBody, true);
			pCharacter->SetControllerActive(AnimationData::Parts::LowerBody, true);
		}
	}
}

void CState_Death::UpdateLowerBodyState(CCharacterObject* pCharacter)
{
}

void CState_Death::ExitState(CCharacterObject* pCharacter, AnimationData::Parts type)
{
	pCharacter->Revival();
}