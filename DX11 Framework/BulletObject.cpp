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

void CBulletObject::OnCollisionCheck()
{
	CollisionInfo info;
	
	if (COLLISION_MGR->RayCastCollisionToCharacter(info, GetvPosition(), GetvLook())) {
		//SetActive(false);
		//info.m_pHitObject->SetCollision(true);
		cout << "CBulletObject::OnCollisionCheck() 충돌함" << endl;
	}
}

void CBulletObject::Update(float fDeltaTime)
{ 
	if (m_fMovingDistance < m_fRange) {
		m_fMovingDistance += m_fSpeed * fDeltaTime;
		MoveForward(m_fSpeed * fDeltaTime);
	}
	else {
		InitializeData();
	}

//	OnCollisionCheck(); - 레이 충돌하므로 제외함
}
