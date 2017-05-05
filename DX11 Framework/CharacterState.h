#pragma once
#include "FSM_State.h"

class CState_Idle : public CFSM_State<CCharacterObject>
{
public:
	CState_Idle() {};
	virtual ~CState_Idle() {};

	virtual void EnterState(CCharacterObject*) override;
	virtual void UpdateState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*) override;

	static CState_Idle * GetInstance()
	{
		static CState_Idle instance;

		return &instance;
	}
};

class CState_Walking : public CFSM_State<CCharacterObject>
{
	AnimationData::CharacterAnim		m_CurrentAnim;

public:
	CState_Walking() {};
	virtual ~CState_Walking() {};

	virtual void EnterState(CCharacterObject*) override;
	virtual void UpdateState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*) override;

	static CState_Walking * GetInstance()
	{
		static CState_Walking instance;

		return &instance;
	}
};
