#include "stdafx.h"
#include "CollisionManager.h"

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::InitializeManager()
{
}

void CCollisionManager::ReleseManager()
{
}

void CCollisionManager::UpdateManager()
{
	// 0.5초 마다 충돌 초기화
	if (GLOBAL_MGR->SetTimer(m_dwInitTime, 500)) {
		for (auto& staticObject : m_vecStaticMeshContainer)
			staticObject->SetCollision(false);
	}

	/*
	for (auto& dynamicObject : m_vecDynamicMeshContainer) {
		if (dynamicObject->GetActive()) {
			// Dynamic to Dynamic
			for (auto& targetDynamicObject : m_vecDynamicMeshContainer) {
				if (dynamicObject == targetDynamicObject)
					continue; 

				if (targetDynamicObject->GetActive()) {

		//		dynamicObject->IsCollision(targetDynamicObject);
				}
			}

			// Dynamic to Static
			for (auto& staticObject : m_vecStaticMeshContainer) {
				if (dynamicObject->IsCollision(staticObject)) {
					dynamicObject->SetActive(false);
					staticObject->SetCollision(true);
				}

			}
		}
	}
	*/
}

bool CCollisionManager::RayCastCollision(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction)
{
	bool isCollision = false;
	float fNearestDistance = FLT_MAX;
	CGameObject* pNearestObject = nullptr;
	// Dynamic to Dynamic

	// Dynamic to Static
	for (auto& staticObject : m_vecStaticMeshContainer) {	
		if (staticObject->GetBoundingOBox().Intersects(originPos, direction, info.m_fDistance)) {
			if (fNearestDistance > info.m_fDistance) {
				fNearestDistance = info.m_fDistance;
				pNearestObject = staticObject;
				isCollision = true;
			}
		}
	}
	
	if (isCollision) {
		info.m_pHitObject = pNearestObject;
		info.m_fDistance = fNearestDistance;
		return true;
	}

	return false;
}

bool CCollisionManager::RayCastCollisionToCharacter(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction)
{
	bool isCollision = false;
	float fNearestDistance = FLT_MAX;
	CGameObject* pNearestObject = nullptr;

	// 1차 AABB
	for (auto& character : m_vecCharacterContainer) {
		if (character->GetBoundingBox(0).Intersects(originPos, direction, info.m_fDistance)) {
			if (fNearestDistance > info.m_fDistance) {
				fNearestDistance = info.m_fDistance;
				isCollision = true;
				pNearestObject = character;
				cout << "1차 충돌 " << endl;
			}
		}

	}
	// 2차 Parts
	for (auto& character : m_vecCharacterContainer) {
		if (character->GetPartsBoundingOBox(0).Intersects(originPos, direction, info.m_fDistance)) {
			if (fNearestDistance > info.m_fDistance) {
				fNearestDistance = info.m_fDistance;
				isCollision = true;
				pNearestObject = character;
				cout << "몸통 충돌 " << endl;
			}
		}
		if (character->GetPartsBoundingOBox(1).Intersects(originPos, direction, info.m_fDistance)) {
			if (fNearestDistance > info.m_fDistance) {
				fNearestDistance = info.m_fDistance;
				isCollision = true;
				pNearestObject = character;
				cout << "머리 충돌 " << endl;
			}
		}
	}

	// 3차 Polygon
	if (isCollision) {
		info.m_pHitObject = pNearestObject;
		if (RayCastCollisionInPolygon(info, originPos, direction)) {
			//	info.m_fDistance = fNearestDistance;

			return true;
		}
	}

	return false;
}

bool CCollisionManager::RayCastCollisionInPolygon(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction)
{
	XMMATRIX d3dxmtxInverse;
	d3dxmtxInverse = XMMatrixInverse(NULL, info.m_pHitObject->m_mtxWorld);

	XMVECTOR rayPosition;
	rayPosition = XMVector3TransformCoord(originPos, d3dxmtxInverse);

	XMVECTOR rayDirection;
	rayDirection = XMVector3TransformCoord(direction, d3dxmtxInverse);
	rayDirection = XMVector3Normalize(rayDirection);

	int count = info.m_pHitObject->GetMesh()->CheckRayIntersection(&rayPosition, &rayDirection, &info);
	cout << count << "여기까지 잘되는지 확인하자 " << endl;

	return true;
}

bool CCollisionManager::AABBCollision(CollisionInfo& info, BoundingBox bcBox)
{
	// Dynamic to Dynamic

	// Dynamic to Static
	for (auto& staticObject : m_vecStaticMeshContainer) {
		const bool isCollision = staticObject->GetBoundingBox().Intersects(bcBox);
	
		if (isCollision) {
			info.m_pHitObject = staticObject;
			return true;
		}
	}
	return false;
}

bool CCollisionManager::OBBCollision(CollisionInfo& info, BoundingOrientedBox bcObbox)
{
	// Dynamic to Dynamic

	// Dynamic to Static
	for (auto& staticObject : m_vecStaticMeshContainer) {
		const bool isCollision = staticObject->GetBoundingOBox().Intersects(bcObbox);

		if (isCollision) {
			info.m_pHitObject = staticObject;
			return true;
		}
	}
	return false;
}

bool CCollisionManager::CheckCollision(CollisionInfo& info, CGameObject* pOriginObject, CGameObject* pTargetObject)
{
	// 1. Sphere
	if (!pOriginObject->GetBoundingSphere().Intersects(pTargetObject->GetBoundingSphere()))
		return false;

	// 2. OBB			-> static Mesh에 대해서는 AABB 적용하기
	if (!pOriginObject->GetBoundingOBox().Intersects(pTargetObject->GetBoundingOBox()))
		return false;


	//if (pOriginObject->GetMeshTag()) 필요한 객체들만 확인하기
	// 3. Primitive
	if (!pOriginObject->IsCollision(pTargetObject))
		return false;

	return true;
}