#include "stdafx.h"
#include "LineObject.h"

CLineObject::CLineObject(XMFLOAT3 pos1, XMFLOAT3 pos2, DWORD lifeTime, XMVECTOR color)
	: m_dwLifeTime(lifeTime), m_StartPosition(pos1), m_EndPosition(pos2), m_Color(color)
{
	m_dwAgeTime = GetTickCount();
}

CLineObject::CLineObject(XMVECTOR pos1, XMVECTOR pos2, DWORD lifeTime, XMVECTOR color)
	: m_dwLifeTime(lifeTime), m_Color(color)
{
	XMStoreFloat3(&m_StartPosition, pos1);
	XMStoreFloat3(&m_EndPosition, pos2);
	this->CLineObject::CLineObject(m_StartPosition, m_EndPosition, lifeTime, color);
}

CLineObject::~CLineObject()
{
}

void CLineObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CreateMesh(pd3dDevice);
	CreateShader(pd3dDevice);
}

void CLineObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	CMesh* pMesh = new CLineMesh(pd3dDevice, m_StartPosition, m_EndPosition, m_Color);

	SetMesh(pMesh);
}

void CLineObject::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader* pShader = new CShader();
	pShader->CreateShader(pd3dDevice, ShaderTag::eColor);
	
	SetShader(pShader);
}

void CLineObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if (GetTickCount() - m_dwAgeTime <= m_dwLifeTime) {
		if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);
		CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &m_mtxWorld);

		RenderMesh(pd3dDeviceContext, pCamera);
	}
}