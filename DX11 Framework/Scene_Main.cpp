#include "stdafx.h"
#include "Scene_Main.h"


CScene_Main::CScene_Main()
{
}

CScene_Main::~CScene_Main()
{
}

bool CScene_Main::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CScene::OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID) {
	case WM_LBUTTONDOWN:
		/*
		m_pSelectedObject = PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam));
		
		if (m_pSelectedObject)
			cout << m_pSelectedObject->GetPosition().x << ", " << m_pSelectedObject->GetPosition().y << ", " << m_pSelectedObject->GetPosition().z << endl;
			*/

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

bool CScene_Main::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CScene::OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
				case '1':
					
				break;

				case '2':
				
				break;
			}
			break;
		case WM_KEYUP:
			switch (wParam) {
				case VK_A:
					
				break;
			}
			break;
		}

	return(false);
}

void CScene_Main::OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex)
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
	DXUT_SetDebugName(pd3dsrvTexture, "SKYBox");
	
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

void CScene_Main::BuildObjects(ID3D11Device *pd3dDevice)
{
	CScene::BuildObjects(pd3dDevice);
	
#pragma region [Create SkyBox]
#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
	CTexture *pSkyboxTexture = new CTexture(1, 1, PS_TEXTURE_SLOT_SKYBOX, PS_SAMPLER_SLOT_SKYBOX);
#else
	#ifdef _WITH_SKYBOX_TEXTURE_CUBE
		CTexture *pSkyboxTexture = new CTexture(1, 1, PS_TEXTURE_SLOT_SKYBOX, PS_SAMPLER_SLOT_SKYBOX);
	#else
		CTexture *pSkyboxTexture = new CTexture(6, 1, PS_TEXTURE_SLOT_SKYBOX, PS_SAMPLER_SLOT_SKYBOX);
	#endif
#endif

	pSkyboxTexture->SetSampler(0, STATEOBJ_MGR->g_pPointClampSS);
	
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

	m_pSkyBox = move(pSkyBox);
#pragma endregion

#pragma region [Create Terrain]
	CTexture *pTerrainTexture = new CTexture(2, 2, PS_TEXTURE_SLOT_TERRAIN_DIFUSE, PS_SAMPLER_SLOT_TERRAIN);

	pTerrainTexture->SetTexture(0, eTexture_TerrainDiffuse);
	pTerrainTexture->SetSampler(0, STATEOBJ_MGR->g_pPointClampSS);
	
	pTerrainTexture->SetTexture(1, eTexture_TerrainDetail);
	pTerrainTexture->SetSampler(1, STATEOBJ_MGR->g_pPointWarpSS);
	
	CMaterialColors *pTerrainColors = new CMaterialColors();
	
	pTerrainColors->m_d3dxcDiffuse = XMFLOAT4(0.8f, 1.0f, 0.2f, 1.0f);
	pTerrainColors->m_d3dxcAmbient = XMFLOAT4(0.1f, 0.3f, 0.1f, 1.0f);
	pTerrainColors->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pTerrainColors->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	/*
	pTerrainColors->m_d3dxcDiffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pTerrainColors->m_d3dxcAmbient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pTerrainColors->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pTerrainColors->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	*/
	CMaterial *pTerrainMaterial = new CMaterial(pTerrainColors);
	pTerrainMaterial->SetTexture(pTerrainTexture);

	XMVECTOR d3dxvScale = XMVectorSet(8.0f, 2.0f, 8.0f, 0.0f);

//	XMVECTOR d3dxvScale = XMVectorSet(8.0f, 1.0f, 8.0f, 0.0f);
#ifdef _WITH_TERRAIN_PARTITION
	CHeightMapTerrain *pTerrain = new CHeightMapTerrain(pd3dDevice, RESOURCE_MGR->FindResourcePath(eTexture_TerrainHeightMapRaw).c_str(), 257, 257, 17, 17, d3dxvScale);
#else
	CHeightMapTerrain *pTerrain = new CHeightMapTerrain(pd3dDevice, RESOURCE_MGR->FindResourcePath(eTexture_HeightMap).c_str(), 257, 257, 257, 257, d3dxvScale);
#endif
	pTerrain->SetMaterial(pTerrainMaterial);

	CShader *pTerrainShader = new CTerrainShader();
	pTerrainShader->CreateShader(pd3dDevice);
	pTerrain->SetShader(pTerrainShader);

	m_pTerrain = move(pTerrain);
#pragma endregion

#pragma region [Create Water]
	CTexture *pTerrainWaterTexture = new CTexture(2, 2, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pTerrainWaterTexture->SetTexture(0, RESOURCE_MGR->FindResourceAndCreateSRV(eTexture_Water));
	pTerrainWaterTexture->SetSampler(0, STATEOBJ_MGR->g_pPointWarpSS);

	pTerrainWaterTexture->SetTexture(1, RESOURCE_MGR->FindResourceAndCreateSRV(eTexture_WaterDetail));
	pTerrainWaterTexture->SetSampler(1, STATEOBJ_MGR->g_pPointWarpSS);

	CMaterialColors *pWaterColors = new CMaterialColors();
	CMaterial *pTerrainWaterMaterial = new CMaterial(pWaterColors);
	pTerrainWaterMaterial->SetTexture(pTerrainWaterTexture);

	CTerrainWater *pTerrainWater = new CTerrainWater(pd3dDevice, 257, 257, 257, 257, d3dxvScale);
	pTerrainWater->SetMaterial(pTerrainWaterMaterial);
	pTerrainWater->SetPosition(0.0f, 80.0f, 0.0f);

	CShader *pTerrainWaterShader = new CWaterShader();
	pTerrainWaterShader->CreateShader(pd3dDevice);
	pTerrainWater->SetShader(pTerrainWaterShader);
	
	m_vObjectsVector.push_back(move(pTerrainWater));
#pragma endregion

#pragma region [Create Character]
	/*
	// Character
	ID3D11ShaderResourceView *pd3dsrvNPCTexture;
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("Character/SimpleMilitary_GasMask_White.png"), nullptr, nullptr, &pd3dsrvNPCTexture, nullptr);
	CTexture *pNPCTexture = new CTexture(1, 1, PS_SLOT_TEXTURE, PS_SLOT_SAMPLER);
	pNPCTexture->SetSampler(0, pd3dSamplerState);
	pNPCTexture->SetTexture(0, pd3dsrvNPCTexture);
	pd3dsrvNPCTexture->Release();

	CMaterialColors* pNPCColors = new CMaterialColors();
	pNPCColors->m_d3dxcDiffuse = D3DXCOLOR(0, 1.0f, 0.0f, 1.0f);
	CMaterial* NPCMaterial = new CMaterial(pNPCColors);
	NPCMaterial->SetTexture(pNPCTexture);

*/
	CSkinnedMesh* pNPCMesh = new CSkinnedMesh(pd3dDevice, RESOURCE_MGR->FindResourcePath(eMesh_Siegetank));
	pNPCMesh->Initialize(pd3dDevice);
#pragma endregion 

	
	CNormalMapObject* normalMapObject = new CNormalMapObject();
	normalMapObject->CreateMaterial(pd3dDevice);
	normalMapObject->CreateMesh(pd3dDevice);
	normalMapObject->CreateShader(pd3dDevice);

	normalMapObject->SetPosition(1000, 700, 1000);
	m_vObjectsVector.push_back(normalMapObject);
	
#pragma region [Create Shader Object]
	
	CMaterial *pPlayerMaterial = new CMaterial();

	CTexture *pPlayerTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pPlayerTexture->SetTexture(0, eTexture_DarkFighterColor);
	pPlayerTexture->SetSampler(0, STATEOBJ_MGR->g_pPointWarpSS);
	pPlayerMaterial->SetTexture(pPlayerTexture);
	
	CModelMesh_FBX* pPlayerMesh = new CModelMesh_FBX(pd3dDevice, RESOURCE_MGR->FindResourcePath(eMesh_DarkFighter));
	pPlayerMesh->Initialize(pd3dDevice);

	CObjectsShader* pModelShader = new CObjectsShader(10);
	pModelShader->CreateShader(pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0);
	pModelShader->SetMaterial(pPlayerMaterial);
	
	CGameObject* pPlayer = new CGameObject();
	pPlayer->SetPosition(200, 250, 200);
	pPlayer->SetMesh(pPlayerMesh);
	pModelShader->AddObject(pPlayer);
	m_vObjectsShaderVector.push_back(pModelShader);
	
#pragma endregion

#pragma region [Create Instancing Object]
	CMaterial *pInstancingMaterials[3];

	CMaterialColors *pRedColor = new CMaterialColors();
	pRedColor->m_d3dxcDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	pRedColor->m_d3dxcAmbient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	pRedColor->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pRedColor->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pInstancingMaterials[0] = new CMaterial(pRedColor);

	CMaterialColors *pGreenColor = new CMaterialColors();
	pGreenColor->m_d3dxcDiffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pGreenColor->m_d3dxcAmbient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pGreenColor->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pGreenColor->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pInstancingMaterials[1] = new CMaterial(pGreenColor);

	CMaterialColors *pBlueColor = new CMaterialColors();
	pBlueColor->m_d3dxcDiffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	pBlueColor->m_d3dxcAmbient = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	pBlueColor->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pBlueColor->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pInstancingMaterials[2] = new CMaterial(pBlueColor);

	CTexture *pStoneTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pStoneTexture->SetTexture(0, eTexture_Stone);
	pStoneTexture->SetSampler(0, STATEOBJ_MGR->g_pPointWarpSS);
	pInstancingMaterials[0]->SetTexture(pStoneTexture);
	
	CTexture *pBrickTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pBrickTexture->SetTexture(0, eTexture_Bricks);
	pBrickTexture->SetSampler(0, STATEOBJ_MGR->g_pPointWarpSS);
	pInstancingMaterials[1]->SetTexture(pBrickTexture);

	CTexture *pWoodTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pWoodTexture->SetTexture(0, eTexture_Wood);
	pWoodTexture->SetSampler(0, STATEOBJ_MGR->g_pPointWarpSS);
	pInstancingMaterials[2]->SetTexture(pWoodTexture);

	float fSize = 24.0f;

	CMesh *pMeshes[6];
	pMeshes[0] = new CCubeMeshTexturedIlluminated(pd3dDevice, fSize, fSize, fSize);
	pMeshes[1] = new CSphereMeshTexturedIlluminated(pd3dDevice, fSize, 20, 20);
	pMeshes[2] = new CCubeMeshTexturedIlluminated(pd3dDevice, fSize, fSize, fSize);
	pMeshes[3] = new CSphereMeshTexturedIlluminated(pd3dDevice, fSize, 20, 20);
	pMeshes[4] = new CCubeMeshTexturedIlluminated(pd3dDevice, 100, 100, 100);
	pMeshes[5] = new CSphereMeshTexturedIlluminated(pd3dDevice, fSize, 20, 20);

	float fxPitch = fSize * 3.5f;
	float fyPitch = fSize * 3.5f;
	float fzPitch = fSize * 3.5f;

	float fTerrainWidth = pTerrain->GetWidth();
	float fTerrainLength = pTerrain->GetLength();

	int xObjects = int(fTerrainWidth / (fxPitch * 3.0f)), zObjects = int(fTerrainLength / (fzPitch * 3.0f));


	m_vInstancedObjectsShaderVector.reserve(6);

	CInstancedObjectsShader *pInstancingShaders;
	for (int k = 0; k < 6; k++)
	{
		pInstancingShaders = new CInstancedObjectsShader(xObjects * zObjects);
		pInstancingShaders->SetMesh(pMeshes[k]);
		pInstancingShaders->SetMaterial(pInstancingMaterials[k % 3]);
		pInstancingShaders->BuildObjects(pd3dDevice, NULL);
		pInstancingShaders->CreateShader(pd3dDevice);

		m_vInstancedObjectsShaderVector.push_back(move(pInstancingShaders));
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
				pRotatingObject->SetRotationSpeed(0.0f);

				m_vInstancedObjectsShaderVector[k * 2]->AddObject(pRotatingObject);

				pRotatingObject = new CRotatingObject();
				pRotatingObject->SetMesh(pMeshes[k * 2 + 1]);
				pRotatingObject->SetPosition(xPosition, fHeight + (fyPitch * 4), zPosition);
				pRotatingObject->SetRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
				pRotatingObject->SetRotationSpeed(0.0f);

				m_vInstancedObjectsShaderVector[k * 2 + 1]->AddObject(pRotatingObject);
			}
		}
	}
