#include "stdafx.h"
#include "BulletObject.h"


CBulletObject::CBulletObject(float fRange, float fSpeed)
	: m_fRange(fRange), m_fSpeed(fSpeed)
{
}

CBulletObject::~CBulletObject()
{
}

void CBulletObject::InitializeData()
{
	SetActive(false);
	m_fMovingDistance = 0;
}

void CBulletObject::OnCollisionEnter()
{
	CollisionInfo info;

	if (COLLISION_MGR->RayCastCollision(info, GetvPosition(), GetLook(), m_fRange)) {
		cout << "CBulletObject - 충돌 되었네" << endl;
	}
}

void CBulletObject::Update(float fTimeElapsed)
{ 
	if (m_fMovingDistance < m_fRange) {
		m_fMovingDistance += m_fSpeed * fTimeElapsed;
		MoveForward(m_fSpeed * fTimeElapsed);
	}
	else {
		InitializeData();
	}

	OnCollisionEnter();
}