#include "stdafx.h"
#include "RifleGunWeapon.h"

CRifleGunWeapon::CRifleGunWeapon(CCharacterObject* pOwner) : CWeapon(pOwner)
{
	XMFLOAT3 offsetPos, offsetRotate;

	if (pOwner->GetMeshTag() == MeshTag::eTerrorist) {
		m_nBoneIndex = 22;
		offsetPos		= XMFLOAT3(1.0f, 1.4f, -0.05f);
		offsetRotate	= XMFLOAT3(-30.f, 125.f, 85.f);
	}

	SetRotate(offsetRotate, true);
	SetPosition(offsetPos, true);

	// Data Initialize
	m_Type = WeaponData::Type::eRifle;
	m_fDamage = 30.f;
	m_fRange = 1000.f;
	m_uiFireSpeed = 100;

	m_nMaxhasBulletCount = 30;
	m_nhasBulletCount = 30;

}

CRifleGunWeapon::~CRifleGunWeapon()
{
}

void CRifleGunWeapon::CreateMesh(ID3D11Device *pd3dDevice)
{
	CFbxModelMesh* pMesh = new CFbxModelMesh(pd3dDevice, MeshTag::eRifle);
	pMesh->Initialize(pd3dDevice);

	SetMesh(pMesh);
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