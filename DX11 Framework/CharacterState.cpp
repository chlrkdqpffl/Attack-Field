#include "stdafx.h"
#include "CharacterState.h"

// ---------------------------- Idle ---------------------------- //
void CState_Idle::EnterState(CCharacterObject* pCharacter)
{
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eIdle);
	
}

void CState_Idle::UpdateState(CCharacterObject* pCharacter)
{
	if(pCharacter->IsMoving())
		pCharacter->GetFSM()->ChangeState(CState_Walking::GetInstance());
}

void CState_Idle::ExitState(CCharacterObject* pCharacter)
{

}

// ---------------------------- Walking ---------------------------- //
void CState_Walking::EnterState(CCharacterObject* pCharacter)
{
	pCharacter->SetAnimation(AnimationData::CharacterAnim::eForwardWalk);
}

void CState_Walking::UpdateState(CCharacterObject* pCharacter)
{ 
	AnimationData::CharacterAnim characterAnim = pCharacter->GetAnimation();

	XMFLOAT3 relativeVelocity = pCharacter->GetRelativeVelocity();		// 추후 방향만 있는 것이 아닌 속력까지 갖도록 설정하기. 현재는 방향만 존재

	// Forward
	if (relativeVelocity.z > 0) {
		if (relativeVelocity.x < 0) {		// Left
			if (m_CurrentAnim != characterAnim)
				pCharacter->SetAnimation(AnimationData::CharacterAnim::eForwardWalk);
		
		}
		else if (relativeVelocity.x > 0) {	// Right
			if (m_CurrentAnim != characterAnim)
				pCharacter->SetAnimation(AnimationData::CharacterAnim::eForwardWalk);
	
		}
		else {
			if (m_CurrentAnim != characterAnim)
				pCharacter->SetAnimation(AnimationData::CharacterAnim::eForwardWalk);
		
		}
	}
	// Backward
	else if (relativeVelocity.z < 0) {
		if (relativeVelocity.x < 0) {		// Left
			if (m_CurrentAnim != characterAnim)
				pCharacter->SetAnimation(AnimationData::CharacterAnim::eForwardWalk);
		
		}
		else if (relativeVelocity.x > 0) {	// Right
			if (m_CurrentAnim != characterAnim)
				pCharacter->SetAnimation(AnimationData::CharacterAnim::eForwardWalk);
		
		}
		else {
			if (m_CurrentAnim != characterAnim)
				pCharacter->SetAnimation(AnimationData::CharacterAnim::eBackwardWalk);
		
		}
	}
	else {
		if (relativeVelocity.x < 0) {		// Left
			if (m_CurrentAnim != characterAnim)
				pCharacter->SetAnimation(AnimationData::CharacterAnim::eForwardWalk);
		}
		else if (relativeVelocity.x > 0) {	// Right
			if (m_CurrentAnim != characterAnim)
				pCharacter->SetAnimation(AnimationData::CharacterAnim::eForwardWalk);

		}
	}

}

void CState_Walking::ExitState(CCharacterObject* pCharacter)
{
}

/*
// ---------------------------- Walking ---------------------------- //
CState_Walking::CState_Walking()
{
}

CState_Walking::~CState_Walking()
{
}

void CState_Walking::EnterState(CCharacterObject* pCharacter)
{
}

void CState_Walking::UpdateState(CCharacterObject* pCharacter)
{

}

void CState_Walking::ExitState(CCharacterObject* pCharacter)
{
}
*/