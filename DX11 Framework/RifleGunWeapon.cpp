#include "stdafx.h"
#include "RifleGunWeapon.h"

CRifleGunWeapon::CRifleGunWeapon()
{
}

CRifleGunWeapon::~CRifleGunWeapon()
{
}

void CRifleGunWeapon::CreateMesh(ID3D11Device *pd3dDevice)
{
	//	CMesh* pMesh = new CNormalMapCubeMesh(pd3dDevice, 200, 200, 200);
	CMesh* pMesh = new CCubeMeshTexturedIlluminated(pd3dDevice, 20, 20, 50);
	SetMesh(pMesh);
}

void CRifleGunWeapon::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CShader();

	m_pShader->CreateShader(pd3dDevice);
}

void CRifleGunWeapon::CreateMaterial(ID3D11Device *pd3dDevice)
{
	m_pMaterial = new CMaterial();

	//	CTexture *pNormalTexture = new CTexture(2, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);

	//	pNormalTexture->SetTexture(0, eTexture_WallDiffuse);
	//	pNormalTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	//	m_pMaterial->SetTexture(pNormalTexture);
}
