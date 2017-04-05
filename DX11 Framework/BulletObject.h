#pragma once
#include "Object.h"

class CBulletObject : public CGameObject
{
	float			m_fSpeed = 0.0f;
	float			m_fRange = 0.0f;
	float			m_fMovingDistance = 0.0f;

public:
	CBulletObject(float fRange, float fSpeed);
	virtual ~CBulletObject();

	virtual void Update(float fTimeElapsed) override;
	virtual void OnCollisionCheck() override;

	void InitializeData();
	// ----- Get, Setter ----- //
};

