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
	m_fRange = 300.f;
	m_uiFireSpeed = 2000;
//	m_uiFireSpeed = 100;

//	m_nMaxhasBulletCount = 5;
//	m_nhasBulletCount = 5;
	m_nMaxhasBulletCount = 100;
	m_nhasBulletCount = 100;
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