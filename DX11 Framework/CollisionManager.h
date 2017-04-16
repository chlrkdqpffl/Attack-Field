#pragma once
#include "SingletonManager.h"
#include "Object.h"

struct CollisionInfo 
{
	CGameObject*	m_pHitObject = nullptr;
	float			m_fDistance = FLT_MAX;
	XMFLOAT3		m_f3HitNormal = XMFLOAT3(0,0,0);

};

class CCollisionManager : public CSingletonManager<CCollisionManager>
{
	DWORD								m_dwInitTime = 0;
public:
	vector<CGameObject*>				m_vecStaticMeshContainer;
	vector<CGameObject*>				m_vecDynamicMeshContainer;

public:
	CCollisionManager();
	virtual ~CCollisionManager();

	virtual void InitializeManager() override;
	virtual void UpdateManager() override;
	virtual void ReleseManager() override;

	bool RayCastCollision(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction);
	bool AABBCollision(CollisionInfo& info, BoundingBox bcBox);
	bool OBBCollision(CollisionInfo& info, BoundingOrientedBox bcObbox);
};