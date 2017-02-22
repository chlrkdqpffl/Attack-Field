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

void CAnimationController::SetAnimation(Animation::Character anim, float speed)
{
	if (get<0>(m_currAnimState) == anim)
		return;

	for (auto& animState : m_animaitionTupleVector)
		if (get<0>(animState) == anim) {
			m_prevAnimState = m_currAnimState;
			m_currAnimState = animState;

			m_fTimePos = 0.0f;
			get<1>(m_currAnimState).m_bEnable = true;
			get<1>(m_currAnimState).m_fSpeed = speed;
			m_pSkinnedMesh->SetClipName(get<1>(m_currAnimState).m_strClipName);
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
	float timeElapse = get<1>(m_currAnimState).m_fSpeed * fTimeElapsed;

	switch (GetAnimType()) {
	case AnimationType::Loop:
		m_fTimePos += timeElapse;

		if (m_fTimePos > endTime)
			m_fTimePos = 0.0f;

		break;
	case AnimationType::Once:
		m_fTimePos += timeElapse;

		if (m_fTimePos  > endTime) {
			SetAnimation(Animation::eIdle);
			m_fTimePos = 0.0f;
		}

		break;
	case AnimationType::PingPong:
		static bool isReverse = false;

		if (isReverse == false) {
			m_fTimePos += timeElapse;

			if (endTime < m_fTimePos) {
				isReverse = true;
			}
		}
		else if (isReverse == true) {
			m_fTimePos -= timeElapse;

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
	m_pSkinnedMesh->GetFinalTransformsBlending(get<1>(m_prevAnimState), get<1>(m_currAnimState), m_fTimePos);
}

void CAnimationController::UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_pSkinnedMesh->UpdateConstantBuffer(pd3dDeviceContext);
}