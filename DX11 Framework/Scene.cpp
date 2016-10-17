//-----------------------------------------------------------------------------
// File: Scene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"
#include "resource.h"

CScene::CScene()
{
	m_ppObjects = NULL;
	m_nObjects = 0;

	m_ppObjectShaders = NULL;
	m_nObjectShaders = 0;

	m_ppInstancingShaders = NULL;
	m_nInstancingShaders = 0;

	m_pLights = NULL;
	m_pd3dcbLights = NULL;

	m_pCamera = NULL;
	m_pSelectedObject = NULL;
}

CScene::~CScene()
{
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		m_pSelectedObject = PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONUP:
		break;
	default:
		break;
	}
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

void CScene::OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex)
{
#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
	_TCHAR ppstrTextureNames[6][128];
	_stprintf_s(ppstrTextureNames[0], _T("../Assets/Image/SkyBox/SkyBox_Front_%d.dds"), nIndex, 128);
	_stprintf_s(ppstrTextureNames[1], _T("../Assets/Image/SkyBox/SkyBox_Back_%d.dds"), nIndex, 128);
	_stprintf_s(ppstrTextureNames[2], _T("../Assets/Image/SkyBox/SkyBox_Left_%d.dds"), nIndex, 128);
	_stprintf_s(ppstrTextureNames[3], _T("../Assets/Image/SkyBox/SkyBox_Right_%d.dds"), nIndex, 128);
	_stprintf_s(ppstrTextureNames[4], _T("../Assets/Image/SkyBox/SkyBox_Top_%d.dds"), nIndex, 128);
	_stprintf_s(ppstrTextureNames[5], _T("../Assets/Image/SkyBox/SkyBox_Bottom_%d.dds"), nIndex, 128);
	ID3D11ShaderResourceView *pd3dsrvTexture = CTexture::CreateTexture2DArraySRV(pd3dDevice, ppstrTextureNames, 6);
	pMaterial->m_pTexture->SetTexture(0, pd3dsrvTexture); pd3dsrvTexture->Release();
#else
#ifdef _WITH_SKYBOX_TEXTURE_CUBE
	_TCHAR pstrTextureNames[128];
	_stprintf_s(pstrTextureNames, _T("../Assets/Image/SkyBox/SkyBox_%d.dds"), nIndex, 128);
	ID3D11ShaderResourceView *pd3dsrvTexture = NULL;
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, pstrTextureNames, NULL, NULL, &pd3dsrvTexture, NULL);
	pMaterial->m_pTexture->SetTexture(0, pd3dsrvTexture); pd3dsrvTexture->Release();
#else
	_TCHAR pstrTextureName[80];
	_stprintf_s(pstrTextureName, _T("../Assets/Image/SkyBox/SkyBox_Front_%d.jpg"), nIndex, 80);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, pstrTextureName, NULL, NULL, &pd3dsrvTexture, NULL);
	pMaterial->m_pTexture->SetTexture(0, pd3dsrvTexture); pd3dsrvTexture->Release();
	_stprintf_s(pstrTextureName, _T("../Assets/Image/SkyBox/SkyBox_Back_%d.jpg"), nIndex, 80);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, pstrTextureName, NULL, NULL, &pd3dsrvTexture, NULL);
	pMaterial->m_pTexture->SetTexture(1, pd3dsrvTexture); pd3dsrvTexture->Release();
	_stprintf_s(pstrTextureName, _T("../Assets/Image/SkyBox/SkyBox_Left_%d.jpg"), nIndex, 80);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, pstrTextureName, NULL, NULL, &pd3dsrvTexture, NULL);
	pMaterial->m_pTexture->SetTexture(2, pd3dsrvTexture); pd3dsrvTexture->Release();
	_stprintf_s(pstrTextureName, _T("../Assets/Image/SkyBox/SkyBox_Right_%d.jpg"), nIndex, 80);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, pstrTextureName, NULL, NULL, &pd3dsrvTexture, NULL);
	pMaterial->m_pTexture->SetTexture(3, pd3dsrvTexture); pd3dsrvTexture->Release();
	_stprintf_s(pstrTextureName, _T("../Assets/Image/SkyBox/SkyBox_Top_%d.jpg"), nIndex, 80);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, pstrTextureName, NULL, NULL, &pd3dsrvTexture, NULL);
	pMaterial->m_pTexture->SetTexture(4, pd3dsrvTexture); pd3dsrvTexture->Release();
	_stprintf_s(pstrTextureName, _T("../Assets/Image/SkyBox/SkyBox_Bottom_%d.jpg"), nIndex, 80);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, pstrTextureName, NULL, NULL, &pd3dsrvTexture, NULL);
	pMaterial->m_pTexture->SetTexture(5, pd3dsrvTexture); pd3dsrvTexture->Release();
