#include "stdafx.h"
#include "ScreenShader.h"

CScreenShader::CScreenShader()
{
}

CScreenShader::~CScreenShader()
{
	SafeDelete(m_pScreenMesh);
	SafeDelete(m_pTexture);
}

void CScreenShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromFile(pd3dDevice, L"Shader HLSL File/TextureToScreen.hlsli", "VSTextureToScreen", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Shader HLSL File/TextureToScreen.hlsli", "PSTextureToScreen", "ps_5_0", &m_pd3dPixelShader);
}

void CScreenShader::CreateMesh(ID3D11Device *pd3dDevice)
{
	m_pScreenMesh = new CTextureToScreenRectMesh(pd3dDevice);

	m_pTexture = new CTexture(1, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);
	m_pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearClampSS);
}

void CScreenShader::SetTexture(ID3D11ShaderResourceView* pShaderResourceView)
{
	m_pTexture->SetTexture(0, pShaderResourceView);
}

void CScreenShader::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, nullptr, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, nullptr, 0);
	pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
}

void CScreenShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	m_pTexture->UpdateShaderVariable(pd3dDeviceContext);

	m_pScreenMesh->Render(pd3dDeviceContext);
}