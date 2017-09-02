#include "stdafx.h"
#include "BillboardObject.h"


CBillboardObject::CBillboardObject()
{
}

CBillboardObject::~CBillboardObject()
{
}

void CBillboardObject::SetLookAt(XMFLOAT3 cameraPos)
{
	XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 1.f);
	XMVECTOR look = XMLoadFloat3(&cameraPos) - GetvPosition();
	look = XMVector4NormalizeEst(look);

	XMVECTOR right = XMVector3Cross(look, up);
	right = XMVector4NormalizeEst(right);
	
	SetvRight(right);
	SetvUp(up);
	SetvLook(look);
}

void CBillboardObject::Update(float fDeltaTime)
{
	CGameObject::Update(fDeltaTime);

	SetLookAt(SCENE_MGR->g_pCamera->GetPosition());
}

void CBillboardObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);
	if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pd3dDeviceContext);

	CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &m_mtxWorld);

	ID3D11DepthStencilState* prevDSS; UINT prevStencil;
	pd3dDeviceContext->OMGetDepthStencilState(&prevDSS, &prevStencil);

//	pd3dDeviceContext->OMSetDepthStencilState(STATEOBJ_MGR->g_pNoDepthWritesDSS, 0);
	pd3dDeviceContext->OMSetBlendState(STATEOBJ_MGR->g_pTransparentBS, NULL, 0xffffffff);
	RenderMesh(pd3dDeviceContext, pCamera);
//	pd3dDeviceContext->OMSetDepthStencilState(prevDSS, prevStencil);
	pd3dDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}