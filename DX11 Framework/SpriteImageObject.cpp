#include "stdafx.h"
#include "SpriteImageObject.h"

ID3D11Buffer*	CSpriteImageObject::m_pSpriteInfoCB = NULL;

CSpriteImageObject::CSpriteImageObject(TextureTag tag, bool bIsInfinity)
	: m_bIsInfinity(bIsInfinity)
{
	m_tagTexture = tag;
	m_infoSprite = SPRITE_MGR->CloneSpriteInfo(tag);
}

CSpriteImageObject::~CSpriteImageObject()
{
}

void CSpriteImageObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	CMesh* pMesh = new CTextureToScreenRectMesh(pd3dDevice, m_infoSprite.m_fMeshSizeX, m_infoSprite.m_fMeshSizeY);
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

void CSpriteImageObject::CreateConstantBuffers()
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT4);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(STATEOBJ_MGR->g_pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &m_pSpriteInfoCB));

	DXUT_SetDebugName(m_pSpriteInfoCB, "Sprite Info CB");
}

void CSpriteImageObject::UpdateConstantBuffers(int frame, SpriteInfo info)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Map(m_pSpriteInfoCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMFLOAT4 *pSpriteInfo = (XMFLOAT4 *)d3dMappedResource.pData;
	memcpy(pSpriteInfo, &XMFLOAT4(frame, info.m_nSpriteCount.x, info.m_nSpriteCount.y, 0.0f), sizeof(XMFLOAT4));

	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Unmap(m_pSpriteInfoCB, 0);

	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_SPRITEINFO, 1, &m_pSpriteInfoCB);
}

void CSpriteImageObject::ReleaseConstantBuffers()
{
	ReleaseCOM(m_pSpriteInfoCB);
}

void CSpriteImageObject::Update(float fDeltaTime)
{
	CBillboardObject::Update(fDeltaTime);

	m_fTotalTime += fDeltaTime;
	m_nFrame = int((m_fTotalTime / m_infoSprite.m_fLifeTime ) * (m_infoSprite.m_nSpriteCount.x * m_infoSprite.m_nSpriteCount.y));
//	m_nFrame++;
//	if (m_nFrame > m_infoSprite.m_nSizeX * m_infoSprite.m_nSizeY)
//		m_nFrame = 0;

	if (m_bIsInfinity) {
		if (m_fTotalTime >= m_infoSprite.m_fLifeTime)
			m_fTotalTime -= m_infoSprite.m_fLifeTime;
	}
	else {
		if (m_fTotalTime >= m_infoSprite.m_fLifeTime)
			m_bActive = false;
	}
}

void CSpriteImageObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);
	if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pd3dDeviceContext);

	CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &m_mtxWorld);
	CSpriteImageObject::UpdateConstantBuffers(m_nFrame, SpriteInfo(m_infoSprite));

	RenderMesh(pd3dDeviceContext, pCamera);
}