#pragma endregion

#pragma region [Particle System]
	m_pParticleSystem = new CParticleSystem();
	m_pParticleSystem->Initialize(pd3dDevice, NULL, m_pParticleSystem->CreateRandomTexture1DSRV(pd3dDevice), 200);
	m_pParticleSystem->CreateShader(pd3dDevice);
#pragma endregion
	CreateLights();
	CreateConstantBuffers(pd3dDevice);
	CreateTweakBars();
}

void CScene_Main::CreateTweakBars()
{
	TwBar *bar = TwNewBar("TweakBar");

	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	TwDefine(" TweakBar size = '300 300'"); 
	TwDefine(" TweakBar alpha = 230");   // transparent bar
	TwDefine(" TweakBar position = '30 100'");

	TwAddVarRW(bar, "Light direction", TW_TYPE_DIR3F, &m_pLights->m_pLights[0].m_d3dxvDirection, "group = Light opened = true axisz = -z");
	TwAddVarRW(bar, "Global Ambient", TW_TYPE_FLOAT, &m_fGlobalAmbient, "group = Light min = 0 max = 1 step = 0.05");
	
	// Option
	TwAddVarRW(bar, "RGB Axis Option", TW_TYPE_BOOLCPP, &m_bShowRGBAxis, "group = Option key=o");
	
	/*
	// Add variables to the tweak bar
	TwAddVarCB(bar, "Level", TW_TYPE_INT32, SetSpongeLevelCB, GetSpongeLevelCB, NULL, "min=0 max=3 group=Sponge keyincr=l keydecr=L");
	TwAddVarCB(bar, "Ambient Occlusion", TW_TYPE_BOOLCPP, SetSpongeAOCB, GetSpongeAOCB, NULL, "group=Sponge key=o");
	TwAddVarRW(bar, "Rotation", TW_TYPE_QUAT4F, &g_SpongeRotation, "opened=true axisz=-z group=Sponge");
	
	
	TwAddVarRW(bar, "Camera distance", TW_TYPE_FLOAT, &g_CamDistance, "min=0 max=4 step=0.01 keyincr=PGUP keydecr=PGDOWN");
	TwAddVarRW(bar, "Background", TW_TYPE_COLOR4F, &g_BackgroundColor, "colormode=hls");
	*/
}

