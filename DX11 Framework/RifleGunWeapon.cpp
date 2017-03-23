#include "stdafx.h"
#include "RifleGunWeapon.h"

CRifleGunWeapon::CRifleGunWeapon(ID3D11Device* pd3dDevice, CCharacterObject* pOwner) : CWeapon(pd3dDevice, pOwner)
{
	m_Type = WeaponData::Type::eRifle;


	XMFLOAT3 offsetPos, offsetRotate;

	if (pOwner->GetMeshTag() == MeshTag::eTerrorist) {
		m_nBoneIndex = 22;
		offsetPos = XMFLOAT3(-8.5f, 13.f, 0.5f);
		offsetRotate = XMFLOAT3(-35.f, -90.f, 100.f);
	}
	else if(pOwner->GetMeshTag() == MeshTag::ePolice)
	{

	}

	SetRotate(offsetRotate, true);
	SetPosition(offsetPos, true);
	m_mtxOffset = XMLoadFloat4x4(&m_pOwner->GetSkinnedMesh()->GetOffsetMtx(m_nBoneIndex));
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

	m_pShader->CreateShader(pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 );
//	m_pShader->CreateShader(pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TANGENT_ELEMENT | VERTEX_TEXTURE_ELEMENT_0);
}

void CRifleGunWeapon::CreateMaterial()
{
	m_pMaterial = new CMaterial();

	CTexture *pTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pTexture->SetTexture(0, TextureTag::eRifleD);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);
	
	m_pMaterial->SetTexture(pTexture);
}
