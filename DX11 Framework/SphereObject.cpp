#include "stdafx.h"
#include "SphereObject.h"


CSphereObject::CSphereObject()
{
}

CSphereObject::~CSphereObject()
{
}

void CSphereObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CreateMesh(pd3dDevice);
	CreateShader(pd3dDevice);
}

void CSphereObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	CMesh* pMesh = new CSphereMeshDiffused(pd3dDevice, 0.2f, 10, 10, XMVectorSet(1, 0, 0, 0));
	SetMesh(pMesh, 0);

	pMesh = new CSphereMeshDiffused(pd3dDevice, 1, 10, 10, XMVectorSet(1, 0, 0, 0));
	SetMesh(pMesh, 1);

	pMesh = new CSphereMeshDiffused(pd3dDevice, 5, 10, 10, XMVectorSet(1, 0, 0, 0));
	SetMesh(pMesh, 2);
}

void CSphereObject::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader* pShader = new CShader();
	pShader->CreateShader(pd3dDevice, ShaderTag::eColor);

	SetShader(pShader);
}

void CSphereObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);
	CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &m_mtxWorld);

	m_ppMeshes[m_nSizeType]->Render(pd3dDeviceContext);
}