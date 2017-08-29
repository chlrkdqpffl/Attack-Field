#include "stdafx.h"
#include "SpriteImageObject.h"


CSpriteImageObject::CSpriteImageObject(TextureTag tag)
{
	m_tagTexture = tag;
}

CSpriteImageObject::~CSpriteImageObject()
{
}

void CSpriteImageObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	CMesh* pMesh = SPRITE_MGR->CloneSpriteMesh(m_tagTexture);
	SetMesh(pMesh);
}

void CSpriteImageObject::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CSpriteImageShader();
	m_pShader->CreateShader(pd3dDevice);
}

void CSpriteImageObject::CreateMaterial()
{
	m_pMaterial = new CMaterial();
	CTexture *pTexture = new CTexture(1, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);

	pTexture->SetTexture(0, m_tagTexture);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	m_pMaterial->SetTexture(pTexture);
}

void CSpriteImageObject::Update(float fDeltaTime)
{

}