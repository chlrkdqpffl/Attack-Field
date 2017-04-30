#include "stdafx.h"
#include "RifleBullet.h"


CRifleBullet::CRifleBullet(float fRange, float fSpeed)
	: CBulletObject(fRange, fSpeed)
{
}

CRifleBullet::~CRifleBullet()
{
}

void CRifleBullet::CreateMesh(ID3D11Device *pd3dDevice)
{
	CSphereMeshDiffused* pMesh = new CSphereMeshDiffused(pd3dDevice, 0.2f, 3, 3);

	SetMesh(pMesh);
}

void CRifleBullet::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CShader();

	m_pShader->CreateShader(pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_COLOR_ELEMENT);
}

void CRifleBullet::CreateMaterial()
{
	m_pMaterial = new CMaterial();
}

void CRifleBullet::Update(float fTimeElapsed)
{
	CBulletObject::Update(fTimeElapsed);
}