void CScene_Main::ReleaseObjects()
{
	CScene::ReleaseObjects();
	ReleaseConstantBuffers();

	delete m_pParticleSystem;
	m_pParticleSystem = nullptr;
}

void CScene_Main::CreateConstantBuffers(ID3D11Device *pd3dDevice)
{
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

	DXUT_SetDebugName(m_pd3dcbLights, "Lights");
}

void CScene_Main::UpdateConstantBuffers(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS *pLights)
{
	m_pLights->m_d3dxcGlobalAmbient = XMFLOAT4(m_fGlobalAmbient, m_fGlobalAmbient, m_fGlobalAmbient, 1.0f);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	LIGHTS *pcbLight = (LIGHTS *)d3dMappedResource.pData;
	memcpy(pcbLight, pLights, sizeof(LIGHTS));
	pd3dDeviceContext->Unmap(m_pd3dcbLights, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_LIGHT, 1, &m_pd3dcbLights);
}

void CScene_Main::ReleaseConstantBuffers()
{
	if (m_pLights) {
		delete m_pLights;
		m_pLights = nullptr;
	}

	ReleaseCOM(m_pd3dcbLights);
}

void CScene_Main::CreateLights()
{
	m_pLights = new LIGHTS;
	::ZeroMemory(m_pLights, sizeof(LIGHTS));

	m_fGlobalAmbient = 0.5f;
	m_pLights->m_d3dxcGlobalAmbient = XMFLOAT4(m_fGlobalAmbient, m_fGlobalAmbient, m_fGlobalAmbient, 1.0f);

	m_pLights->m_pLights[0].m_bEnable = 1.0f;
	m_pLights->m_pLights[0].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[0].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcDiffuse = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcSpecular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
	m_pLights->m_pLights[0].m_d3dxvDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);

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
	m_pLights->m_pLights[2].m_nType = POINT_LIGHT;
	m_pLights->m_pLights[2].m_fRange = 300.0f;
	m_pLights->m_pLights[2].m_d3dxcAmbient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcDiffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[2].m_d3dxvPosition = XMFLOAT3(300.0f, 300.0f, 300.0f);
	m_pLights->m_pLights[2].m_d3dxvDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[2].m_d3dxvAttenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);

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
}

