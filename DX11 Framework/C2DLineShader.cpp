#include "stdafx.h"
#include "C2DLineShader.h"


C2DLineShader::C2DLineShader()
{
}

C2DLineShader::~C2DLineShader()
{
}

void C2DLineShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromFile(pd3dDevice, L"../Shader HLSL File/RenderToScreen.hlsli", "VSDiffusedColor", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"../Shader HLSL File/RenderToScreen.hlsli", "PSDiffusedColor", "ps_5_0", &m_pd3dPixelShader);
}