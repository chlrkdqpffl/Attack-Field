#include "stdafx.h"
#include "SpriteImageShader.h"


CSpriteImageShader::CSpriteImageShader()
{
}

CSpriteImageShader::~CSpriteImageShader()
{
}

void CSpriteImageShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromFile(pd3dDevice, L"../Shader HLSL File/SpriteImage.hlsli", "VSSpriteImage", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"../Shader HLSL File/SpriteImage.hlsli", "PSSpriteImage", "ps_5_0", &m_pd3dPixelShader);
}