#endif
#endif
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
	CTexture *pSkyboxTexture = new CTexture(1, 1, PS_SLOT_TEXTURE_SKYBOX, PS_SLOT_SAMPLER_SKYBOX);
#else
#ifdef _WITH_SKYBOX_TEXTURE_CUBE
	CTexture *pSkyboxTexture = new CTexture(1, 1, PS_SLOT_TEXTURE_SKYBOX, PS_SLOT_SAMPLER_SKYBOX);
#else
	CTexture *pSkyboxTexture = new CTexture(6, 1, PS_SLOT_TEXTURE_SKYBOX, PS_SLOT_SAMPLER_SKYBOX);
#endif
#endif
	pSkyboxTexture->SetSampler(0, pd3dSamplerState);
	pd3dSamplerState->Release();

	CMaterial *pSkyboxMaterial = new CMaterial(NULL);
	pSkyboxMaterial->SetTexture(pSkyboxTexture);
	OnChangeSkyBoxTextures(pd3dDevice, pSkyboxMaterial, 0);

	CSkyBoxMesh *pSkyBoxMesh = new CSkyBoxMesh(pd3dDevice, 20.0f, 20.0f, 20.0f);
	CSkyBox *pSkyBox = new CSkyBox(pd3dDevice);
	pSkyBox->SetMesh(pSkyBoxMesh, 0);
	pSkyBox->SetMaterial(pSkyboxMaterial);

	CShader *pSkyBoxShader = new CSkyBoxShader();
	pSkyBoxShader->CreateShader(pd3dDevice);
	pSkyBox->SetShader(pSkyBoxShader);

	ID3D11SamplerState *pd3dBaseSamplerState = NULL;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dBaseSamplerState);

	ID3D11SamplerState *pd3dDetailSamplerState = NULL;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dDetailSamplerState);

	CTexture *pTerrainTexture = new CTexture(2, 2, PS_SLOT_TEXTURE_TERRAIN, PS_SLOT_SAMPLER_TERRAIN);

	ID3D11ShaderResourceView *pd3dsrvBaseTexture = NULL;
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Terrain/Base_Texture.jpg"), NULL, NULL, &pd3dsrvBaseTexture, NULL);
	pTerrainTexture->SetTexture(0, pd3dsrvBaseTexture);
	pTerrainTexture->SetSampler(0, pd3dBaseSamplerState);
	pd3dsrvBaseTexture->Release();
	pd3dBaseSamplerState->Release();

	ID3D11ShaderResourceView *pd3dsrvDetailTexture = NULL;
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Terrain/Detail_Texture_7.jpg"), NULL, NULL, &pd3dsrvDetailTexture, NULL);
	pTerrainTexture->SetTexture(1, pd3dsrvDetailTexture);
	pTerrainTexture->SetSampler(1, pd3dDetailSamplerState);
	pd3dsrvDetailTexture->Release();
	pd3dDetailSamplerState->Release();

	CMaterialColors *pTerrainColors = new CMaterialColors();
	pTerrainColors->m_d3dxcDiffuse = XMFLOAT4(0.8f, 1.0f, 0.2f, 1.0f);
	pTerrainColors->m_d3dxcAmbient = XMFLOAT4(0.1f, 0.3f, 0.1f, 1.0f);
	pTerrainColors->m_d3dxcSpecular= XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pTerrainColors->m_d3dxcEmissive= XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	CMaterial *pTerrainMaterial = new CMaterial(pTerrainColors);
	pTerrainMaterial->SetTexture(pTerrainTexture);

	XMVECTOR d3dxvScale = XMVectorSet(8.0f, 2.0f, 8.0f, 0.0f);
#ifdef _WITH_TERRAIN_PARTITION
	CHeightMapTerrain *pTerrain = new CHeightMapTerrain(pd3dDevice, _T("../Assets/Image/Terrain/HeightMap.raw"), 257, 257, 17, 17, d3dxvScale);
