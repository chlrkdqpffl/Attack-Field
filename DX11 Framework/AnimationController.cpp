#include "stdafx.h"
#include "AnimationController.h"


CAnimationController::CAnimationController()
{
}

CAnimationController::~CAnimationController()
{
}

void CAnimationController::SetMesh(CSkinnedMesh* mesh)
{
	m_pSkinnedMesh = mesh; 
}

void CAnimationController::SetAnimation(Animation::Character anim)
{
	for (auto& animState : m_animaitionTupleVector)
		if (get<0>(animState) == anim) {
			m_prevAnimState = m_currAnimState;
			m_currAnimState = animState;
			m_fTimePos = 0.0f;

			m_pSkinnedMesh->SetClipName(get<1>(animState).m_strClipName);
			return;
		}
}

void CAnimationController::AddAnimation(tuple<Animation::Character, AnimationTrack, AnimationType::Type> anim)
{
	m_animaitionTupleVector.push_back(anim);
	
	auto animMap = m_pSkinnedMesh->GetAnimMap();
	auto iter = animMap.find(get<1>(anim).m_strClipName);

	if (iter == animMap.end())
		ShowTaskFail("Animation Mapping Error!! < " + get<1>(anim).m_strClipName + " > \t 애니메이션 이름 확인이 필요!");
}

void CAnimationController::UpdateTime(float fTimeElapsed)
{
	float endTime = m_pSkinnedMesh->GetClipEndTime(m_pSkinnedMesh->GetClipName());

	switch (GetAnimType()) {
	case AnimationType::Loop:
		m_fTimePos += fTimeElapsed;

		if (m_fTimePos > endTime)
			m_fTimePos = 0.0f;
		break;

	case AnimationType::Once:
		m_fTimePos += fTimeElapsed;

		if (m_fTimePos  > endTime) {
			m_fTimePos = 0.0f;
			SetAnimation(Animation::eIdle);
		}

	case AnimationType::PingPong:
		static bool isReverse = false;

		if (isReverse == false) {
			m_fTimePos += fTimeElapsed;

			if (endTime < m_fTimePos) {
				isReverse = true;
			}
		}
		else if (isReverse == true) {
			m_fTimePos -= fTimeElapsed;

			if (m_fTimePos  < 0.0f) {
				isReverse = false;
				m_fTimePos = 0.0f;
				SetAnimation(Animation::eIdle);
			}
		}
		break;
	}
}

void CAnimationController::Update(float fTimeElapsed)
{
	UpdateTime(fTimeElapsed);
//	m_pSkinnedMesh->GetFinalTransforms(m_pSkinnedMesh->GetClipName(), m_fTimePos); 
	m_pSkinnedMesh->GetFinalTransformsBlending(get<1>(m_prevAnimState).m_strClipName, get<1>(m_currAnimState).m_strClipName, m_fTimePos);
}

void CAnimationController::UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_pSkinnedMesh->UpdateConstantBuffer(pd3dDeviceContext);
}