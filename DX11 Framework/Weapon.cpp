#include "stdafx.h"
#include "Weapon.h"
#include "CharacterObject.h"

CWeapon::CWeapon(CCharacterObject* pOwner) 
	: m_pOwner(pOwner)
{
//	TWBAR_MGR->g_xmf3Offset = XMFLOAT3(1.0f, 1.4f, -0.05f);
//	TWBAR_MGR->g_xmf3Rotate = XMFLOAT3(-30.f, 125.f, 85.f);	
}

CWeapon::~CWeapon()
{
	for (auto& bullet : m_vecBulletContainer)
		SafeDelete(bullet);
}

void CWeapon::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CGameObject::CreateObjectData(pd3dDevice);
	CreateBulletPool(pd3dDevice);
}

void CWeapon::CreateFireDirectionLine(XMVECTOR direction)
{
	float lineLength = 10;
	XMVECTOR endPos = GetvPosition() + (direction * lineLength);
	CLineObject* pRayObject = new CLineObject(GetvPosition(), endPos, 3000);
	pRayObject->CreateObjectData(STATEOBJ_MGR->g_pd3dDevice);

	GLOBAL_MGR->g_vecLineContainer.push_back(pRayObject);
}

void CWeapon::Firing(XMVECTOR direction)
{
	if (GetTickCount() - m_dwLastAttackTime >= m_uiFireSpeed) {
		m_dwLastAttackTime = GetTickCount();

		CollisionInfo info;
		if (COLLISION_MGR->RayCastCollisionToCharacter(info, GetvPosition(), direction)) 
		{
			info.m_pHitObject->SetCollision(true);	//���⼭ hp�� ��´�.
		}
	
		CreateFireDirectionLine(direction);
		/*
		// �Ʒ��� �Ѿ� �������� �ʿ��� �ڵ�
		for (auto bullet : m_vecBulletContainer) {
			if (false == bullet->GetActive()) {
				bullet->SetActive(true);
				bullet->SetPosition(GetPosition());

				bullet->SetvLook(direction);
				break;
			}
		}
		*/
	}
}

void CWeapon::Update(float fDeltaTime)
{
	CGameObject::Update(fDeltaTime);

	for (auto bullet : m_vecBulletContainer) {
		if (bullet->GetActive())
			bullet->Update(fDeltaTime);
	}

	m_mtxParent = m_pOwner->GetSkinnedMesh()->GetFinalBoneMtx(m_nBoneIndex);
	m_mtxWorld = m_mtxLocal * m_mtxParent * m_pOwner->m_mtxWorld;
//	SetRotate(TWBAR_MGR->g_xmf3Rotate, true);
//	SetPosition(TWBAR_MGR->g_xmf3Offset, true);
}

void CWeapon::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CGameObject::Render(pd3dDeviceContext, pCamera);

	for (auto bullet : m_vecBulletContainer)
		if (bullet->GetActive()) 
			bullet->Render(pd3dDeviceContext, pCamera);
}