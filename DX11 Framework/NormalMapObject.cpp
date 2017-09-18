#include "stdafx.h"
#include "NormalMapObject.h"


CNormalMapObject::CNormalMapObject()
{
}

CNormalMapObject::~CNormalMapObject()
{
}

void CNormalMapObject::CreateMaterial()
{
	CMaterialColors *pColors = new CMaterialColors();
	pColors->m_d3dxcDiffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pColors->m_d3dxcAmbient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pColors->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 16.0f);
	pColors->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_pMaterial = new CMaterial(pColors);

	CTexture *pTexture = new CTexture(2, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);

	pTexture ->SetTexture(0, TextureTag::eWallD);
	pTexture ->SetTexture(1, TextureTag::eWallND);
	pTexture ->SetSampler(0, STATEOBJ_MGR->g_pLinearWrapSS);

	m_pMaterial->SetTexture(pTexture);
}

void CNormalMapObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	CMesh* pMesh = new CNormalMapCubeMesh(pd3dDevice, 20, 20, 20);
//	CMesh* pMesh = new CCubeMeshTexturedIlluminated(pd3dDevice, 300, 300, 300);
	SetMesh(pMesh);
}

void CNormalMapObject::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CNormalDisplaceMapShader();
	m_pShader->CreateShader(pd3dDevice);
}