#include "stdafx.h"
#include "CharacterObject.h"

CCharacterObject::CCharacterObject()
{
	for (int i = 0; i < ePartsCount; ++i)
		m_pPartsBoundingBoxMesh[i] = nullptr;
}

CCharacterObject::~CCharacterObject()
{
	SafeDelete(m_pWeapon);

	for (int i = 0; i < ePartsCount; ++i)
		SafeDelete(m_pPartsBoundingBoxMesh[i]);

	SafeDelete(m_pStateMachine);
}

void CCharacterObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CSkinnedObject::CreateObjectData(pd3dDevice);
	CreateWeapon(pd3dDevice);
	CreateStateMachine();
}

void CCharacterObject::CreateStateMachine()
{
	m_pStateMachine = new CStateMachine<CCharacterObject>(this);
}

BoundingOrientedBox CCharacterObject::GetPartsBoundingOBox(UINT index) const
{
	BoundingOrientedBox bcObox = m_bcPartsBoundingOBox[index];
	bcObox.Transform(bcObox, m_mtxPartsBoundingWorld[index]);
	return bcObox;
}

void CCharacterObject::Firing()
{
//	SetAnimation(AnimationData::CharacterAnim::eStandingFire);
	m_pWeapon->Firing(XMLoadFloat3(&m_f3FiringDirection));
}

void CCharacterObject::Running()
{
	SetAnimation(AnimationData::CharacterAnim::eRun);
//	m_pPlayer->GetCamera()->Move(GetLook() * 1);			// 추후 구현
}

void CCharacterObject::OnCollisionCheck()
{	
	if(IsMoving()){
		for (auto staticObject : COLLISION_MGR->m_vecStaticMeshContainer) {
			CollisionInfo info;
			XMVECTOR velocity = XMLoadFloat3(&m_f3Velocity);

			XMMATRIX d3dxmtxInverse;
			d3dxmtxInverse = XMMatrixInverse(NULL, staticObject->m_mtxWorld);

			XMVECTOR rayPosition;
			rayPosition = XMVector3TransformCoord(GetvPosition(), d3dxmtxInverse);
			
			XMVECTOR rayDirection;
			rayDirection = XMVector3TransformNormal(velocity, d3dxmtxInverse);
			rayDirection = XMVector3Normalize(rayDirection);

			int collisionObjectCount = staticObject->GetMesh()->CheckRayIntersection(&rayPosition, &rayDirection, &info);

			if (collisionObjectCount) {
				if (info.m_fDistance < 4) {
					XMVECTOR slidingVec = velocity - XMVector3Dot(velocity, XMLoadFloat3(&info.m_f3HitNormal)) * XMLoadFloat3(&info.m_f3HitNormal);

					// ----- 슬라이딩 벡터 충돌 ----- //
					XMVECTOR rayDirection;
					rayDirection = XMVector3TransformNormal(slidingVec, d3dxmtxInverse);
					rayDirection = XMVector3Normalize(rayDirection);
					
					CollisionInfo slidingVecInfo;

					int collisionSlidingObjectCount = staticObject->GetMesh()->CheckRayIntersection(&rayPosition, &rayDirection, &slidingVecInfo);
					if (collisionSlidingObjectCount) {
						if (slidingVecInfo.m_fDistance < 2) {
							XMVECTOR normal1 = XMLoadFloat3(&info.m_f3HitNormal);
							XMVECTOR normal2 = XMLoadFloat3(&slidingVecInfo.m_f3HitNormal);
							float fDot = XMVectorGetX(XMVector3Dot(normal1, normal2));

							if (fDot == 0) {			// 직각
								slidingVec = XMVectorSet(0, 0, 0, 0);
							}
							else if (fDot < 0) {		// 예각
								slidingVec = velocity - XMVector3Dot(velocity, XMLoadFloat3(&slidingVecInfo.m_f3HitNormal)) * XMLoadFloat3(&slidingVecInfo.m_f3HitNormal);
							}
							else {						// 둔각
								slidingVec = XMVectorSet(0, 0, 0, 0);
							}
						}
					}
					m_pPlayer->SetvVelocity(slidingVec);	
				}
			}
		}
	}

	// Ground Collision
	XMVECTOR centerPos = m_pPlayer->GetvPrevPosition();			// 혹시 몰라서 이전 프레임의 위치로 설정
	BoundingOrientedBox bcObox = GetBoundingOBox();

	if (COLLISION_MGR->RayCastCollision(m_infoCollision, centerPos, -1 * GetUp())) {
		if (m_infoCollision.m_fDistance <= bcObox.Extents.y) {
			m_pPlayer->SetFloorCollision(true);
		}
	}
	
	/*
	// ----- Rendering Ray Cast ----- // 
	XMVECTOR endPos = centerPos + (-1 * GetUp() * m_infoCollision.m_fDistance);
	CLineObject* pRayObject = new CLineObject(centerPos, endPos, 100);
	pRayObject->CreateObjectData(STATEOBJ_MGR->g_pd3dDevice);

	GLOBAL_MGR->g_vecLineContainer.push_back(pRayObject);
	// ------------------------------ //
	*/
} 

void CCharacterObject::SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal)
{
	CGameObject::SetRotate(0, fYaw, fRoll, isLocal);
	m_fPitch = fPitch;
}

