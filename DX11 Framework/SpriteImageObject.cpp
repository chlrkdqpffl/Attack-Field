#include "stdafx.h"
#include "SpriteImageObject.h"


CSpriteImageObject::CSpriteImageObject(CPlayer* player, TextureTag tag) 
	: CBillboardObject(player)
{
	m_tagTexture = tag;

	XMStoreFloat4x4(&m_mtxTexture, XMMatrixIdentity());
}

CSpriteImageObject::~CSpriteImageObject()
{

}

void CSpriteImageObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	//CMesh* pMesh = SPRITE_MGR->CloneSpriteMesh(m_tagTexture);
	//CMesh* pMesh = new CTextureToScreenRectMesh(pd3dDevice, 10, 10);
	CMesh* pMesh = new CSpriteImageMesh(pd3dDevice, 5, 5, 7);
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
	pTexture->CreateShaderVariables(STATEOBJ_MGR->g_pd3dDevice);
	m_pMaterial->SetTexture(pTexture);
}

void CSpriteImageObject::Update(float fDeltaTime)
{
	CBillboardObject::Update(fDeltaTime);

	static XMFLOAT3 f3Offset(0.0f, 0.0f, 0.0f);
//	f3Offset.x = TWBAR_MGR->g_xmf3Offset.x;
	f3Offset.x = 1.0f / 7.0f;
	//d3dxOffset.x = 0.0025f * sinf(4.0f * d3dxOffset.y);
	m_mtxTexture._41 += f3Offset.x;

	m_pMaterial->UpdateShaderVariable(STATEOBJ_MGR->g_pd3dImmediateDeviceContext, &XMLoadFloat4x4(&m_mtxTexture));
}