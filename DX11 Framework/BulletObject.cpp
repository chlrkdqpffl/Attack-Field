#include "stdafx.h"
#include "BulletObject.h"


CBulletObject::CBulletObject()
{
	m_fSpeed = 1.0f;
}

CBulletObject::~CBulletObject()
{
}

void CBulletObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	CSphereMeshDiffused* pMesh = new CSphereMeshDiffused(pd3dDevice, 20, 10, 10);

	SetMesh(pMesh);
}

void CBulletObject::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CShader();

	m_pShader->CreateShader(pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_COLOR_ELEMENT);
}

void CBulletObject::CreateMaterial()
{
	m_pMaterial = new CMaterial();

	/*
	CTexture *pTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pTexture->SetTexture(0, TextureTag::eRifleD);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	m_pMaterial->SetTexture(pTexture);
	*/
}

void CBulletObject::Update(float fTimeElapsed)
{
	MoveForward(m_fSpeed * fTimeElapsed);
}