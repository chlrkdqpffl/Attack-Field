#include "stdafx.h"
#include "NormalMapObject.h"


CNormalMapObject::CNormalMapObject()
{
}

CNormalMapObject::~CNormalMapObject()
{
}


void CNormalMapObject::CreateMaterial(ID3D11Device *pd3dDevice)
{
	m_pMaterial = new CMaterial();

	CTexture *pNormalTexture = new CTexture(2, 1, PS_SLOT_TEXTURE_DIFFUSE, PS_SLOT_SAMPLER);

	pNormalTexture->SetTexture(0, eTexture_Stone);
	pNormalTexture->SetTexture(1, eTexture_StoneNormal);
	pNormalTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);
	
	m_pMaterial->SetTexture(pNormalTexture);
}

void CNormalMapObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	CMesh* pMesh = new CNormalMapCubeMesh(pd3dDevice, 300, 300, 300);
//	CMesh* pMesh = new CCubeMeshTexturedIlluminated(pd3dDevice, 300, 300, 300);
	SetMesh(pMesh);
}

void CNormalMapObject::CreateShader(ID3D11Device *pd3dDevice)
{
	// 배치 처리 고려해보기
	
	m_pShader = new CNormalMapShader(pd3dDevice);
//	m_pShader->CreateShader(pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0);
	m_pShader->CreateShader(pd3dDevice);
}