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

	m_nMaxhasBulletCount = 30;
	m_nhasBulletCount = 30;

//	m_nMaxhasBulletCount = 100;
//	m_nhasBulletCount = 100;

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
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);
	
	m_pMaterial->SetTexture(pTexture);
}

void CRifleGunWeapon::Update(float fDeltaTime)
{
	CWeapon::Update(fDeltaTime);

	XMVECTOR muzzlePosition;
	if (m_pOwner->GetCharacterID() == 0)
		//		muzzlePosition = GetvPosition() + (GetvRight() * TWBAR_MGR->g_xmf3Offset.x) + (GetvUp() * TWBAR_MGR->g_xmf3Offset.y) + (GetvLook() * TWBAR_MGR->g_xmf3Offset.z);
		muzzlePosition = GetvPosition() + (GetvRight() * -1.85f) + (GetvUp() * 0.03f) + (GetvLook() * 0.2f);
	else
		muzzlePosition = GetvPosition() + (GetvRight() * -1.0f) + (GetvUp() * 0.03f) + (GetvLook() * 0.02f);

	XMStoreFloat3(&m_f3MuzzlePosition, muzzlePosition);
	SPRITE_MGR->SetPosition(m_pMuzzleSpirte, m_f3MuzzlePosition);
}