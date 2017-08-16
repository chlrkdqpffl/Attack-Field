#include "stdafx.h"
#include "Weapon.h"
#include "CharacterObject.h"
#include "protocol.h"

CWeapon::CWeapon(CCharacterObject* pOwner) 
	: m_pOwner(pOwner)
{
//	TWBAR_MGR->g_xmf3Offset = XMFLOAT3(1.0f, 1.4f, -0.05f);
//	TWBAR_MGR->g_xmf3Rotate = XMFLOAT3(-30.f, 125.f, 85.f);	
}

CWeapon::~CWeapon()
{
}

void CWeapon::Firing(XMVECTOR direction)
{
	if (GetTickCount() - m_dwLastAttackTime >= m_uiFireSpeed) {
		m_dwLastAttackTime = GetTickCount();
#ifndef DEVELOP_MODE
		//SOUND_MGR->Play3DSound(SoundTag::eFire, m_pOwner->GetPosition(), m_pOwner->GetLook(), 1, 1);	// �ʹ� �ò������� �ӽ� ����
#endif
		SOUND_MGR->Play3DSound(SoundTag::eShellsFall, m_pOwner->GetPosition(), m_pOwner->GetLook(), 1, 1);
		m_nhasBulletCount--;

		XMVECTOR firePosOffset = GetvPosition() + (-1 * GetvRight() * (GetBoundingBox().Extents.z - 0.6f)) + (-1 * GetvLook() * 0.225) + (GetvUp() * 0.1f);
	
#ifdef USE_SERVER
		cs_weapon* packet = reinterpret_cast<cs_weapon *>(SERVER_MGR->GetSendbuffer());
		packet->size = sizeof(cs_weapon);
		packet->type = CS_WEAPONE;
		XMStoreFloat3(&packet->position, firePosOffset);
		XMStoreFloat3(&packet->direction, direction);

		SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(packet));


		// Particle�� �浹üũ - �ӽù���
		CollisionInfo info;
		if (COLLISION_MGR->RayCastCollisionToCharacter(info, firePosOffset, direction)) {
			if (info.m_fDistance < m_fRange) {
				XMVECTOR bloodOffset = firePosOffset;
				bloodOffset += direction * info.m_fDistance;

				// Head Shot ����
				if (info.m_HitParts == ChracterBoundingBoxParts::eHead) {
			
					PARTICLE_MGR->CreateCopiousBleeding(bloodOffset);
				}
				else {
					PARTICLE_MGR->CreateBlood(bloodOffset);
				}
			}
		}
		else if (COLLISION_MGR->RayCastCollision(info, firePosOffset, direction)) {
			if (info.m_fDistance < m_fRange) {
				XMVECTOR SparkOffset = firePosOffset;
				SparkOffset += direction * info.m_fDistance;
				PARTICLE_MGR->CreateSpark(SparkOffset);
			}
		}

#else
		CollisionInfo info;
		if (COLLISION_MGR->RayCastCollisionToCharacter(info, firePosOffset, direction)) {
			if (info.m_fDistance < m_fRange) {
				info.m_pHitObject->SetCollision(true);

				CCharacterObject* hitCharacter = static_cast<CCharacterObject*>(info.m_pHitObject);

				XMVECTOR bloodOffset = firePosOffset;
				bloodOffset += direction * info.m_fDistance;

				// Head Shot ����
				if (info.m_HitParts == ChracterBoundingBoxParts::eHead) {
					hitCharacter->SetIsHeadHit(true);
					hitCharacter->DamagedCharacter(m_fDamage * 2.5f);

					PARTICLE_MGR->CreateCopiousBleeding(bloodOffset);
				}
				else {
					hitCharacter->DamagedCharacter(m_fDamage);

					PARTICLE_MGR->CreateBlood(bloodOffset);
				}
			}
		}
		else if (COLLISION_MGR->RayCastCollision(info, firePosOffset, direction)) {
			if (info.m_fDistance < m_fRange) {
				XMVECTOR SparkOffset = firePosOffset;
				SparkOffset += direction * info.m_fDistance;
				PARTICLE_MGR->CreateSpark(SparkOffset);
			}
		}

#endif
		//COLLISION_MGR->CreateFireDirectionLine(firePosOffset, direction, m_fRange);
	}
}


void CWeapon::Reloading()
{
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