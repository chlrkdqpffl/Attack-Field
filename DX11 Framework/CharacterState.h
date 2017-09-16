#pragma once
#include "FSM_State.h"

class CState_AnyTime : public CFSM_State<CCharacterObject>
{
public:
	CState_AnyTime() {};
	virtual ~CState_AnyTime() {};

	virtual void EnterState(CCharacterObject*, AnimationData::Parts) override {};
	virtual void UpdateUpperBodyState(CCharacterObject*) override;
	virtual void UpdateLowerBodyState(CCharacterObject*) override {};
	virtual void ExitState(CCharacterObject*, AnimationData::Parts) override {};

	static CState_AnyTime* GetInstance()
	{
		static CState_AnyTime instance;

		return &instance;
	}
};

class CState_Idle : public CFSM_State<CCharacterObject>
{
public:
	CState_Idle() {};
	virtual ~CState_Idle() {};

	virtual void EnterState(CCharacterObject*, AnimationData::Parts) override;
	virtual void UpdateUpperBodyState(CCharacterObject*) override;
	virtual void UpdateLowerBodyState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*, AnimationData::Parts) override;

	static CState_Idle* GetInstance()
	{
		static CState_Idle instance;

		return &instance;
	}
};

class CState_Walk : public CFSM_State<CCharacterObject>
{
public:
	CState_Walk() {};
	virtual ~CState_Walk() {};

	virtual void EnterState(CCharacterObject*, AnimationData::Parts) override;
	virtual void UpdateUpperBodyState(CCharacterObject*) override;
	virtual void UpdateLowerBodyState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*, AnimationData::Parts) override;

	static CState_Walk* GetInstance()
	{
		static CState_Walk instance;

		return &instance;
	}
};

class CState_Crouch : public CFSM_State<CCharacterObject>
{
public:
	CState_Crouch() {};
	virtual ~CState_Crouch() {};

	virtual void EnterState(CCharacterObject*, AnimationData::Parts) override;
	virtual void UpdateUpperBodyState(CCharacterObject*) override;
	virtual void UpdateLowerBodyState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*, AnimationData::Parts) override;

	static CState_Crouch* GetInstance()
	{
		static CState_Crouch instance;

		return &instance;
	}
};

class CState_Reload : public CFSM_State<CCharacterObject>
{
public:
	CState_Reload() {};
	virtual ~CState_Reload() {};

	virtual void EnterState(CCharacterObject*, AnimationData::Parts) override;
	virtual void UpdateUpperBodyState(CCharacterObject*) override;
	virtual void UpdateLowerBodyState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*, AnimationData::Parts) override;

	static CState_Reload* GetInstance()
	{
		static CState_Reload instance;

		return &instance;
	}
};

class CState_Fire : public CFSM_State<CCharacterObject>
{
public:
	CState_Fire() {};
	virtual ~CState_Fire() {};

	virtual void EnterState(CCharacterObject*, AnimationData::Parts) override;
	virtual void UpdateUpperBodyState(CCharacterObject*) override;
	virtual void UpdateLowerBodyState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*, AnimationData::Parts) override;

	static CState_Fire* GetInstance()
	{
		static CState_Fire instance;

		return &instance;
	}
};

class CState_Run : public CFSM_State<CCharacterObject>
{
public:
	CState_Run() {};
	virtual ~CState_Run() {};

	virtual void EnterState(CCharacterObject*, AnimationData::Parts) override;
	virtual void UpdateUpperBodyState(CCharacterObject*) override;
	virtual void UpdateLowerBodyState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*, AnimationData::Parts) override;

	static CState_Run* GetInstance()
	{
		static CState_Run instance;

		return &instance;
	}
};

class CState_Death : public CFSM_State<CCharacterObject>
{
	DWORD		m_dwDeathWaitingTime = 0;
	DWORD		m_dwDeathStartTime = 0;
	XMVECTOR	m_Position;
public:
	CState_Death() {};
	virtual ~CState_Death() {};

	virtual void EnterState(CCharacterObject*, AnimationData::Parts) override;
	virtual void UpdateUpperBodyState(CCharacterObject*) override;
	virtual void UpdateLowerBodyState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*, AnimationData::Parts) override;

	static CState_Death* GetInstance()
	{
		static CState_Death instance;

		return &instance;
	}
};

class CState_HeadHit : public CFSM_State<CCharacterObject>
{
public:
	CState_HeadHit() {};
	virtual ~CState_HeadHit() {};

	virtual void EnterState(CCharacterObject*, AnimationData::Parts) override;
	virtual void UpdateUpperBodyState(CCharacterObject*) override;
	virtual void UpdateLowerBodyState(CCharacterObject*) override;
	virtual void ExitState(CCharacterObject*, AnimationData::Parts) override;

	static CState_HeadHit* GetInstance()
	{
		static CState_HeadHit instance;

		return &instance;
	}
};

