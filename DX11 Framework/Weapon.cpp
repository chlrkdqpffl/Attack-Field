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
		m_bIsFire = true;
		m_nhasBulletCount--;
		FireEffect();

		if(m_pOwner->GetCharacterID() == 0)		// 본인 캐릭터만 적용
			FireRecoil();
		
		XMVECTOR firePosOffset = GetvPosition() + (GetvRight() * -0.13f) + (GetvUp() * 0.05f) + (GetvLook() * -0.225f);
		COLLISION_MGR->CreateFireDirectionLine(firePosOffset, direction, m_fRange);		// 총 발사 레이 렌더링

#ifdef USE_SERVER
		if (SCENE_MGR->g_pPlayerCharacter->GetIsFire())	{
			cs_weapon* packet = reinterpret_cast<cs_weapon *>(SERVER_MGR->GetSendbuffer());
			packet->size = sizeof(cs_weapon);
			packet->type = CS_WEAPONE;
			XMStoreFloat3(&packet->position, firePosOffset);
			XMStoreFloat3(&packet->direction, direction);

			SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(packet));
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
	}
}

void CWeapon::Reloading()
{
	m_nhasBulletCount = m_nMaxhasBulletCount;
}

void CWeapon::FireEffect()
{
	SOUND_MGR->Play3DSound(SoundTag::eGunFire, m_pOwner->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);
	SOUND_MGR->Play3DSound(SoundTag::eShellsFall, m_pOwner->GetPosition(), XMFLOAT3(0, 0, 0), 0, 0);
	LIGHT_MGR->AddPointLight(m_f3MuzzlePosition, 8.0f, XMFLOAT3(0.9f, 0.9f, 0.6f));
	SPRITE_MGR->ActivationSprite(m_pMuzzleSpirte);
}

void CWeapon::FireRecoil()
{
	// ----- 총기 반동 ----- //
	m_nFireBulletCount++;

	if (0 < m_nFireBulletCount && m_nFireBulletCount < 4)
	{
		m_pOwner->AddPitch(RAND_FLOAT(-0.2f, 0.0f));
	}
	else if (4 <= m_nFireBulletCount && m_nFireBulletCount < 8) 
	{
		m_pOwner->AddPitch(RAND_FLOAT(-0.5f, -0.3f));
		SCENE_MGR->g_pPlayer->Rotate(0.0f, RAND_FLOAT(-0.5f, 0.5f));
	}
	else if (8 <= m_nFireBulletCount) 
	{
		if (m_fInitPitch - m_pOwner->GetPitch() < m_fMaxPitchGap) {		// 반동 최대치 전
			m_pOwner->AddPitch(RAND_FLOAT(-0.6f, -0.4f));
			SCENE_MGR->g_pPlayer->Rotate(0.0f, RAND_FLOAT(-1.0f, 1.0f));
		}
		else {															// 반동 최대치 도달
			m_pOwner->AddPitch(RAND_FLOAT(-0.3f, 0.3f));
			SCENE_MGR->g_pPlayer->Rotate(0.0f, RAND_FLOAT(-1.0f, 1.0f));
		}
	}
}

void CWeapon::UpdateRecoil(float fDeltaTime)
{
	if (m_bIsFire) 
	{	// 발사 중지 시점
		if (m_nFireBulletCount == 0)
		{		
			float gap = m_pOwner->GetPitch() - m_fInitPitch;
			float returnSpeedFactor = abs(-gap * 20) / 100;					// 자연스럽게 속도 줄이기
			const float returnSpeed = 80.0f;

			if (returnSpeedFactor< 0.1f)
				returnSpeedFactor = 0.1f;

			if (gap < -0.0f)
				m_pOwner->AddPitch(returnSpeed * returnSpeedFactor * fDeltaTime);
			else
				m_bIsFire = false;

			if (gap > 0.0f) 
				m_pOwner->SetPitch(m_fInitPitch);
		}
	}

	if (m_nFireBulletCount == 1)	// 최초 발사 시점
		m_fInitPitch = m_pOwner->GetPitch();
}

void CWeapon::Update(float fDeltaTime)
{
	CGameObject::Update(fDeltaTime);
	if (m_pOwner->GetCharacterID() == 0)		// 본인 캐릭터만 적용
		UpdateRecoil(fDeltaTime);

	m_mtxParent = m_pOwner->GetSkinnedMesh()->GetFinalBoneMtx(m_nBoneIndex);
	m_mtxWorld = m_mtxLocal * m_mtxParent * m_pOwner->m_mtxWorld;
//	SetRotate(TWBAR_MGR->g_xmf3Rotate, true);
//	SetPosition(TWBAR_MGR->g_xmf3Offset, true);
	
	XMVECTOR muzzlePosition;
	if (m_pOwner->GetCharacterID() == 0)
//		muzzlePosition = GetvPosition() + (GetvRight() * TWBAR_MGR->g_xmf3Offset.x) + (GetvUp() * TWBAR_MGR->g_xmf3Offset.y) + (GetvLook() * TWBAR_MGR->g_xmf3Offset.z);
		muzzlePosition = GetvPosition() + (GetvRight() * -1.85f) + (GetvUp() * 0.03f) + (GetvLook() * 0.2f);
	else
		muzzlePosition = GetvPosition() + (GetvRight() * -1.0f) + (GetvUp() * 0.03f) + (GetvLook() * 0.02f);

	XMStoreFloat3(&m_f3MuzzlePosition, muzzlePosition);
	SPRITE_MGR->SetPosition(m_pMuzzleSpirte, m_f3MuzzlePosition);
}