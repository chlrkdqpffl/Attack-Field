#pragma once
#include "StateMachine.h"

class CCharacterStateMachine :	public CStateMachine<CCharacterObject>
{
public:
	CCharacterStateMachine();
	virtual ~CCharacterStateMachine();

	virtual void ChangeState(CFSM_State<CCharacterObject>* newState) override;
	virtual void Update() override;

};