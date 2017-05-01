#pragma once
#include "FSM_State.h"

class CState_Idle :	public CFSM_State<CCharacterObject>
{
public:
	CState_Idle();
	virtual ~CState_Idle();

	virtual void EnterState(CCharacterObject*) override;
	virtual void UpdateState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*) override;
};

