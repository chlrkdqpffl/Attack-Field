#include "stdafx.h"
#include "BoundingBoxShader.h"

CBoundingBoxShader::CBoundingBoxShader()
{
}

CBoundingBoxShader::~CBoundingBoxShader()
{
	ReleaseCOM(m_pd3dCollisionPixelShader);
}

void CBoundingBoxShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromFile(pd3dDevice, L"Shader HLSL File/BoundingBox.hlsli", "VSBoundingBox", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Shader HLSL File/BoundingBox.hlsli", "PSBoundingBox", "ps_5_0", &m_pd3dPixelShader);
	CreatePixelShaderFromFile(pd3dDevice, L"Shader HLSL File/BoundingBox.hlsli", "PSBoundingBoxCollision", "ps_5_0", &m_pd3dCollisionPixelShader);
}

void CBoundingBoxShader::OnPrepareSetting(ID3D11DeviceContext *pd3dDeviceContext, bool isCollision)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, nullptr, 0);
	if (isCollision)
		pd3dDeviceContext->PSSetShader(m_pd3dCollisionPixelShader, nullptr, 0);
	else 
		pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, nullptr, 0);
}