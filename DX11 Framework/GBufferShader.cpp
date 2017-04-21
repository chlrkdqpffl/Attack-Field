#include "stdafx.h"
#include "GBufferShader.h"

CGBufferShader::CGBufferShader(GBufferType type)
	:m_nType(type)
{
}

CGBufferShader::~CGBufferShader()
{
}

void CGBufferShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromFile(pd3dDevice, L"Shader HLSL File/GBuffer.hlsli", "VSGBuffer", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);


	switch (m_nType) {
	case GBufferType::eDepth:
		CreatePixelShaderFromFile(pd3dDevice, L"Shader HLSL File/GBuffer.hlsli", "PSGBuffer_LinearDepth", "ps_5_0", &m_pd3dPixelShader);
		break;
	case GBufferType::eDiffuse:
		CreatePixelShaderFromFile(pd3dDevice, L"Shader HLSL File/GBuffer.hlsli", "PSGBuffer_Diffuse", "ps_5_0", &m_pd3dPixelShader);
		break;
	case GBufferType::eNormal:
		CreatePixelShaderFromFile(pd3dDevice, L"Shader HLSL File/GBuffer.hlsli", "PSGBuffer_Normal", "ps_5_0", &m_pd3dPixelShader);
		break;
	case GBufferType::eSpec:
		CreatePixelShaderFromFile(pd3dDevice, L"Shader HLSL File/GBuffer.hlsli", "PSGBuffer_Spec", "ps_5_0", &m_pd3dPixelShader);
		break;
	default:
		assert(!"Not Initialized GBufferType Type");
		break;
	}
}