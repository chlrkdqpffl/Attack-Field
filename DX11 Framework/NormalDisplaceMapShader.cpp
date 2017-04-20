#include "stdafx.h"
#include "NormalDisplaceMapShader.h"


CNormalDisplaceMapShader::CNormalDisplaceMapShader() : CShader()
{
}

CNormalDisplaceMapShader::~CNormalDisplaceMapShader()
{
}

void CNormalDisplaceMapShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromFile(pd3dDevice, L"Shader HLSL File/DisplacementMap.hlsli", "VS_NormalDisplace", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreateHullShaderFromFile(pd3dDevice, L"Shader HLSL File/DisplacementMap.hlsli", "HS_NormalDisplace", "hs_5_0", &m_pd3dHullShader);
	CreateDomainShaderFromFile(pd3dDevice, L"Shader HLSL File/DisplacementMap.hlsli", "DS_NormalDisplace", "ds_5_0", &m_pd3dDomainShader);
	CreatePixelShaderFromFile(pd3dDevice, L"Shader HLSL File/DisplacementMap.hlsli", "PS_NormalDisplace", "ps_5_0", &m_pd3dPixelShader);
}

void CNormalDisplaceMapShader::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, nullptr, 0);
	pd3dDeviceContext->HSSetShader(m_pd3dHullShader, nullptr, 0);
	pd3dDeviceContext->DSSetShader(m_pd3dDomainShader, nullptr, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, nullptr, 0);
}

void CNormalDisplaceMapShader::OnPostRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetInputLayout(nullptr);
	pd3dDeviceContext->VSSetShader(nullptr, nullptr, 0);
	pd3dDeviceContext->HSSetShader(nullptr, nullptr, 0);
	pd3dDeviceContext->DSSetShader(nullptr, nullptr, 0);
	pd3dDeviceContext->PSSetShader(nullptr, nullptr, 0);
}