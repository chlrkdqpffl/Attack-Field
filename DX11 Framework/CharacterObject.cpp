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
	WORD wKeyState = m_pPlayer->GetKeyState();
	BoundingOrientedBox bcObox = GetBoundingOBox();

	if (wKeyState & static_cast<int>(KeyInput::eForward)) {
		XMVECTOR forwardPos = XMLoadFloat3(&bcObox.Center) + XMVectorSet(0, bcObox.Extents.z, 0, 0);
		if (COLLISION_MGR->RayCastCollision(m_infoCollision, forwardPos, GetLook())) {
			if (m_infoCollision.m_fDistance <= 0.0f) {
				XMVECTOR velocity = m_pPlayer->GetVelocity();
				velocity -= m_pPlayer->GetLookVector();

				m_pPlayer->SetVelocity(velocity);

				XMFLOAT3 pos = GetPosition();
				float fixPos = m_infoCollision.m_pHitObject->GetBoundingOBox().Center.z + m_infoCollision.m_pHitObject->GetBoundingOBox().Extents.z;

				SetPosition(pos.x, pos.y, fixPos);
			}	
		}
	}
	
	if (wKeyState & static_cast<int>(KeyInput::eLeft)) {
		XMVECTOR leftPos = XMLoadFloat3(&bcObox.Center) - XMVectorSet(0, bcObox.Extents.x, 0, 0);

	//	if (COLLISION_MGR->RayCastCollision(m_infoCollision, leftPos, -1 * GetRight())) 
		if (COLLISION_MGR->RayCastCollision(m_infoCollision, leftPos, XMVectorSet(-1, 0, 0, 0))) {
			if (m_infoCollision.m_fDistance <= 0.0f) {
				XMVECTOR velocity = m_pPlayer->GetVelocity();
				velocity += m_pPlayer->GetRightVector();

				m_pPlayer->SetVelocity(velocity);

				XMFLOAT3 pos = GetPosition();
				float fixPos = m_infoCollision.m_pHitObject->GetBoundingOBox().Center.x + m_infoCollision.m_pHitObject->GetBoundingOBox().Extents.x;

				SetPosition(fixPos, pos.y, pos.z);
			}
		}
	}
	
	if (wKeyState & static_cast<int>(KeyInput::eRight)) {
		XMVECTOR rightPos = XMLoadFloat3(&bcObox.Center) + XMVectorSet(0, bcObox.Extents.x, 0, 0);
	
		//	if (COLLISION_MGR->RayCastCollision(m_infoCollision, leftPos, -1 * GetRight())) 
		if (COLLISION_MGR->RayCastCollision(m_infoCollision, rightPos, XMVectorSet(1, 0, 0, 0))) {
			if (m_infoCollision.m_fDistance <= 0.0f) {
				XMVECTOR velocity = m_pPlayer->GetVelocity();
				velocity -= m_pPlayer->GetRightVector();
				
				m_pPlayer->SetVelocity(velocity);

				XMFLOAT3 pos = GetPosition();
				float fixPos = m_infoCollision.m_pHitObject->GetBoundingOBox().Center.x + m_infoCollision.m_pHitObject->GetBoundingOBox().Extents.x;

				SetPosition(fixPos, pos.y, pos.z);
			}
		}
	}



	// 아래 지형과의 충돌 - 현재는 높이에 맞춰서 위치를 그대로 set
	// -> 추후 구해진 distance를 이용하여 높이를 정하고 중력값을 이용하여 낙하하도록 구현하기
	
	/*
	XMVECTOR toePos = XMLoadFloat3(&bcObox.Center) - XMVectorSet(0, bcObox.Extents.y, 0, 0);
	if (COLLISION_MGR->RayCastCollision(m_infoCollision, toePos, -1 * GetUp())) {
		if (m_infoCollision.m_fDistance <= 0.0f) {
			m_pPlayer->SetFloorCollision(true);
		}
	}
	*/

	/* ------------------ 미리 다음 프레임으로 이동시켜서 충돌확인한다면 바닥도 항시 충돌되기 때문에 확인이 안된다. -----------
	XMVECTOR nowPos = GetvPosition();
	XMVECTOR nextPos = nowPos + m_pPlayer->GetVelocity();
	XMMATRIX mtxWorld = m_mtxWorld;
	XMFLOAT4X4 mtxf4World; XMStoreFloat4x4(&mtxf4World, mtxWorld);
	XMFLOAT3 pos; XMStoreFloat3(&pos, nextPos);

	mtxf4World._41 = pos.x;
	mtxf4World._42 = pos.y;
	mtxf4World._43 = pos.z;
	mtxWorld = XMLoadFloat4x4(&mtxf4World);

	BoundingOrientedBox bcObox; GetBoundingOBox(true).Transform(bcObox, mtxWorld);


	for (auto staticObject : COLLISION_MGR->m_vecStaticMeshContainer) {
		if (staticObject->GetBoundingOBox().Intersects(bcObox)) {
			cout << "충돌했네 이전 위치로 돌리자" << endl;
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