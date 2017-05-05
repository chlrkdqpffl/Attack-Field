#pragma once
#include "FSM_State.h"
#include "CharacterState.h"

template <typename ObjectType>
class CStateMachine
{
public:
	CStateMachine(ObjectType* owner)
	{
		m_pOwner = owner;

		m_pCurrentState = CState_Idle::GetInstance();
		m_pCurrentState->EnterState(m_pOwner);
	}
	virtual ~CStateMachine() {};

	void ChangeState(CFSM_State<ObjectType>* newState)
	{
		if (newState == m_pCurrentState)
			return;

		m_pPreviousState = m_pCurrentState;
		m_pCurrentState->ExitState(m_pOwner);

		m_pCurrentState = newState;
		m_pCurrentState->EnterState(m_pOwner);
	}
	virtual void Update()
	{
		m_pCurrentState->UpdateState(m_pOwner);
	}

protected:
	ObjectType*					m_pOwner = nullptr;
	CFSM_State<ObjectType>*		m_pPreviousState = nullptr;
	CFSM_State<ObjectType>*		m_pCurrentState = nullptr;

};