void CCharacterObject::SetRotate(XMFLOAT3 fAngle, bool isLocal)
{
	CGameObject::SetRotate(0, fAngle.y, fAngle.z, isLocal);
	m_fPitch = fAngle.x;
}

void CCharacterObject::SetRotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal)
{
	XMFLOAT3 axis; XMStoreFloat3(&axis, *pd3dxvAxis);

	CGameObject::SetRotate(0, axis.y, axis.z, isLocal);
	m_fPitch = axis.x;
}

void CCharacterObject::RotateFiringPos()
{
	if (m_pPlayer) {
		XMVECTOR firingDirection = XMLoadFloat3(&m_f3FiringDirection);
		XMMATRIX mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetvRight(), XMConvertToRadians(m_fPitch));
		firingDirection = XMVector3TransformNormal(m_pPlayer->GetvLook(), mtxRotate);
		XMStoreFloat3(&m_f3FiringDirection, firingDirection);
	}

	m_fPitch = clamp(m_fPitch, -40.0f, 50.0f);		// 이 각도는 캐릭터가 최대로 허리를 숙이는 각도로 캐릭터마다 다르지만 현재는 여기에서 clamp하도록 만듦
	GetSkinnedMesh()->SetPitch(m_fPitch);
}

void CCharacterObject::Update(float fDeltaTime)
{ 
	RotateFiringPos();
	if (m_pPlayer) {
		m_pPlayer->UpdateKeyInput(fDeltaTime);
		OnCollisionCheck();
	}
	m_pStateMachine->Update();

	if (m_pPlayer) {
		m_pPlayer->Update(fDeltaTime);
	}

	CGameObject::Update(fDeltaTime);
	CSkinnedObject::Update(fDeltaTime);
	m_pWeapon->Update(fDeltaTime);
}

void CCharacterObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if(m_pPlayer)
		m_pPlayer->UpdateShaderVariables(pd3dDeviceContext);

	CSkinnedObject::Render(pd3dDeviceContext, pCamera);
	m_pWeapon->Render(pd3dDeviceContext, pCamera);
}

void CCharacterObject::BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_mtxPartsBoundingWorld[BoundingBoxParts::eBody] = GetSkinnedMesh()->GetFinalBoneMtx(2) * m_mtxWorld;
	m_mtxPartsBoundingWorld[BoundingBoxParts::eHead] = GetSkinnedMesh()->GetFinalBoneMtx(5) * m_mtxWorld;

	// --- Arm --- //
	m_mtxPartsBoundingWorld[BoundingBoxParts::eLeftUpArm] = GetSkinnedMesh()->GetFinalBoneMtx(7) * m_mtxWorld;
	m_mtxPartsBoundingWorld[BoundingBoxParts::eLeftDownArm] = GetSkinnedMesh()->GetFinalBoneMtx(8) * m_mtxWorld;

	m_mtxPartsBoundingWorld[BoundingBoxParts::eRightUpArm] = GetSkinnedMesh()->GetFinalBoneMtx(20) * m_mtxWorld;
	m_mtxPartsBoundingWorld[BoundingBoxParts::eRightDownArm] = GetSkinnedMesh()->GetFinalBoneMtx(21) * m_mtxWorld;

	// --- Leg --- //
	m_mtxPartsBoundingWorld[BoundingBoxParts::eLeftUpLeg] = GetSkinnedMesh()->GetFinalBoneMtx(32) * m_mtxWorld;
	m_mtxPartsBoundingWorld[BoundingBoxParts::eLeftDownLeg] = GetSkinnedMesh()->GetFinalBoneMtx(33) * m_mtxWorld;

	m_mtxPartsBoundingWorld[BoundingBoxParts::eRightUpLeg] = GetSkinnedMesh()->GetFinalBoneMtx(36) * m_mtxWorld;
	m_mtxPartsBoundingWorld[BoundingBoxParts::eRightDownLeg] = GetSkinnedMesh()->GetFinalBoneMtx(37) * m_mtxWorld;
	
	for (int i = 0; i < ePartsCount; ++i) {
		if (m_pPartsBoundingBoxMesh[i]) {
			BoundingOrientedBox bcObbox;
			XMMATRIX mtxBoundingBoxWorld = m_mtxPartsBoundingWorld[i];

			m_bcPartsBoundingOBox[i].Transform(bcObbox, mtxBoundingBoxWorld);
			
			mtxBoundingBoxWorld = XMMatrixRotationQuaternion(XMLoadFloat4(&bcObbox.Orientation)) * 
				XMMatrixTranslation(bcObbox.Center.x, bcObbox.Center.y, bcObbox.Center.z);
			
			CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &mtxBoundingBoxWorld);

			m_pPartsBoundingBoxMesh[i]->Render(pd3dDeviceContext);
		}
	}

	if (m_pBoundingBoxMesh) {
		XMMATRIX mtxBoundingBoxWorld = m_mtxWorld;
		BoundingOrientedBox bcObbox;
		m_bcMeshBoundingOBox.Transform(bcObbox, mtxBoundingBoxWorld);

		mtxBoundingBoxWorld = XMMatrixRotationQuaternion(XMLoadFloat4(&bcObbox.Orientation)) *
			XMMatrixTranslation(bcObbox.Center.x, bcObbox.Center.y, bcObbox.Center.z);

		CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &mtxBoundingBoxWorld);

		m_pBoundingBoxMesh->Render(pd3dDeviceContext);
	}
}