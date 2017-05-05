#pragma once

template <typename ObjectType>
class CFSM_State
{
protected:
	
public:
	CFSM_State() {};
	virtual ~CFSM_State() {};

	virtual void EnterState(ObjectType*) {};
	virtual void UpdateState(ObjectType*) {};
	virtual void ExitState(ObjectType*) {};
};