#else
	CHeightMapTerrain *pTerrain = new CHeightMapTerrain(pd3dDevice, _T("Image/Terrain/HeightMap.raw"), 257, 257, 257, 257, d3dxvScale);
#endif
	pTerrain->SetMaterial(pTerrainMaterial);

	CShader *pTerrainShader = new CTerrainShader();
	pTerrainShader->CreateShader(pd3dDevice);
	pTerrain->SetShader(pTerrainShader);

	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dBaseSamplerState);

	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dDetailSamplerState);

	pd3dsrvBaseTexture = NULL;
	pd3dsrvDetailTexture = NULL;
	CTexture *pTerrainWaterTexture = new CTexture(2, 2, PS_SLOT_TEXTURE, PS_SLOT_SAMPLER);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Terrain/14_-water-texture.jpg."), NULL, NULL, &pd3dsrvBaseTexture, NULL);
	pTerrainWaterTexture->SetTexture(0, pd3dsrvBaseTexture);
	pTerrainWaterTexture->SetSampler(0, pd3dBaseSamplerState);
	pd3dsrvBaseTexture->Release();
	pd3dBaseSamplerState->Release();

	//D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Terrain/Detail_Texture_1.jpg"), NULL, NULL, &pd3dsrvDetailTexture, NULL);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Terrain/water-detail_texture_alpha.jpg"), NULL, NULL, &pd3dsrvDetailTexture, NULL);
	pTerrainWaterTexture->SetTexture(1, pd3dsrvDetailTexture);
	pTerrainWaterTexture->SetSampler(1, pd3dDetailSamplerState);
	pd3dsrvDetailTexture->Release();
	pd3dDetailSamplerState->Release();


	CMaterialColors *pWaterColors = new CMaterialColors();
	pTerrainColors->m_d3dxcDiffuse = XMFLOAT4(0.8f, 1.0f, 0.2f, 1.0f);
	pTerrainColors->m_d3dxcAmbient = XMFLOAT4(0.1f, 0.3f, 0.1f, 1.0f);
	pTerrainColors->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pTerrainColors->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	CMaterial *pTerrainWaterMaterial = new CMaterial(pWaterColors);
	pTerrainWaterMaterial->SetTexture(pTerrainWaterTexture);

	CTerrainWater *pTerrainWater = new CTerrainWater(pd3dDevice, 257, 257, 17, 17, d3dxvScale);
	pTerrainWater->SetMaterial(pTerrainWaterMaterial);
	pTerrainWater->SetPosition(0.0f, 80.0f, 0.0f);

	CShader *pTerrainWaterShader = new CWaterShader();
	pTerrainWaterShader->CreateShader(pd3dDevice);
	pTerrainWater->SetShader(pTerrainWaterShader);

	m_nObjects = 3;
	m_ppObjects = new CGameObject*[m_nObjects];

	m_ppObjects[0] = pSkyBox;
	m_ppObjects[1] = pTerrain;
	m_ppObjects[2] = pTerrainWater;

	//Instancing
	CMaterial *pInstancingMaterials[3];

	CMaterialColors *pRedColor = new CMaterialColors();
	pRedColor->m_d3dxcDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	pRedColor->m_d3dxcAmbient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	pRedColor->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pRedColor->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pInstancingMaterials[0] = new CMaterial(pRedColor);

	CMaterialColors *pGreenColor = new CMaterialColors();
	pGreenColor->m_d3dxcDiffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	pGreenColor->m_d3dxcAmbient = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	pGreenColor->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pGreenColor->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pInstancingMaterials[1] = new CMaterial(pGreenColor);

	CMaterialColors *pBlueColor = new CMaterialColors();
	pBlueColor->m_d3dxcDiffuse =  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	pBlueColor->m_d3dxcAmbient =  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	pBlueColor->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pBlueColor->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pInstancingMaterials[2] = new CMaterial(pBlueColor);

	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11ShaderResourceView *pd3dsrvTexture = NULL;
	CTexture *pStoneTexture = new CTexture(1, 1, PS_SLOT_TEXTURE, PS_SLOT_SAMPLER);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Miscellaneous/Stone01.jpg"), NULL, NULL, &pd3dsrvTexture, NULL);
	pStoneTexture->SetTexture(0, pd3dsrvTexture);
	pStoneTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	pInstancingMaterials[0]->SetTexture(pStoneTexture);

	CTexture *pBrickTexture = new CTexture(1, 1, PS_SLOT_TEXTURE, PS_SLOT_SAMPLER);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Miscellaneous/Brick01.jpg"), NULL, NULL, &pd3dsrvTexture, NULL);
	pBrickTexture->SetTexture(0, pd3dsrvTexture);
	pBrickTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	pInstancingMaterials[1]->SetTexture(pBrickTexture);

	CTexture *pWoodTexture = new CTexture(1, 1, PS_SLOT_TEXTURE, PS_SLOT_SAMPLER);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Miscellaneous/Wood01.jpg"), NULL, NULL, &pd3dsrvTexture, NULL);
	pWoodTexture->SetTexture(0, pd3dsrvTexture);
	pWoodTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	pInstancingMaterials[2]->SetTexture(pWoodTexture);


	pd3dSamplerState->Release();

	float fSize = 24.0f;

	CMesh *pMeshes[6];
	pMeshes[0] = new CCubeMeshTexturedIlluminated(pd3dDevice, fSize, fSize, fSize);
	pMeshes[1] = new CSphereMeshTexturedIlluminated(pd3dDevice, fSize, 20, 20);
	pMeshes[2] = new CCubeMeshTexturedIlluminated(pd3dDevice, fSize, fSize, fSize);
	pMeshes[3] = new CSphereMeshTexturedIlluminated(pd3dDevice, fSize, 20, 20);
	pMeshes[4] = new CCubeMeshTexturedIlluminated(pd3dDevice, fSize, fSize, fSize);
	pMeshes[5] = new CSphereMeshTexturedIlluminated(pd3dDevice, fSize, 20, 20);

	float fxPitch = fSize * 3.5f;
	float fyPitch = fSize * 3.5f;
	float fzPitch = fSize * 3.5f;

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	int xObjects = int(fTerrainWidth / (fxPitch * 3.0f)), zObjects = int(fTerrainLength / (fzPitch * 3.0f));

	m_nInstancingShaders = 6;
	m_ppInstancingShaders = new CInstancedObjectsShader*[m_nInstancingShaders];

	for (int k = 0; k < m_nInstancingShaders; k++)
	{
		m_ppInstancingShaders[k] = new CInstancedObjectsShader(xObjects * zObjects);
		m_ppInstancingShaders[k]->SetMesh(pMeshes[k]);
		m_ppInstancingShaders[k]->SetMaterial(pInstancingMaterials[k % 3]);
		m_ppInstancingShaders[k]->BuildObjects(pd3dDevice, NULL);
		m_ppInstancingShaders[k]->CreateShader(pd3dDevice);
	}

	XMVECTOR d3dxvRotateAxis;
	CRotatingObject *pRotatingObject = NULL;

	for (int k = 0; k < 3; k++)
	{
		float xStart = k * fxPitch;
		float zStart = k * fzPitch;
		for (int i = 0, x = 0; x < xObjects; x++)
		{
			float xPosition = xStart + (x * fxPitch * 3.0f);
			for (int z = 0; z < zObjects; z++, i++)
			{
				float zPosition = zStart + (z * fzPitch * 3.0f);
				float fHeight = pTerrain->GetHeight(xPosition, zPosition);

				pRotatingObject = new CRotatingObject();
				pRotatingObject->SetMesh(pMeshes[k * 2]);
				pRotatingObject->SetPosition(xPosition, fHeight + 6.0f, zPosition);
				XMVECTOR d3dxvSurfaceNormal = pTerrain->GetNormal(xPosition, zPosition);
				d3dxvRotateAxis = XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), d3dxvSurfaceNormal);
				float fAngle = acos(XMVectorGetX(XMVector3Dot(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), d3dxvSurfaceNormal)));
				if (!XMVector3Equal(d3dxvRotateAxis, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)))
				pRotatingObject->Rotate(&d3dxvRotateAxis, XMConvertToDegrees(fAngle));
				pRotatingObject->SetRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
				pRotatingObject->SetRotationSpeed(18.0f * (i % 10) + 10.0f);
				m_ppInstancingShaders[k * 2]->AddObject(pRotatingObject);

				pRotatingObject = new CRotatingObject();
				pRotatingObject->SetMesh(pMeshes[k * 2 + 1]);
				pRotatingObject->SetPosition(xPosition, fHeight + (fyPitch * 4), zPosition);
				pRotatingObject->SetRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
				pRotatingObject->SetRotationSpeed(36.0f * (i % 10) + 36.0f);
				m_ppInstancingShaders[k * 2 + 1]->AddObject(pRotatingObject);
			}
		}
	}


	CreateShaderVariables(pd3dDevice);
}

