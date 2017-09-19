#include "stdafx.h"
#include "CollisionManager.h"

void CCollisionManager::InitializeManager()
{
}

void CCollisionManager::ReleseManager()
{
}

void CCollisionManager::UpdateManager()
{
}

void CCollisionManager::InitCollisionInfo()
{
	// 1.5�� ���� �浹 ���� �ʱ�ȭ
	if (GetTickCount() - m_dwInitCollisionInfoTime > 1500) {
		m_dwInitCollisionInfoTime = GetTickCount();

		for (auto& staticObject : m_vecStaticMeshContainer)
			staticObject->InitCollisionInfo();

		for (auto& character : m_vecCharacterContainer)
			character->InitCollisionInfo();
	}
}

void CCollisionManager::ClearContainer()
{
	m_vecStaticMeshContainer.clear();
	m_vecCharacterContainer.clear();
}

bool CCollisionManager::RayCastCollision(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction)
{
	bool isCollision = false;
	float fNearestDistance = FLT_MAX;
	CGameObject* pNearestObject = nullptr;
	// Dynamic to Dynamic

	// Dynamic to Static
	for (auto& staticObject : m_vecStaticMeshContainer) {	
		/*
		// Ray�� �ٿ�� �ڽ� �ȿ� �ִ��� Ȯ��
		if (CONTAINS == staticObject->GetBoundingOBox().Contains(originPos)) {

			fNearestDistance = XMVectorGetX(XMVector3LengthEst(staticObject->GetvPosition() - originPos));
			cout << "���� �Ÿ� : " << fNearestDistance << endl;
			pNearestObject = staticObject;
			isCollision = true;
			break;
		}
		*/
		if (staticObject->GetBoundingOBox().Intersects(originPos, direction, info.m_fDistance)) {
			if (0 < info.m_fDistance && info.m_fDistance < fNearestDistance) {
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

bool CCollisionManager::RayCastCollisionToCharacter(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction)	//ĳ���� �浹 �Լ�.
{
	// 1�� AABB
	if (!RayCastCollisionToCharacter_AABB(info, originPos, direction))
		return false;

	// 3�� Parts
	if (!RayCastCollisionToCharacter_Parts(info, originPos, direction))
		return false;
	
	return true;
}

bool CCollisionManager::RayCastCollisionInPolygon(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction)
{
	XMMATRIX d3dxmtxInverse;
	d3dxmtxInverse = XMMatrixInverse(NULL, info.m_pHitObject->m_mtxWorld);

	XMVECTOR rayPosition;
	rayPosition = XMVector3TransformCoord(originPos, d3dxmtxInverse);

	XMVECTOR rayDirection;
	rayDirection = XMVector3TransformNormal(direction, d3dxmtxInverse);
	rayDirection = XMVector3Normalize(rayDirection);

	
	// ----- Rendering Ray Cast ----- // 
	float lineLength = 200;
	XMVECTOR endPos = rayPosition + (rayDirection * lineLength);
	CLineObject* pRayObject = new CLineObject(rayPosition, endPos, 3000, XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));
	pRayObject->CreateObjectData(STATEOBJ_MGR->g_pd3dDevice);

	GLOBAL_MGR->g_vecLineContainer.push_back(pRayObject);
	// ------------------------------ //
	
	int count = info.m_pHitObject->GetMesh()->CheckRayIntersection(&rayPosition, &rayDirection, &info);

	if (count) {
		cout << count << "������� �ߵǴ��� Ȯ������ " << endl;

		return true;
	}

	return false;
}

bool CCollisionManager::RayCastCollision_AABB(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction)
{
	CGameObject* pNearestObject = nullptr;
	float fNearestDistance = FLT_MAX;
	float fDist = 0;
	bool isCollision = false;

	for (auto& object : m_vecStaticMeshContainer) {
		if (object->GetBoundingBox(0).Intersects(originPos, direction, info.m_fDistance)) {
			if (fNearestDistance > info.m_fDistance) {
				fNearestDistance = info.m_fDistance;
				pNearestObject = object;
				isCollision = true;
				cout << "1�� �浹 " << endl;
			}
		}
	}

	if (isCollision) {
		info.m_pHitObject = pNearestObject;
		return true;
	}
	return false;
}

bool CCollisionManager::RayCastCollisionToCharacter_AABB(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction)
{
	CGameObject* pNearestObject = nullptr;
	float fNearestDistance = FLT_MAX;
	float fDist = 0;
	bool isCollision = false;

	for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer()) {
		if (!character->GetBoundingSphere().Intersects(originPos, direction, info.m_fDistance))
			return false;
	
		info.m_pHitObject = character;
		if (info.m_pHitObject->GetBoundingBox(0).Intersects(originPos, direction, info.m_fDistance)) {
			if (info.m_fDistance > 0) {			// ������ �� Ray���� Ȯ��
				if (fNearestDistance > info.m_fDistance) {
					fNearestDistance = info.m_fDistance;
					pNearestObject = character;
					isCollision = true;
				}
			}
		}
	}

	if (isCollision) {
		info.m_pHitObject = pNearestObject;
		return true;
	}
	return false;
}

bool CCollisionManager::RayCastCollisionToCharacter_Parts(CollisionInfo& info, XMVECTOR originPos, XMVECTOR direction)
{
	CCharacterObject* pCollisionObject = static_cast<CCharacterObject*>(info.m_pHitObject);

	for (int i = 0; i < 10; ++i) {
		if (pCollisionObject->GetPartsBoundingOBox(i).Intersects(originPos, direction, info.m_fDistance)) {
			info.m_HitParts = static_cast<ChracterBoundingBoxParts>(i);
			return true;
		}
	}
	return false;
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

	// 2. OBB			-> static Mesh�� ���ؼ��� AABB �����ϱ�
	if (!pOriginObject->GetBoundingOBox().Intersects(pTargetObject->GetBoundingOBox()))
		return false;


	//if (pOriginObject->GetMeshTag()) �ʿ��� ��ü�鸸 Ȯ���ϱ�
	// 3. Primitive
	if (!pOriginObject->IsCollision(pTargetObject))
		return false;

	return true;
}

void CCollisionManager::CreateFireDirectionLine(XMVECTOR position, XMVECTOR direction, float length)
{
	XMVECTOR endPos = position + (direction * length);
	CLineObject* pRayObject = new CLineObject(position, endPos, 3000);
	pRayObject->CreateObjectData(STATEOBJ_MGR->g_pd3dDevice);

	GLOBAL_MGR->g_vecLineContainer.push_back(pRayObject);
}