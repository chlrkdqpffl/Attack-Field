#include "stdafx.h"
#include "Drayer.h"

CDrayer::CDrayer()
{
//	m_animaitionTupleVector.push_back(make_tuple(Animation::eIdle, "Take_001", AnimationType::Loop));
	m_animaitionTupleVector.push_back(make_tuple(Animation::eIdle,		"idle", AnimationType::Loop));
	m_animaitionTupleVector.push_back(make_tuple(Animation::eRun,		"Run", AnimationType::Loop));
	m_animaitionTupleVector.push_back(make_tuple(Animation::eAttack,	"CrossPunch", AnimationType::Once));
}

CDrayer::~CDrayer()
{
}