bool CScene_Main::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void CScene_Main::UpdateObjects(float fTimeElapsed)
{
	CScene::UpdateObjects(fTimeElapsed);

	XMVECTOR d3dxvCameraPosition = m_pCamera->GetPosition();
	if (m_pLights && m_pd3dcbLights)
	{
		XMFLOAT4 f4vCameraPosition;
		XMStoreFloat4(&f4vCameraPosition, d3dxvCameraPosition);
//		XMStoreFloat4(&m_pLights->m_d3dxvCameraPosition, XMVectorSet(f4vCameraPosition.x, f4vCameraPosition.y, f4vCameraPosition.z, 1.0f));

		CHeightMapTerrain *pTerrain = GetTerrain();
		static XMVECTOR d3dxvRotated = XMVectorSet(pTerrain->GetWidth()*0.3f, 0.0f, 0.0f, 0.0f);
		XMMATRIX d3dxmtxRotate;
		d3dxmtxRotate = XMMatrixRotationRollPitchYaw(0.0f, XMConvertToRadians(30.0f*fTimeElapsed), 0.0f);
		d3dxvRotated = XMVector3TransformCoord(d3dxvRotated, d3dxmtxRotate);
		XMVECTOR d3dxvTerrainCenter = XMVectorSet(pTerrain->GetWidth()*0.5f, pTerrain->GetPeakHeight() + 10.0f, pTerrain->GetLength()*0.5f, 0.0f);
		XMStoreFloat3(&m_pLights->m_pLights[0].m_d3dxvPosition, d3dxvTerrainCenter + d3dxvRotated);
		m_pLights->m_pLights[0].m_fRange = pTerrain->GetPeakHeight();

		CPlayer *pPlayer = m_pCamera->GetPlayer();
		XMStoreFloat3(&m_pLights->m_pLights[1].m_d3dxvPosition, pPlayer->GetvPosition());
		XMStoreFloat3(&m_pLights->m_pLights[1].m_d3dxvDirection, pPlayer->GetLookVector());

		XMStoreFloat3(&m_pLights->m_pLights[3].m_d3dxvPosition, pPlayer->GetvPosition() + XMVectorSet(0.0f, 80.0f, 0.0f, 0.0f));
	}

	// Light Shader Update
	if (m_pLights && m_pd3dcbLights) UpdateConstantBuffers(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get(), m_pLights);

	// Particle
	m_fGametime += fTimeElapsed;
	m_pParticleSystem->Update(fTimeElapsed, m_fGametime);
}

