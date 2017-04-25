#include "stdafx.h"
#include "CharacterObject.h"

CCharacterObject::CCharacterObject()
{
}

CCharacterObject::~CCharacterObject()
{
	SafeDelete(m_pWeapon);
}

void CCharacterObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CSkinnedObject::CreateObjectData(pd3dDevice);
	CreateWeapon(pd3dDevice);
}

void CCharacterObject::Fire()
{
	SetAnimation(AnimationData::CharacterAnim::eStandingFire);
	m_pWeapon->Fire();
}

void CCharacterObject::OnCollisionCheck()
{	
	BoundingOrientedBox bcObox = GetBoundingOBox();

	CollisionInfo info;
	/*
	for (auto targetObject : COLLISION_MGR->m_vecStaticMeshContainer) {
		if (COLLISION_MGR->CheckCollision(info, this, targetObject))
			cout << "충돌 "<< endl;
	}
	*/

	XMVECTOR velocity = m_pPlayer->GetVelocity();

	if(m_pPlayer->IsMoving()){
		for (auto staticObject : COLLISION_MGR->m_vecStaticMeshContainer) {

			XMMATRIX d3dxmtxInverse;
			d3dxmtxInverse = XMMatrixInverse(NULL, staticObject->m_mtxWorld);

			XMVECTOR rayPosition;
			rayPosition = XMVector3TransformCoord(XMLoadFloat3(&GetPosition()), d3dxmtxInverse);
			
			XMVECTOR rayDirection;
			rayDirection = XMVector3TransformCoord(velocity, d3dxmtxInverse);
			rayDirection = XMVector3Normalize(rayDirection);

			int count = staticObject->GetMesh()->CheckRayIntersection(&rayPosition, &rayDirection, &info);

			if (count) {
				if (info.m_fDistance < 5) {
					cout << "충돌체 정보 : " << endl;
					cout << "거리 : " << info.m_fDistance << endl;
//					cout << "법선 : "; ShowXMFloat3(info.m_f3HitNormal);
//					cout << "메쉬 태그 : " << static_cast<int>(staticObject->GetMeshTag()) << endl;

					XMVECTOR slidingVec = velocity - XMVector3Dot(velocity, XMLoadFloat3(&info.m_f3HitNormal)) * XMLoadFloat3(&info.m_f3HitNormal);
//					cout << "슬라이딩 벡터 : "; ShowXMVector(slidingVec);

					// 위치 보정코드 추가하기
					m_pPlayer->SetVelocity(slidingVec);
					// 노말벡터 방향으로 distance 만큼 밀어내기

		//			XMVECTOR offsetPos = XMLoadFloat3(&info.m_f3HitNormal) * info.m_fDistance;
		//			m_pPlayer->Move(offsetPos);
				}
			}
		}
	}
		
	/*
	// 아래 지형과의 충돌 - 현재는 높이에 맞춰서 위치를 그대로 set
	// -> 추후 구해진 distance를 이용하여 높이를 정하고 중력값을 이용하여 낙하하도록 구현하기
	if (COLLISION_MGR->RayCastCollision(m_infoCollision, XMLoadFloat3(&bcObox.Center), -1 * GetUp())) {
		if (m_infoCollision.m_fDistance <= bcObox.Extents.y) {
			m_pPlayer->SetFloorCollision(true);
		}
	}
	*/



	/*
	// ------------------ 미리 다음 프레임으로 이동시켜서 충돌확인한다면 바닥도 항시 충돌되기 때문에 확인이 안된다. -----------
	XMVECTOR nowPos = GetvPosition();
	XMVECTOR nextPos = nowPos + m_pPlayer->GetVelocity();
	XMMATRIX mtxNextWorld = m_mtxWorld;
	XMFLOAT4X4 mtxf4World; XMStoreFloat4x4(&mtxf4World, mtxNextWorld);
	XMFLOAT3 pos; XMStoreFloat3(&pos, nextPos);

	mtxf4World._41 = pos.x;
	mtxf4World._42 = pos.y;
	mtxf4World._43 = pos.z;
	mtxNextWorld = XMLoadFloat4x4(&mtxf4World);

	BoundingOrientedBox bcObox2; GetBoundingOBox(true).Transform(bcObox2, mtxNextWorld);


	for (auto staticObject : COLLISION_MGR->m_vecStaticMeshContainer) {
		if (pPlaneObject != staticObject) {
			if (staticObject->GetBoundingOBox().Intersects(bcObox2)) {
				ShowXMVector(m_pPlayer->GetVelocity());
				m_pPlayer->SetVelocity(XMVectorSet(0,0,0,0));
			}
		}
	}
	*/
} 

void CCharacterObject::Update(float fTimeElapsed)
{
	if (m_pPlayer) {
		m_pPlayer->OnPrepareRender();
		m_pPlayer->UpdateKeyInput(fTimeElapsed);
		OnCollisionCheck();
		m_pPlayer->Update(fTimeElapsed);
	}

	CGameObject::Update(fTimeElapsed);
	CSkinnedObject::Update(fTimeElapsed);
	m_pWeapon->Update(fTimeElapsed);
}

void CCharacterObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
//	if (m_pPlayer->GetCamera()->GetCameraTag() == CameraTag::eThirdPerson) {

	if(m_pPlayer)
		m_pPlayer->UpdateShaderVariables(pd3dDeviceContext);
		CSkinnedObject::Render(pd3dDeviceContext, pCamera);
//	}
	m_pWeapon->Render(pd3dDeviceContext, pCamera);
}