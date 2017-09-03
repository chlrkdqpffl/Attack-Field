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
	if (GetTickCount() - m_dwLastAttackTime >= m_uiFireSpeed / SCENE_MGR->g_nowScene->GetFrameSpeed()) {
		m_dwLastAttackTime = GetTickCount();
		SOUND_MGR->Play3DSound(SoundTag::eGunFire, m_pOwner->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);
		SOUND_MGR->Play3DSound(SoundTag::eShellsFall, m_pOwner->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);
		LIGHT_MGR->AddPointLight(m_f3MuzzlePosition, 8.0f, XMFLOAT3(0.9f, 0.9f, 0.6f));
		SPRITE_MGR->ActivationSprite(m_pMuzzleSpirte);
		m_nhasBulletCount--;

		XMVECTOR firePosOffset = GetvPosition() + (-1 * GetvRight() * (GetBoundingBox().Extents.z - 0.6f)) + (-1 * GetvLook() * 0.225) + (GetvUp() * 0.1f);
#ifdef USE_SERVER
		if (SCENE_MGR->g_pPlayerCharacter->GetIsFire())	{
			cs_weapon* packet = reinterpret_cast<cs_weapon *>(SERVER_MGR->GetSendbuffer());
			packet->size = sizeof(cs_weapon);
			packet->type = CS_WEAPONE;
			XMStoreFloat3(&packet->position, firePosOffset);
			XMStoreFloat3(&packet->direction, direction);

			SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(packet));
		}
		else {
			for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer())
				character->SetIsFire(false);
		}
		// Particle용 충돌체크 - 임시방편
		CollisionInfo info;
		if (COLLISION_MGR->RayCastCollisionToCharacter(info, firePosOffset, direction)) {
			if (info.m_fDistance < m_fRange) {
				XMVECTOR bloodOffset = firePosOffset;
				bloodOffset += direction * info.m_fDistance;

				// Head Shot 판정
				if (info.m_HitParts == ChracterBoundingBoxParts::eHead) {
					PARTICLE_MGR->CreateParticle(ParticleTag::eCopiousBleeding, bloodOffset);
				}
				else {
					PARTICLE_MGR->CreateParticle(ParticleTag::eBleeding, bloodOffset);
				}
			}
		}
		else if (COLLISION_MGR->RayCastCollision(info, firePosOffset, direction)) {
			if (info.m_fDistance < m_fRange) {
				XMVECTOR SparkOffset = firePosOffset;
				SparkOffset += direction * info.m_fDistance;
				PARTICLE_MGR->CreateParticle(ParticleTag::eSpark, SparkOffset);
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

				// Head Shot 판정
				if (info.m_HitParts == ChracterBoundingBoxParts::eHead) {
					hitCharacter->SetIsHeadHit(true);
					hitCharacter->DamagedCharacter(m_fDamage * 2.5f);
					PARTICLE_MGR->CreateParticle(ParticleTag::eCopiousBleeding, bloodOffset);
				}
				else {
					hitCharacter->DamagedCharacter(m_fDamage);

					PARTICLE_MGR->CreateParticle(ParticleTag::eBleeding, bloodOffset);
				}
			}
		}
		else if (COLLISION_MGR->RayCastCollision(info, firePosOffset, direction)) {
			if (info.m_fDistance < m_fRange) {
				XMVECTOR SparkOffset = firePosOffset;
				SparkOffset += direction * info.m_fDistance;
				PARTICLE_MGR->CreateParticle(ParticleTag::eSpark, SparkOffset);
			}
		}
#endif
//		COLLISION_MGR->CreateFireDirectionLine(firePosOffset, direction, m_fRange);
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
	
	XMVECTOR muzzlePosition;
	if (m_pOwner->GetCharacterID() == 0)
		//muzzlePosition = GetvPosition() + (GetvRight() * TWBAR_MGR->g_xmf3Offset.x) + (GetvUp() * TWBAR_MGR->g_xmf3Offset.y) + (GetvLook() * TWBAR_MGR->g_xmf3Offset.z);
		muzzlePosition = GetvPosition() + (GetvRight() * -1.15f) + (GetvUp() * 0.035f);
	else
		muzzlePosition = GetvPosition() + (GetvRight() * -1.0f) + (GetvUp() * 0.03f) + (GetvLook() * 0.02f);

	XMStoreFloat3(&m_f3MuzzlePosition, muzzlePosition);
	SPRITE_MGR->SetPosition(m_pMuzzleSpirte, m_f3MuzzlePosition);
}