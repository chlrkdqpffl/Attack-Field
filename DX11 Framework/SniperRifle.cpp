#include "stdafx.h"
#include "SniperRifle.h"

CSniperRifle::CSniperRifle(CCharacterObject* pOwner)
	: CWeapon(pOwner)
{
	XMFLOAT3 offsetPos, offsetRotate;

	// 테러리스트 캐릭터에 맞춰진 Offset
	if (pOwner->GetMeshTag() == MeshTag::eTerrorist) {
		m_nBoneIndex = 22;

		if (pOwner->GetCharacterID() == 0)
		{	// Player 자신에게만 적용되는 Offset
			offsetPos = XMFLOAT3(1.25f, 0.89f, -0.14f);
			offsetRotate = XMFLOAT3(180.f, 200.f, 30.f);
		}
		else
		{	// 타 캐릭터
			offsetPos = XMFLOAT3(1.135f, 1.175f, -0.04f);
			offsetRotate = XMFLOAT3(180.f, 210.f, 23.f);
		}
	}

	SetRotate(offsetRotate, true);
	SetPosition(offsetPos, true);

	// Data Initialize
	m_tagWeapon = WeaponTag::eSniperRifle;
	m_fDamage = 80.f;
	m_fRange = 1000.f;
	m_uiFireSpeed = 1500;

	m_fReCoil = 10.0f;
	m_fMaxPitchGap = 10.0f;

//	m_fReCoil = 0.0f;
//	m_fMaxPitchGap = 0.0f;

	m_nMaxhasBulletCount = 5;
	m_nhasBulletCount = m_nMaxhasBulletCount;
//	m_nMaxhasBulletCount = 100;
}

CSniperRifle::~CSniperRifle()
{
}

void CSniperRifle::CreateMesh(ID3D11Device *pd3dDevice)
{
	CFbxModelMesh* pMesh = new CFbxModelMesh(pd3dDevice, MeshTag::eSniperRifle);
	pMesh->Initialize(pd3dDevice);

	SetMesh(pMesh);

	// Sprite Image
	if (m_pOwner->GetCharacterID() == 0)
		m_pMuzzleSpirte = SPRITE_MGR->CreateSpriteImage(TextureTag::eGunExplosionPlayer, XMFLOAT3(0, 0, 0), true);
	else
		m_pMuzzleSpirte = SPRITE_MGR->CreateSpriteImage(TextureTag::eGunExplosionOtherPlayer, XMFLOAT3(0, 0, 0), true);

	m_pMuzzleSpirte->SetActive(false);
}

void CSniperRifle::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CShader();

	m_pShader->CreateShader(pd3dDevice, ShaderTag::eNormalTexture);
}

void CSniperRifle::CreateMaterial()
{
	m_pMaterial = new CMaterial();

	CTexture *pTexture = new CTexture(1, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);
	pTexture->SetTexture(0, TextureTag::eSniperRifleD);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWrapSS);

	m_pMaterial->SetTexture(pTexture);
}

void CSniperRifle::FireRecoil()
{
	m_nFireBulletCount++;

	if (false == m_bIsFire)
		m_fInitPitch = m_pOwner->GetPitch();

	// 1발부터 최대 반동
	if (0 < m_nFireBulletCount) {
		m_fNowRecoil += m_fCalcRecoil;
		m_pOwner->AddPitch(-m_fCalcRecoil);
		m_bIsFire = true;
	}
}

void CSniperRifle::UpdateRecoil(float fDeltaTime)
{
	if (m_bIsFire)
	{	// 발사 중지 시점	
		float gap = m_pOwner->GetPitch() - m_fInitPitch - m_fUserMovePitch;

		float returnSpeedFactor = abs(-gap * 20) / 100;					// 자연스럽게 속도 줄이기
		const float returnSpeed = 15.0f;

		if (returnSpeedFactor < 0.1f)
			returnSpeedFactor = 0.1f;

		if (gap < -0.0f)
			m_pOwner->AddPitch(returnSpeed * returnSpeedFactor * fDeltaTime);
		
		if (gap > 0.0f)
			m_bIsFire = false;
	}

	// ----- 반동력 계산 ----- //
	float recoilFactor = 1.0f;
	if (m_pOwner->GetIsCrouch())
		recoilFactor = 0.6f;
	else if (m_pOwner->IsMoving())
		recoilFactor = 1.2f;

	if (SCENE_MGR->g_pPlayer->GetIsZoom())
		recoilFactor *= 0.3f;

	m_fCalcRecoil = recoilFactor * m_fReCoil;

	m_fNowRecoil -= m_fReCoil * fDeltaTime;
	m_fNowRecoil = clamp(m_fNowRecoil, recoilFactor, 5.f);
}

void CSniperRifle::Update(float fDeltaTime)
{
	CWeapon::Update(fDeltaTime);

	XMVECTOR muzzlePosition;
	if (m_pOwner->GetCharacterID() == 0)
	//	muzzlePosition = GetvPosition() + (GetvRight() * TWBAR_MGR->g_xmf3Offset.x) + (GetvUp() * TWBAR_MGR->g_xmf3Offset.y) + (GetvLook() * TWBAR_MGR->g_xmf3Offset.z);
		muzzlePosition = GetvPosition() + (GetvRight() * -0.1f) + (GetvUp() * 1.92f) + (GetvLook() * -0.025f);
	else
		muzzlePosition = GetvPosition() + (GetvRight() * -0.015f) + (GetvUp() * 1.26f) + (GetvLook() * 0.02f);

	XMStoreFloat3(&m_f3MuzzlePosition, muzzlePosition);
	SPRITE_MGR->SetPosition(m_pMuzzleSpirte, m_f3MuzzlePosition);
}