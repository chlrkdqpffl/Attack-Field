#include "stdafx.h"
#include "LineObject.h"


CLineObject::CLineObject(XMFLOAT3 pos1, XMFLOAT3 pos2, DWORD lifeTime) 
	: m_dwLifeTime(lifeTime), m_StartPosition(pos1), m_EndPosition(pos2)
{
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
	CMesh* pMesh = new CLineMesh(pd3dDevice, m_StartPosition, m_EndPosition, XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));

	SetMesh(pMesh);
}

void CLineObject::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader* pShader = new CShader();
	pShader->CreateShader(pd3dDevice, ShaderTag::eColor);

	SetShader(pShader);
}