void CScene_Main::OnPreRender(ID3D11DeviceContext *pd3dDeviceContext)
{
}

void CScene_Main::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CScene::Render(pd3dDeviceContext, pCamera);

//	m_pParticleSystem->Render(pd3dDeviceContext);
}

void CScene_Main::RenderAllText(ID3D11DeviceContext *pd3dDeviceContext)
{
	CScene::RenderAllText(pd3dDeviceContext);

	string str;
	wstring wstr;
	
	// Draw Position
	XMFLOAT3 playerPos = m_pPlayer->GetPosition();
	str = "Player Position : (" + to_string(playerPos.x) + ", " + to_string(playerPos.y) + ", " + to_string(playerPos.z) + ")";
	wstr.assign(str.begin(), str.end());
	TEXT_MGR->RenderText(pd3dDeviceContext, wstr, 30, 20, 50, 0xFFFFFFFF, FW1_LEFT);

	/*
	// Draw Time Gap
	XMFLOAT3 cameraPos;
	XMStoreFloat3(&cameraPos, m_pCamera->GetPosition());
	str = "Camera Position : (" + to_string(cameraPos.x) + ", " + to_string(cameraPos.y) + ", " + to_string(cameraPos.z) + ")";
	wstr.assign(str.begin(), str.end());
	TEXT_MGR->RenderText(pd3dDeviceContext, wstr, 30, 20, 80, 0xFFFFFFFF, FW1_LEFT);
	*/


//	str = L"최고의 플레이";
//	wstr = L"최고의 플레이";
//	wstr.assign(str.begin(), str.end());
//	TEXT_MGR->RenderText(pd3dDeviceContext, wstr, 40, 70, 100, 0xFFFFFFFF, FW1_LEFT);
}