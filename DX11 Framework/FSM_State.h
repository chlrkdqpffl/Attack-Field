#pragma once

template <typename ObjectType>
class CFSM_State
{
public:
	CFSM_State() {};
	virtual ~CFSM_State() {};

	virtual void EnterState(ObjectType*, CharacterParts) {};
	virtual void UpdateUpperBodyState(ObjectType*) {};
	virtual void UpdateLowerBodyState(ObjectType*) {};
	virtual void ExitState(ObjectType*, CharacterParts) {};
};