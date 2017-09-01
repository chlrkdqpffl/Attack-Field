#include "stdafx.h"
#include "SpriteImageObject.h"


CSpriteImageObject::CSpriteImageObject(CPlayer* player, TextureTag tag) 
	: CBillboardObject(player)
{
	m_tagTexture = tag;

	m_nFrame = 0; //현재스프라이트의 프레임
	m_nSizeX = 7; //가로로 스프라이트 갯수
	m_nSizeY = 1; //세로로 스프라이트 갯수

	m_fLifeTime = 0.9f; //생명시간(스프라이트 속도와 연관) 값이 낮을수록 속도 빨라짐
	m_fTotalTime = 0.0f;

}

CSpriteImageObject::~CSpriteImageObject()
{
}

void CSpriteImageObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	//CMesh* pMesh = SPRITE_MGR->CloneSpriteMesh(m_tagTexture);
	CMesh* pMesh = new CTextureToScreenRectMesh(pd3dDevice, 5, 5);
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

	m_fTotalTime += fDeltaTime;
	m_nFrame = int((m_fTotalTime / m_fLifeTime) * (m_nSizeX * m_nSizeY));

	if (m_fTotalTime >= m_fLifeTime)
		m_fTotalTime -= m_fLifeTime;
	

	TWBAR_MGR->g_xmf4TestVariable.x = m_nFrame;
	TWBAR_MGR->g_xmf4TestVariable.y = m_nSizeX;
	TWBAR_MGR->g_xmf4TestVariable.z = m_nSizeY;
}