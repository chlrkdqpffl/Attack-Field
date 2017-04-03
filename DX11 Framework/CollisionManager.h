#pragma once
#include "SingletonManager.h"
#include "Object.h"

struct CollisionInfo 
{
	CGameObject*	m_pHitObject = nullptr;
	float			m_fDistance = 0.0f;
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

	bool RayCastCollision(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction, float distance = FLT_MAX);
	void AABBCollision();
	void OBBCollision();
};