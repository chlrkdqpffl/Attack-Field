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
		SOUND_MGR->Play3DSound(SoundTag::eFire, m_pOwner->GetPosition(), m_pOwner->GetLook(), 1, 1);
		SOUND_MGR->Play3DSound(SoundTag::eShellsFall, m_pOwner->GetPosition(), m_pOwner->GetLook(), 1, 1);
		m_nhasBulletCount--;

		CollisionInfo info;
		if (COLLISION_MGR->RayCastCollisionToCharacter(info, GetvPosition(), direction)) {
			if (info.m_fDistance < m_fRange) {
				// 최종 충돌 확인
				info.m_pHitObject->SetCollision(true);

				CCharacterObject* hitCharacter = static_cast<CCharacterObject*>(info.m_pHitObject);
				// Head Shot 판정
				if (info.m_HitParts == ChracterBoundingBoxParts::eHead) {
					cout << "헤드샷" << endl;
					hitCharacter->SetIsHeadHit(true);
					hitCharacter->DamagedCharacter(m_fDamage * 2.5f);
				}
				else {
					hitCharacter->DamagedCharacter(m_fDamage);

				}
				cout << "적중 후 체력 : " << hitCharacter->GetLife() << endl;
				
			}
		}

		CreateFireDirectionLine(direction);
	}
}

void CWeapon::Reloading()
{
	cout << "총알 재장전 완료" << endl;
	m_nhasBulletCount = m_nMaxhasBulletCount;
}

void CWeapon::Update(float fDeltaTime)
{
	CGameObject::Update(fDeltaTime);

	m_mtxParent = m_pOwner->GetSkinnedMesh()->GetFinalBoneMtx(m_nBoneIndex);
	m_mtxWorld = m_mtxLocal * m_mtxParent * m_pOwner->m_mtxWorld;
//	SetRotate(TWBAR_MGR->g_xmf3Rotate, true);
//	SetPosition(TWBAR_MGR->g_xmf3Offset, true);
}