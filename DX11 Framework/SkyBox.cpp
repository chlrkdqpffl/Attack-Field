#include "stdafx.h"
#include "SkyBox.h"


CSkyBox::CSkyBox(ID3D11Device *pd3dDevice) : CGameObject(1)
{
	D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = false;
	d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	d3dDepthStencilDesc.StencilEnable = false;
	d3dDepthStencilDesc.StencilReadMask = 0xFF;
	d3dDepthStencilDesc.StencilWriteMask = 0xFF;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	pd3dDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dDepthStencilState);

#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(int) * 4;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbTextureIndex);

	ID3D11DeviceContext *pd3dDeviceContext;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_SKYBOX, 1, &m_pd3dcbTextureIndex);
	if (pd3dDeviceContext) pd3dDeviceContext->Release();
#endif
}

CSkyBox::~CSkyBox()
{
#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
	if (m_pd3dcbTextureIndex) m_pd3dcbTextureIndex->Release();
#endif
}

void CSkyBox::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	XMVECTOR d3dxvCameraPos = pCamera->GetPosition();
	XMFLOAT3 f3vCameraPos;
	XMStoreFloat3(&f3vCameraPos, d3dxvCameraPos);
	SetPosition(f3vCameraPos.x, f3vCameraPos.y, f3vCameraPos.z);
	Update(NULL);

	CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxWorld));

	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);

	CSkyBoxMesh *pSkyBoxMesh = (CSkyBoxMesh *)m_ppMeshes[0];
	pSkyBoxMesh->OnPrepareRender(pd3dDeviceContext);

#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
	m_pMaterial->m_pTexture->UpdateShaderVariable(pd3dDeviceContext);
#else
#ifdef _WITH_SKYBOX_TEXTURE_CUBE
	m_pMaterial->m_pTexture->UpdateShaderVariable(pd3dDeviceContext);
#else
	m_pMaterial->m_pTexture->UpdateSamplerShaderVariable(pd3dDeviceContext, 0, PS_SLOT_SAMPLER_SKYBOX);
#endif
#endif
	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 1);

	for (int i = 0; i < 6; i++)
	{
#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		pd3dDeviceContext->Map(m_pd3dcbTextureIndex, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		int *pfIndex = (int *)d3dMappedResource.pData;
		pfIndex[0] = pfIndex[1] = pfIndex[2] = pfIndex[3] = i;
		pd3dDeviceContext->Unmap(m_pd3dcbTextureIndex, 0);
#else
#ifndef _WITH_SKYBOX_TEXTURE_CUBE
		m_pMaterial->m_pTexture->UpdateTextureShaderVariable(pd3dDeviceContext, i, PS_SLOT_TEXTURE_SKYBOX);
#endif
#endif
		pd3dDeviceContext->DrawIndexed(4, 0, i * 4);
	}

	pd3dDeviceContext->OMSetDepthStencilState(NULL, 1);
}
