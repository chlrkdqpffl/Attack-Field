#pragma once
#include "FSM_State.h"
#include "CharacterState.h"

template <typename ObjectType>
class CStateMachine
{
public:
	CStateMachine(ObjectType* owner, CharacterParts type)
		: m_pOwner(owner), m_partsBody(type)
	{
		m_pCurrentState = CState_Idle::GetInstance();
		m_pCurrentState->EnterState(m_pOwner, m_partsBody);
		m_pPreviousState = m_pCurrentState;

		m_pAnyState = CState_AnyTime::GetInstance();
	}
	virtual ~CStateMachine() {};

	void ChangeState(CFSM_State<ObjectType>* newState)
	{
		if (newState == m_pCurrentState)
			return;

		m_pPreviousState = m_pCurrentState;
		m_pCurrentState->ExitState(m_pOwner, m_partsBody);

		m_pCurrentState = newState;
		m_pCurrentState->EnterState(m_pOwner, m_partsBody);
	}

	virtual void Update()
	{
		if (m_partsBody == CharacterParts::UpperBody) {
			m_pAnyState->UpdateUpperBodyState(m_pOwner);
			m_pCurrentState->UpdateUpperBodyState(m_pOwner);
		}
		else
			m_pCurrentState->UpdateLowerBodyState(m_pOwner);
	}

	// ----- Get, Setter ----- //
	CFSM_State<ObjectType>* GetPreviousState() const { return m_pPreviousState; }
	CFSM_State<ObjectType>* GetCurrentState() const { return m_pCurrentState; }

protected:
	ObjectType*					m_pOwner = nullptr;
	CharacterParts		m_partsBody = CharacterParts::Defalut;

	CFSM_State<ObjectType>*		m_pAnyState = nullptr;
	CFSM_State<ObjectType>*		m_pPreviousState = nullptr;
	CFSM_State<ObjectType>*		m_pCurrentState = nullptr;
};