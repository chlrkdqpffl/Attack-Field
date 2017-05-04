#pragma once

template <typename ObjectType>
class CFSM_State
{
public:
	CFSM_State() {};
	~CFSM_State() {};

	virtual void EnterState(ObjectType*) = 0;
	virtual void UpdateState(ObjectType*) = 0;
	virtual void ExitState(ObjectType*) = 0;
};