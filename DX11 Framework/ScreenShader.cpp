#include "stdafx.h"
#include "ScreenShader.h"

CScreenShader::CScreenShader()
{
}

CScreenShader::~CScreenShader()
{
}

void CScreenShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromFile(pd3dDevice, L"Shader HLSL File/TextureToScreen.hlsli", "VSTextureToScreen", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Shader HLSL File/TextureToScreen.hlsli", "PSTextureToScreen", "ps_5_0", &m_pd3dPixelShader);
}

void CScreenShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CShader::OnPrepareRender(pd3dDeviceContext);


}