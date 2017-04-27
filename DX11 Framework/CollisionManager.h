#pragma once
#include "SingletonManager.h"
#include "Object.h"

class CCollisionManager : public CSingletonManager<CCollisionManager>
{
	DWORD								m_dwInitTime = 0;
public:
	vector<CGameObject*>				m_vecStaticMeshContainer;
	vector<CGameObject*>				m_vecDynamicMeshContainer;
	vector<CCharacterObject*>			m_vecCharacterContainer;

public:
	CCollisionManager();
	virtual ~CCollisionManager();

	virtual void InitializeManager() override;
	virtual void UpdateManager() override;
	virtual void ReleseManager() override;

	bool RayCastCollision(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction);
	bool RayCastCollisionToCharacterParts(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction);
	bool AABBCollision(CollisionInfo& info, BoundingBox bcBox);
	bool OBBCollision(CollisionInfo& info, BoundingOrientedBox bcObbox);



	// User Define
	bool CheckCollision(CollisionInfo& info, CGameObject* pObjectA, CGameObject* pObjectB);
};