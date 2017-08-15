#pragma once
#include "SingletonManager.h"
#include "Object.h"

class CCollisionManager : public CSingletonManager<CCollisionManager>
{
public:
	vector<CGameObject*>				m_vecStaticMeshContainer;
//	vector<CGameObject*>				m_vecDynamicMeshContainer;
	vector<CCharacterObject*>			m_vecCharacterContainer;

public:
	CCollisionManager();
	virtual ~CCollisionManager();

	virtual void InitializeManager() override;
	virtual void UpdateManager() override;
	virtual void ReleseManager() override;

	void InitCollisionInfo();

	bool RayCastCollision(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction);
	bool RayCastCollisionToCharacter(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction);
	bool RayCastCollisionToCharacter_Parts(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction);
	bool AABBCollision(CollisionInfo& info, BoundingBox bcBox);
	bool OBBCollision(CollisionInfo& info, BoundingOrientedBox bcObbox);

	// User Define
	bool CheckCollision(CollisionInfo& info, CGameObject* pObjectA, CGameObject* pObjectB);
	void CreateFireDirectionLine(XMVECTOR position, XMVECTOR direction, float length);

private:
	bool RayCastCollision_AABB(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction);
	bool RayCastCollisionToCharacter_AABB(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction);
	bool RayCastCollisionInPolygon(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction);
};