#include "stdafx.h"
#include "TerrainWater.h"


CTerrainWater::CTerrainWater(ID3D11Device *pd3dDevice, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMVECTOR d3dxvScale)
{
	m_nWidth = nWidth;
	m_nLength = nLength;

	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	XMStoreFloat3(&m_d3dxvScale, d3dxvScale);

	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;
	m_nMeshes = cxBlocks * czBlocks;
	m_ppMeshes = new CMesh*[m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++)	m_ppMeshes[i] = NULL;

	m_pWaterGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			m_pWaterGridMesh = new CWaterGridMesh(pd3dDevice, xStart, zStart, nBlockWidth, nBlockLength, d3dxvScale, NULL, D3D11_USAGE_DYNAMIC);
			SetMesh(m_pWaterGridMesh, x + (z*cxBlocks));
		}
	}

	XMStoreFloat4x4(&m_d3dxmtxTexture, XMMatrixIdentity());


	//-------------------------------------------------ºí·»µù
	m_pd3dBlendState = NULL;
	D3D11_BLEND_DESC d3dBlendStateDesc;
	ZeroMemory(&d3dBlendStateDesc, sizeof(D3D11_BLEND_DESC));
	d3dBlendStateDesc.AlphaToCoverageEnable = true;
	d3dBlendStateDesc.IndependentBlendEnable = false;
	d3dBlendStateDesc.RenderTarget[0].BlendEnable = true; //Æ®·ç
	d3dBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pd3dDevice->CreateBlendState(&d3dBlendStateDesc, &m_pd3dBlendState);
}

CTerrainWater::~CTerrainWater()
{
}

void CTerrainWater::Update(float fDeltaTime)
{
	// Update Wave Position
	m_pWaterGridMesh->Update(fDeltaTime);

	// Animate Texture Mtx
	static XMFLOAT2 d3dxOffset(0.0f, 0.0f);
	d3dxOffset.y += 0.000005f;
	d3dxOffset.x += 0.001f * sinf(4.0f * d3dxOffset.y);
	m_d3dxmtxTexture._41 += d3dxOffset.x;
	m_d3dxmtxTexture._42 += d3dxOffset.y;
}

void CTerrainWater::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CTexture::UpdateShaderVariable(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxTexture));

	CGameObject::Render(pd3dDeviceContext, pCamera);
}
