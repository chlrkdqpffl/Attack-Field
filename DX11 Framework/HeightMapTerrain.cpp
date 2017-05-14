#include "stdafx.h"
#include "HeightMapTerrain.h"


CHeightMapTerrain::CHeightMapTerrain(ID3D11Device *pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMVECTOR d3dxvScale) : CGameObject(0)
{
	m_nWidth = nWidth;
	m_nLength = nLength;

	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	XMStoreFloat3(&m_d3dxvScale, d3dxvScale);

	m_pHeightMap = new CHeightMap(pFileName, nWidth, nLength, d3dxvScale);

	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;
	m_vecMeshContainer.reserve(cxBlocks * czBlocks);
	
	CHeightMapGridMesh *pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, xStart, zStart, nBlockWidth, nBlockLength, d3dxvScale, m_pHeightMap);
			SetMesh(pHeightMapGridMesh, x + (z*cxBlocks));
		}
	}

#ifdef _WITH_TERRAIN_TEXTURE_ARRAY
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(int) * 4;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	int pnBuffers[4] = { 0, 0, 0, 0 };
	d3dBufferData.pSysMem = pnBuffers;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbTextureIndex);

	ID3D11DeviceContext *pd3dDeviceContext;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_TERRAIN, 1, &m_pd3dcbTextureIndex);
	if (pd3dDeviceContext) pd3dDeviceContext->Release();
#endif
}

CHeightMapTerrain::~CHeightMapTerrain(void)
{
#ifdef _WITH_TERRAIN_TEXTURE_ARRAY
	if (m_pd3dcbTextureIndex) m_pd3dcbTextureIndex->Release();
#endif
	if (m_pHeightMap) delete m_pHeightMap;
}

void CHeightMapTerrain::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);

	CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &m_mtxWorld);
	if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pd3dDeviceContext);

	RenderMesh(pd3dDeviceContext, pCamera);
}