CHeightMapTerrain *CScene::GetTerrain()
{
	return((CHeightMapTerrain *)m_ppObjects[1]);
}

void CScene::ReleaseObjects()
{
	ReleaseShaderVariables();

	for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->Release();
	if (m_ppObjects) delete[] m_ppObjects;

	for (int j = 0; j < m_nObjectShaders; j++)
	{
		if (m_ppObjectShaders[j]) m_ppObjectShaders[j]->ReleaseObjects();
		if (m_ppObjectShaders[j]) m_ppObjectShaders[j]->Release();
	}
	if (m_ppObjectShaders) delete[] m_ppObjectShaders;

	for (int j = 0; j < m_nInstancingShaders; j++)
	{
		if (m_ppInstancingShaders[j]) m_ppInstancingShaders[j]->ReleaseObjects();
		if (m_ppInstancingShaders[j]) m_ppInstancingShaders[j]->Release();
	}
	if (m_ppInstancingShaders) delete[] m_ppInstancingShaders;
}

void CScene::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	m_pLights = new LIGHTS;
	::ZeroMemory(m_pLights, sizeof(LIGHTS));
	m_pLights->m_d3dxcGlobalAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

	m_pLights->m_pLights[0].m_bEnable = 1.0f;
	m_pLights->m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights->m_pLights[0].m_fRange = 300.0f;
	m_pLights->m_pLights[0].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvPosition = XMFLOAT3(300.0f, 300.0f, 300.0f);
	m_pLights->m_pLights[0].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);

	m_pLights->m_pLights[1].m_bEnable = 1.0f;
	m_pLights->m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[1].m_fRange = 100.0f;
	m_pLights->m_pLights[1].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxcDiffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[1].m_d3dxvPosition = XMFLOAT3(500.0f, 300.0f, 500.0f);
	m_pLights->m_pLights[1].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights->m_pLights[1].m_fFalloff = 8.0f;
	m_pLights->m_pLights[1].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pLights->m_pLights[1].m_fTheta = (float)cos(D3DXToRadian(20.0f));

	m_pLights->m_pLights[2].m_bEnable = 1.0f;
	m_pLights->m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[2].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcDiffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[2].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pLights->m_pLights[3].m_bEnable = 1.0f;

	m_pLights->m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[3].m_fRange = 60.0f;
	m_pLights->m_pLights[3].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[3].m_d3dxcDiffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_pLights->m_pLights[3].m_d3dxcSpecular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[3].m_d3dxvPosition = XMFLOAT3(500.0f, 300.0f, 500.0f);
	m_pLights->m_pLights[3].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pLights->m_pLights[3].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.01f, 0.001f);
	m_pLights->m_pLights[3].m_fFalloff = 20.0f;
	m_pLights->m_pLights[3].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pLights->m_pLights[3].m_fTheta = (float)cos(D3DXToRadian(15.0f));

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(LIGHTS);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pLights;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbLights);
}

