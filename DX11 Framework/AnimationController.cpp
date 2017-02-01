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

	AnimationTrack track;
	for (auto anim : m_pSkinnedMesh->GetAnimMap()) {
		track.m_strClipName = anim.first;
		m_animTrackVector.push_back(track);
	}
}

void CAnimationController::SetAnimation(Animation::Character anim)
{
	for (auto list : m_animaitionTupleVector)
		if (get<0>(list) == anim) {
			m_animState = get<0>(list);
			m_pSkinnedMesh->SetClipName(get<1>(list));
			m_animType = get<2>(list);
			m_fTimePos = 0.0f;
			return;
		}
}

void CAnimationController::AddAnimation(tuple<Animation::Character, string, AnimationType::Type> anim)
{
	m_animaitionTupleVector.push_back(anim);
}

void  CAnimationController::UpdateTime(float fTimeElapsed)
{
	float endTime = m_pSkinnedMesh->GetClipEndTime(m_pSkinnedMesh->GetClipName());

	switch (m_animType) {
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
	//m_pSkinnedMesh->GetFinalTransforms_BlendingTest(fTimePos);
	m_pSkinnedMesh->GetFinalTransforms(m_pSkinnedMesh->GetClipName(), m_fTimePos);
}

void CAnimationController::UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_pSkinnedMesh->UpdateConstantBuffer(pd3dDeviceContext);
}