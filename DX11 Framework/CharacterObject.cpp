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
			cout << "�浹 "<< endl;
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
					cout << "�浹ü ���� : " << endl;
					cout << "�Ÿ� : " << info.m_fDistance << endl;
//					cout << "���� : "; ShowXMFloat3(info.m_f3HitNormal);
//					cout << "�޽� �±� : " << static_cast<int>(staticObject->GetMeshTag()) << endl;

					XMVECTOR slidingVec = velocity - XMVector3Dot(velocity, XMLoadFloat3(&info.m_f3HitNormal)) * XMLoadFloat3(&info.m_f3HitNormal);
//					cout << "�����̵� ���� : "; ShowXMVector(slidingVec);

					// ��ġ �����ڵ� �߰��ϱ�
					m_pPlayer->SetVelocity(slidingVec);
					// �븻���� �������� distance ��ŭ �о��

		//			XMVECTOR offsetPos = XMLoadFloat3(&info.m_f3HitNormal) * info.m_fDistance;
		//			m_pPlayer->Move(offsetPos);
				}
			}
		}
	}
		
	/*
	// �Ʒ� �������� �浹 - ����� ���̿� ���缭 ��ġ�� �״�� set
	// -> ���� ������ distance�� �̿��Ͽ� ���̸� ���ϰ� �߷°��� �̿��Ͽ� �����ϵ��� �����ϱ�
	if (COLLISION_MGR->RayCastCollision(m_infoCollision, XMLoadFloat3(&bcObox.Center), -1 * GetUp())) {
		if (m_infoCollision.m_fDistance <= bcObox.Extents.y) {
			m_pPlayer->SetFloorCollision(true);
		}
	}
	*/



	/*
	// ------------------ �̸� ���� ���������� �̵����Ѽ� �浹Ȯ���Ѵٸ� �ٴڵ� �׽� �浹�Ǳ� ������ Ȯ���� �ȵȴ�. -----------
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