void CScene::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS *pLights)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	LIGHTS *pcbLight = (LIGHTS *)d3dMappedResource.pData;
	memcpy(pcbLight, pLights, sizeof(LIGHTS));
	pd3dDeviceContext->Unmap(m_pd3dcbLights, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_LIGHT, 1, &m_pd3dcbLights);
}

void CScene::ReleaseShaderVariables()
{
	if (m_pLights) delete m_pLights;
	if (m_pd3dcbLights) m_pd3dcbLights->Release();
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

CGameObject *CScene::PickObjectPointedByCursor(int xClient, int yClient)
{
	if (!m_pCamera) return(NULL);

	XMMATRIX d3dxmtxView = m_pCamera->GetViewMatrix();
	XMMATRIX d3dxmtxProjection = m_pCamera->GetProjectionMatrix();
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	XMVECTOR d3dxvPickPosition;

	XMFLOAT4 f4vPickPosition;
	XMFLOAT4X4 f4x4mtxProjection;
	
	XMStoreFloat4x4(&f4x4mtxProjection, d3dxmtxProjection);
	f4vPickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / f4x4mtxProjection._11;
	f4vPickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / f4x4mtxProjection._22;
	f4vPickPosition.z = 1.0f;

	d3dxvPickPosition = XMLoadFloat4(&f4vPickPosition);

	int nIntersected = 0;
	float fNearHitDistance = FLT_MAX;
	MESHINTERSECTINFO d3dxIntersectInfo;
	CGameObject *pIntersectedObject = NULL, *pNearestObject = NULL;
	for (int i = 0; i < m_nObjectShaders; i++)
	{
		pIntersectedObject = m_ppObjectShaders[i]->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if (pIntersectedObject && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = pIntersectedObject;
		}
	}
	for (int i = 1; i < m_nObjects; i++)
	{
		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if ((nIntersected > 0) && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = m_ppObjects[i];
		}
	}

	return(pNearestObject);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	XMVECTOR d3dxvCameraPosition = m_pCamera->GetPosition();
	if (m_pLights && m_pd3dcbLights)
	{
		XMFLOAT4 f4vCameraPosition;
		XMStoreFloat4(&f4vCameraPosition, d3dxvCameraPosition);
		XMStoreFloat4(&m_pLights->m_d3dxvCameraPosition, XMVectorSet(f4vCameraPosition.x, f4vCameraPosition.y, 
			f4vCameraPosition.z, 1.0f));
	
		CHeightMapTerrain *pTerrain = GetTerrain();
		static XMVECTOR d3dxvRotated = XMVectorSet(pTerrain->GetWidth()*0.3f, 0.0f, 0.0f, 0.0f);
		XMMATRIX d3dxmtxRotate;
		d3dxmtxRotate = XMMatrixRotationRollPitchYaw(0.0f, XMConvertToRadians(30.0f*fTimeElapsed), 0.0f);
		d3dxvRotated = XMVector3TransformCoord(d3dxvRotated, d3dxmtxRotate);
		XMVECTOR d3dxvTerrainCenter = XMVectorSet(pTerrain->GetWidth()*0.5f, pTerrain->GetPeakHeight() + 10.0f, pTerrain->GetLength()*0.5f, 0.0f);
		XMStoreFloat3(&m_pLights->m_pLights[0].m_d3dxvPosition, d3dxvTerrainCenter + d3dxvRotated);
		m_pLights->m_pLights[0].m_fRange = pTerrain->GetPeakHeight();
	
		CPlayer *pPlayer = m_pCamera->GetPlayer();
		XMStoreFloat3(&m_pLights->m_pLights[1].m_d3dxvPosition, pPlayer->GetPosition());
		XMStoreFloat3(&m_pLights->m_pLights[1].m_d3dxvDirection, pPlayer->GetLookVector());
	
		XMStoreFloat3(&m_pLights->m_pLights[3].m_d3dxvPosition, pPlayer->GetPosition() + XMVectorSet(0.0f, 80.0f, 0.0f, 0.0f));
	}

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fTimeElapsed, NULL);
	for (int i = 0; i < m_nInstancingShaders; i++) m_ppInstancingShaders[i]->AnimateObjects(fTimeElapsed);
}

void CScene::OnPreRender(ID3D11DeviceContext *pd3dDeviceContext)
{
}

void CScene::Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pLights && m_pd3dcbLights) UpdateShaderVariable(pd3dDeviceContext, m_pLights);

	if (m_ppObjects && m_ppObjects[0]) m_ppObjects[0]->Render(pd3dDeviceContext, pCamera); //SkyBox

	if (m_ppObjects[1]->IsVisible(pCamera))m_ppObjects[1]->Render(pd3dDeviceContext, pCamera);  //Terrain

	if (m_ppObjects && m_ppObjects[2])
		m_ppObjects[2]->Render(pd3dDeviceContext, pCamera); //Water
	
	for (int i = 0; i < m_nObjectShaders; i++) m_ppObjectShaders[i]->Render(pd3dDeviceContext, pCamera);
	for (int i = 0; i < m_nInstancingShaders; i++) m_ppInstancingShaders[i]->Render(pd3dDeviceContext, pCamera);
}
