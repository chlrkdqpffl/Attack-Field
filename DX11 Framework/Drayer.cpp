#include "stdafx.h"
#include "Drayer.h"

CDrayer::CDrayer()
{
	m_pAnimController->AddAnimation(make_tuple(Animation::eIdle,	"idle", AnimationType::Loop));
	m_pAnimController->AddAnimation(make_tuple(Animation::eRun,		"Run", AnimationType::Loop));
	m_pAnimController->AddAnimation(make_tuple(Animation::eAttack,	"CrossPunch", AnimationType::PingPong));
}

CDrayer::~CDrayer()
{
}
