#include "stdafx.h"
#include "Drayer.h"

CDrayer::CDrayer()
{
	m_animaitionListVector.push_back(make_pair(Animation::eIdle, "idle"));
	m_animaitionListVector.push_back(make_pair(Animation::eRun, "Run"));
	m_animaitionListVector.push_back(make_pair(Animation::eAttack, "CrossPunch"));

	m_animaitionTupleListVector.push_back(make_tuple(Animation::eIdle, "idle", AnimationType::Loop));
	m_animaitionTupleListVector.push_back(make_tuple(Animation::eRun, "Run", AnimationType::Loop));
	m_animaitionTupleListVector.push_back(make_tuple(Animation::eAttack, "CrossPunch", AnimationType::Once));
}

CDrayer::~CDrayer()
{
}
