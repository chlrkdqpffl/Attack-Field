#include "stdafx.h"
#include "AnimationController.h"


CAnimationController::CAnimationController(CharacterParts type)
	:m_typeParts(type)
{
}

CAnimationController::~CAnimationController()
{
}

void CAnimationController::SetAnimation(AnimationTag anim, float speed)
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

	cout << "No Animation Data!! -  " << static_cast<int>(anim) << endl;
}

void CAnimationController::AddAnimation(tuple<AnimationTag, AnimationTrack, AnimationType> anim)
{
	m_animaitionTupleVector.push_back(anim);
	
	auto animMap = m_pSkinnedMesh->GetAnimMap();
	auto iter = animMap.find(get<1>(anim).m_strClipName);

	if (iter == animMap.end())
		ShowTaskFail("Animation Mapping Error!! < " + get<1>(anim).m_strClipName + " > \t 애니메이션 이름 확인이 필요!");
}

void CAnimationController::UpdateTime(float fDeltaTime)
{
	float endTime = m_pSkinnedMesh->GetClipEndTime(get<1>(m_currAnimState).m_strClipName);
	float timeElapse = get<1>(m_currAnimState).m_fSpeed * fDeltaTime * TWBAR_MGR->g_fAnimationSpeed;

	switch (GetAnimType()) {
	case AnimationType::eLoop:
		m_fTimePos += timeElapse;

		if (m_fTimePos > endTime)
			m_fTimePos = 0.0f;
		break;
	case AnimationType::eInverseLoop:
		m_fTimePos -= timeElapse;

		if (m_fTimePos < 0.0f)
			m_fTimePos = endTime;
		break;
	case AnimationType::eOnce:
		m_fTimePos += timeElapse;
		if (m_fTimePos > endTime)
			//SetAnimation(AnimationTag::eIdle);
			m_isActive = false;
		break;
	case AnimationType::ePingPong:
		static bool isReverse = false;

		if (isReverse == false) {
			m_fTimePos += timeElapse;

			if (endTime < m_fTimePos) {
				isReverse = true;
			}
		}
		else if (isReverse == true) {
			m_fTimePos -= timeElapse;

			if (m_fTimePos < 0.0f) {
				isReverse = false;
				SetAnimation(AnimationTag::eIdle);
				m_isActive = false;
			}
		}
		break;
	}
}

void CAnimationController::Update(float fDeltaTime)
{
	if (m_isActive) {
		UpdateTime(fDeltaTime);
		m_pSkinnedMesh->CalcFinalTransformsBlending(get<1>(m_prevAnimState), get<1>(m_currAnimState), m_fTimePos, m_typeParts);
	}
}

void CAnimationController::UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_pSkinnedMesh->UpdateConstantBuffer(pd3dDeviceContext);
}