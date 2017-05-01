#pragma once
#include "FSM_State.h"

template <typename ObjectType>
class CStateMachine
{
public:
	CStateMachine() {};
	virtual ~CStateMachine() {};

	virtual void ChangeState(CFSM_State<ObjectType>*) = 0;
	virtual void Update() = 0;

private:
	CFSM_State<ObjectType>*		m_pPrevState = nullptr;
	CFSM_State<ObjectType>*		m_pNowState = nullptr;

};