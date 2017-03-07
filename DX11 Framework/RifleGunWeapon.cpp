#include "stdafx.h"
#include "RifleGunWeapon.h"

CRifleGunWeapon::CRifleGunWeapon()
{
	m_Type = WeaponData::Type::eRifle;
}

CRifleGunWeapon::~CRifleGunWeapon()
{
}

void CRifleGunWeapon::CreateMesh(ID3D11Device *pd3dDevice)
{
	CFbxModelMesh* pMesh = new CFbxModelMesh(pd3dDevice, RESOURCE_MGR->FindResourcePath(MeshData::eMesh_Rifle));
	pMesh->Initialize(pd3dDevice);
//	CMesh* pMesh = new CCubeMeshTexturedIlluminated(pd3dDevice, 200, 200, 500);
	SetMesh(pMesh);
}

void CRifleGunWeapon::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CShader();

	m_pShader->CreateShader(pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 );
}

void CRifleGunWeapon::CreateMaterial(ID3D11Device *pd3dDevice)
{
	m_pMaterial = new CMaterial();

	CTexture *pTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pTexture->SetTexture(0, eTexture_Rifle);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);
	
	m_pMaterial->SetTexture(pTexture);
}
