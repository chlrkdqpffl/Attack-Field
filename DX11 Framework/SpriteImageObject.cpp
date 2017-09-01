#include "stdafx.h"
#include "SpriteImageObject.h"


CSpriteImageObject::CSpriteImageObject(CPlayer* player, TextureTag tag) 
	: CBillboardObject(player)
{
	m_tagTexture = tag;

	m_nFrame = 0; //���罺������Ʈ�� ������
	m_nSizeX = 7; //���η� ��������Ʈ ����
	m_nSizeY = 1; //���η� ��������Ʈ ����

	m_fLifeTime = 0.9f; //����ð�(��������Ʈ �ӵ��� ����) ���� �������� �ӵ� ������
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