#include "stdafx.h"
#include "RifleGunWeapon.h"

CRifleGunWeapon::CRifleGunWeapon(CCharacterObject* pOwner)
	: CWeapon(pOwner)
{
	XMFLOAT3 offsetPos, offsetRotate;

	// 테러리스트 캐릭터에 맞춰진 Offset
	if (pOwner->GetMeshTag() == MeshTag::eTerrorist) {
		m_nBoneIndex = 22;
		offsetPos		= XMFLOAT3(1.0f, 1.4f, -0.05f);
		offsetRotate	= XMFLOAT3(-30.f, 125.f, 85.f);
	}
	
	SetRotate(offsetRotate, true);
	SetPosition(offsetPos, true);

	// Data Initialize
	m_tagWeapon = WeaponTag::eRifle;
	m_fDamage = 30.f;
	m_fRange = 150.f;
	m_uiFireSpeed = 100;

	m_fReCoil = 0.3f;
	m_fMaxPitchGap = 5.0f;

	m_nMaxhasBulletCount = 30;
	m_nhasBulletCount = m_nMaxhasBulletCount;

//	m_nMaxhasBulletCount = 100;
}

CRifleGunWeapon::~CRifleGunWeapon()
{
}

void CRifleGunWeapon::CreateMesh(ID3D11Device *pd3dDevice)
{
	CFbxModelMesh* pMesh = new CFbxModelMesh(pd3dDevice, MeshTag::eRifle);
	pMesh->Initialize(pd3dDevice);

	SetMesh(pMesh);

	// Sprite Image
	if (m_pOwner->GetCharacterID() == 0)
		m_pMuzzleSpirte = SPRITE_MGR->CreateSpriteImage(TextureTag::eGunExplosionPlayer, XMFLOAT3(0, 0, 0), true);
	else 
		m_pMuzzleSpirte = SPRITE_MGR->CreateSpriteImage(TextureTag::eGunExplosionOtherPlayer, XMFLOAT3(0, 0, 0), true);
	
	m_pMuzzleSpirte->SetActive(false);
}

void CRifleGunWeapon::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CShader();

	m_pShader->CreateShader(pd3dDevice, ShaderTag::eNormalTexture);
}

void CRifleGunWeapon::CreateMaterial()
{
	m_pMaterial = new CMaterial();

	CTexture *pTexture = new CTexture(1, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);
	pTexture->SetTexture(0, TextureTag::eRifleD);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWrapSS);
	
	m_pMaterial->SetTexture(pTexture);
}

void CRifleGunWeapon::FireRecoil()
{
	m_bIsFire = true;
	m_nFireBulletCount++;

	if (0 < m_nFireBulletCount && m_nFireBulletCount < 4)
	{	// 0~3발까지 최소 반동
		m_fNowRecoil += m_fCalcRecoil;
		m_pOwner->AddPitch(RAND_FLOAT(-m_fCalcRecoil, 0.0f));
	}
	else if (4 <= m_nFireBulletCount && m_nFireBulletCount < 8)
	{	// 4~7발 추가 반동
		m_fNowRecoil += 10 * m_fReCoil * m_fCalcRecoil;
		m_pOwner->AddPitch(RAND_FLOAT(-m_fCalcRecoil * 2.0f, -m_fCalcRecoil));
		SCENE_MGR->g_pPlayer->Rotate(0.0f, RAND_FLOAT(-m_fCalcRecoil * 2, m_fCalcRecoil * 2));
	}
	else if (8 <= m_nFireBulletCount)
	{	//	8~ 발 최대 반동력
		m_fNowRecoil += 15 * m_fReCoil * m_fCalcRecoil;
		SCENE_MGR->g_pPlayer->Rotate(0.0f, RAND_FLOAT(-m_fCalcRecoil * 5, m_fCalcRecoil * 5));
		if (m_fInitPitch - m_pOwner->GetPitch() < m_fMaxPitchGap) {		// 반동 최대치 전
			m_pOwner->AddPitch(RAND_FLOAT(-m_fCalcRecoil * 2.5f, -m_fCalcRecoil * 1.5f));
		}
		else {															// 반동 최대치 도달
			m_pOwner->AddPitch(RAND_FLOAT(-m_fCalcRecoil * 1.5f, m_fCalcRecoil * 1.5f));
		}
	}
}

void CRifleGunWeapon::UpdateRecoil(float fDeltaTime)
{
	if (m_bIsFire)
	{	// 발사 중지 시점
		if (m_nFireBulletCount == 0)
		{
			float gap = m_pOwner->GetPitch() - m_fInitPitch - m_fUserMovePitch;
			float returnSpeedFactor = abs(-gap * 20) / 100;		// 자연스럽게 속도 줄이기
			const float returnSpeed = 50.0;

			if (returnSpeedFactor< 0.1f)
				returnSpeedFactor = 0.1f;

			if (gap < -0.0f)
				m_pOwner->AddPitch(returnSpeed * returnSpeedFactor * fDeltaTime);
			else
				m_bIsFire = false;

			if (gap > 0.0f)
				m_pOwner->SetPitch(m_pOwner->GetPitch());
		}
	}

	if (m_nFireBulletCount == 1) 	// 최초 발사 시점
		m_fInitPitch = m_pOwner->GetPitch();
	
	// ----- 반동력 계산 ----- //
	float recoilFactor = 1.0f;
	if (m_pOwner->GetIsCrouch())
		recoilFactor = 0.6f;
	else if (m_pOwner->IsMoving())
		recoilFactor = 1.5f;
	if (m_pOwner->GetIsReload())
		recoilFactor = 3.0f;

	m_fCalcRecoil = recoilFactor * m_fReCoil;

	m_fNowRecoil -= 10 * m_fReCoil * fDeltaTime;
	m_fNowRecoil = clamp(m_fNowRecoil, recoilFactor, 3.f);
}

void CRifleGunWeapon::Update(float fDeltaTime)
{
	CWeapon::Update(fDeltaTime);
	
	XMVECTOR muzzlePosition;
	if (m_pOwner->GetCharacterID() == 0)
		//muzzlePosition = GetvPosition() + (GetvRight() * TWBAR_MGR->g_xmf3Offset.x) + (GetvUp() * TWBAR_MGR->g_xmf3Offset.y) + (GetvLook() * TWBAR_MGR->g_xmf3Offset.z);
		muzzlePosition = GetvPosition() + (GetvRight() * -2.0f) + (GetvUp() * 0.0f) + (GetvLook() * 0.2f);
	else
		muzzlePosition = GetvPosition() + (GetvRight() * -1.0f) + (GetvUp() * 0.03f) + (GetvLook() * 0.02f);

	XMStoreFloat3(&m_f3MuzzlePosition, muzzlePosition);
	SPRITE_MGR->SetPosition(m_pMuzzleSpirte, m_f3MuzzlePosition);
}