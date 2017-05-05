#include "stdafx.h"
#include "MainScene.h"

CMainScene::CMainScene()
{
	m_tagScene = SceneTag::eMainScene;
}

CMainScene::~CMainScene()
{
	delete(m_GBuffer);
}

bool CMainScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CScene::OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID) {
	case WM_LBUTTONDOWN:
		m_pSelectedObject = PickObjectPointedByCursor(LOWORD(lParam) * m_fResizeRatioX, HIWORD(lParam) * m_fResizeRatioY);

		/*
		if (m_pSelectedObject) {
			cout << "ID : " << m_pSelectedObject->GetObjectID() << endl;
			cout << "Position : " << m_pSelectedObject->GetPosition().x << ", " << m_pSelectedObject->GetPosition().y << ", " << m_pSelectedObject->GetPosition().z << endl;
			cout << "Bounding Center : " << m_pSelectedObject->GetBoundingSphere().Center.x << ", " << m_pSelectedObject->GetBoundingSphere().Center.y<< ", " << m_pSelectedObject->GetBoundingSphere().Center.z << endl;
			cout << "Bounding Radius : " << m_pSelectedObject->GetBoundingSphere().Radius << endl;
			cout << "Bounding Box Extent : " << m_pSelectedObject->GetBoundingOBox().Extents.x << ", " << m_pSelectedObject->GetBoundingOBox().Extents.y << ", " << m_pSelectedObject->GetBoundingOBox().Extents.z << endl;
		}
		*/
		m_pPlayer->SetKeyDown(KeyInput::eLeftMouse);
		break;
	case WM_LBUTTONUP:
		m_pPlayer->SetKeyUp(KeyInput::eLeftMouse);
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
	return(false);
}

bool CMainScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CScene::OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_W:
				m_pPlayer->SetKeyDown(KeyInput::eForward);
				break;
			case VK_S:
				m_pPlayer->SetKeyDown(KeyInput::eBackward);
				break;
			case VK_A:
				m_pPlayer->SetKeyDown(KeyInput::eLeft);
				break;
			case VK_D:
				m_pPlayer->SetKeyDown(KeyInput::eRight);
				break;
			case VK_SHIFT:
				m_pPlayer->SetKeyDown(KeyInput::eRun);
				break;
			case VK_F1:
			case VK_F2:
			case VK_F3:
					m_pPlayer->ChangeCamera(m_pd3dDevice, CameraTag(wParam - VK_F1 + 1));
					m_pCamera = m_pPlayer->GetCamera();

					if (wParam - VK_F1 == 2)
						m_pUIManager->GetUIObject(TextureTag::eAim)->SetActive(false);
					else 
						m_pUIManager->GetUIObject(TextureTag::eAim)->SetActive(true);
				break;
			case VK_F4:		// 중력 테스트용으로 넣음
				m_pPlayer->SetPosition(XMVectorSet(60, 50, 30, 0));
				m_pPlayer->SetTimeElpased(0.0f);

				m_pPlayer->SetVelocity(XMFLOAT3(0, 0, 0));
				break;
			case VK_F6:		// 회전 테스트용으로 넣음
			//	m_vecCharacterContainer[1]->SetRotate(TWBAR_MGR->g_xmf3Rotate.x, TWBAR_MGR->g_xmf3Rotate.y, TWBAR_MGR->g_xmf3Rotate.z);
			
				//m_pPlayer
				break;
			case VK_Z:
				for(auto& object : m_vecCharacterContainer)
					object->SetAnimation(AnimationData::CharacterAnim::eIdle);
				break;
			case VK_X:
				for (auto& object : m_vecCharacterContainer)
					object->SetAnimation(AnimationData::CharacterAnim::eForwardWalk);
				break;
			case VK_C:
				for (auto& object : m_vecCharacterContainer)
					object->SetAnimation(AnimationData::CharacterAnim::eRun);
				break;
			case VK_V:
			
				break;
			}
			break;
		case WM_KEYUP:
			switch (wParam) {
			case VK_W:
				m_pPlayer->SetKeyUp(KeyInput::eForward);
				break;
			case VK_S:
				m_pPlayer->SetKeyUp(KeyInput::eBackward);
				break;
			case VK_A:
				m_pPlayer->SetKeyUp(KeyInput::eLeft);
				break;
			case VK_D:
				m_pPlayer->SetKeyUp(KeyInput::eRight);
				break;
			case VK_SHIFT:
				m_pPlayer->SetKeyUp(KeyInput::eRun);
				break;
			}
			break;
		}

	return(false);
}

void CMainScene::OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex)
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
	
	pMaterial->GetTexture()->SetTexture(0, pd3dsrvTexture); pd3dsrvTexture->Release();
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

void CMainScene::Initialize()
{
	cout << "=============================================================================================" << endl;
	cout << "==================================== Scene Main Loading =====================================" << endl;

	ShowCursor(false);

	m_pWorldCenterAxis = new CAxisObjects();
	m_pWorldCenterAxis->CreateAxis(m_pd3dDevice);
	m_vecShaderObjectContainer.BuildObjects(m_pd3dDevice);
	m_pBoundingBoxShader = new CBoundingBoxShader();
	m_pBoundingBoxShader->CreateShader(m_pd3dDevice);

	m_GBuffer = new CGBuffer();
	m_GBuffer->Initialize(m_pd3dDevice);

	m_pSphereObject = new CSphereObject();
	m_pSphereObject->CreateObjectData(m_pd3dDevice);
	m_vecObjectsContainer.push_back(m_pSphereObject);

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
	OnChangeSkyBoxTextures(m_pd3dDevice, pSkyboxMaterial, 0);

	//CSkyBoxMesh *pSkyBoxMesh = new CSkyBoxMesh(m_pd3dDevice, 20.0f, 20.0f, 20.0f);
	CSkyBoxMesh *pSkyBoxMesh = new CSkyBoxMesh(m_pd3dDevice, 3000.0f, 3000.0f, 3000.0f);
	CSkyBox *pSkyBox = new CSkyBox(m_pd3dDevice);
	pSkyBox->SetMesh(pSkyBoxMesh, 0);
	pSkyBox->SetMaterial(pSkyboxMaterial);

	CShader *pSkyBoxShader = new CSkyBoxShader();
	pSkyBoxShader->CreateShader(m_pd3dDevice);
	pSkyBox->SetShader(pSkyBoxShader);
	
	m_pSkyBox = move(pSkyBox);
#pragma endregion

	/*
#pragma region [Create Terrain]
	CTexture *pTerrainTexture = new CTexture(2, 2, PS_TEXTURE_SLOT_TERRAIN_DIFUSE, PS_SAMPLER_SLOT_TERRAIN);

	pTerrainTexture->SetTexture(0, TextureTag::eTerrainD);
	pTerrainTexture->SetSampler(0, STATEOBJ_MGR->g_pPointClampSS);
	
	pTerrainTexture->SetTexture(1, TextureTag::eTerrainDetailD);
	pTerrainTexture->SetSampler(1, STATEOBJ_MGR->g_pPointWarpSS);
	
	CMaterialColors *pTerrainColors = new CMaterialColors();
	
	pTerrainColors->m_d3dxcDiffuse = XMFLOAT4(0.8f, 1.0f, 0.2f, 1.0f);
	pTerrainColors->m_d3dxcAmbient = XMFLOAT4(0.1f, 0.3f, 0.1f, 1.0f);
	pTerrainColors->m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	pTerrainColors->m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	CMaterial *pTerrainMaterial = new CMaterial(pTerrainColors);
	pTerrainMaterial->SetTexture(pTerrainTexture);

	XMVECTOR d3dxvScale = XMVectorSet(8.0f, 2.0f, 8.0f, 0.0f);

//	XMVECTOR d3dxvScale = XMVectorSet(8.0f, 1.0f, 8.0f, 0.0f);
#ifdef _WITH_TERRAIN_PARTITION
	CHeightMapTerrain *pTerrain = new CHeightMapTerrain(m_pd3dDevice, RESOURCE_MGR->FindResourcePath(TextureTag::eTerrainH).c_str(), 257, 257, 17, 17, d3dxvScale);
#else
	CHeightMapTerrain *pTerrain = new CHeightMapTerrain(m_pd3dDevice, RESOURCE_MGR->FindResourcePath(eHeightMap).c_str(), 257, 257, 257, 257, d3dxvScale);
#endif
	pTerrain->SetMaterial(pTerrainMaterial);

	CShader *pTerrainShader = new CTerrainShader();
	pTerrainShader->CreateShader(m_pd3dDevice);
	pTerrain->SetShader(pTerrainShader);

	m_pTerrain = move(pTerrain);
#pragma endregion
	*/

	/*
#pragma region [Create Water]
	CTexture *pTerrainWaterTexture = new CTexture(2, 2, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pTerrainWaterTexture->SetTexture(0, TextureTag::eWaterD);
	pTerrainWaterTexture->SetSampler(0, STATEOBJ_MGR->g_pPointWarpSS);

	pTerrainWaterTexture->SetTexture(1, TextureTag::eWaterDetailD);
	pTerrainWaterTexture->SetSampler(1, STATEOBJ_MGR->g_pPointWarpSS);

	CMaterialColors *pWaterColors = new CMaterialColors();
	CMaterial *pTerrainWaterMaterial = new CMaterial(pWaterColors);
	pTerrainWaterMaterial->SetTexture(pTerrainWaterTexture);

	CTerrainWater *pTerrainWater = new CTerrainWater(m_pd3dDevice, 257, 257, 257, 257, d3dxvScale);
	pTerrainWater->SetMaterial(pTerrainWaterMaterial);
	pTerrainWater->SetPosition(0.0f, 80.0f, 0.0f);

	CShader *pTerrainWaterShader = new CWaterShader();
	pTerrainWaterShader->CreateShader(m_pd3dDevice);
	pTerrainWater->SetShader(pTerrainWaterShader);
	
	m_vecObjectsContainer.push_back(move(pTerrainWater));
#pragma endregion
	*/
	
#pragma region [Create Character]
	CScene::CreatePlayer();
	m_vecBBoxRenderContainer.push_back(m_pPlayerCharacter);
	m_vecCharacterContainer.push_back(m_pPlayerCharacter);
//	m_pPlayer->SetPlayerUpdatedContext(pTerrain);
//	m_pPlayer->SetCameraUpdatedContext(pTerrain);


#pragma endregion 

	/*
#pragma region [Create CaptureArea]
	// Material
	CMaterial* pCaptureAreaMaterial = new CMaterial();
	CTexture *pCaptureAreaTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pCaptureAreaTexture->SetTexture(0, TextureTag::eCaptureArea);
	pCaptureAreaTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);
	pCaptureAreaMaterial->SetTexture(pCaptureAreaTexture);
	
	// Mesh
	CMesh* pCaptureAreaMesh = new CTexturedRectMesh(m_pd3dDevice, 50, 50);
	
	// Shader
	CShader* pCaptureAreaShader = new CShader();
	pCaptureAreaShader->CreateShader(m_pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0);

	CBillboardObject* pCaptureAreaObject = new CBillboardObject(m_pPlayer);
	pCaptureAreaObject->SetMaterial(pCaptureAreaMaterial);
	pCaptureAreaObject->SetMesh(pCaptureAreaMesh);
	pCaptureAreaObject->SetShader(pCaptureAreaShader);

	pCaptureAreaObject->SetPosition(0, 300, 0);

	m_vecObjectsContainer.push_back(pCaptureAreaObject);
#pragma endregion 
*/
#pragma region [Create Test - NomalMapping]	
	/*
	CNormalMapObject* normalMapObject = new CNormalMapObject();
	normalMapObject->CreateObjectData(m_pd3dDevice);

	normalMapObject->SetPosition(1000, 700, 1000);
	m_vecObjectsContainer.push_back(normalMapObject);
	*/
#pragma endregion 

	 
	/*
	// ==== Test용 - 총 메쉬 오프셋 찾기용 ==== //
	CTerroristCharacterObject* pCharacter = new CTerroristCharacterObject();
	pCharacter->CreateObjectData(m_pd3dDevice);
	pCharacter->CreateAxisObject(m_pd3dDevice);

	pCharacter->SetPosition(10.0f, 2.0f, 0.0f);

	m_vecBBoxRenderContainer.push_back(pCharacter);
	m_vecCharacterContainer.push_back(pCharacter);

	COLLISION_MGR->m_vecCharacterContainer.push_back(pCharacter);
	*/
#pragma region [Create Shader Object]
	// ----- Test ----- //
	CFbxModelMesh* pTestMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eTest2);
	pTestMesh->Initialize(m_pd3dDevice);


	CGameObject* pTestObject= new CGameObject();
	pTestObject->SetMaterial(1, TextureTag::eTerroristD);
	pTestObject->SetMesh(pTestMesh);
	pTestObject->CreateBoundingBox(m_pd3dDevice);
	pTestObject->CreateAxisObject(m_pd3dDevice);
	
	AddShaderObject(ShaderTag::eNormalTangentTexture, pTestObject);
#pragma endregion

	/*
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
	pStoneTexture->SetTexture(0, TextureTag::eStoneD);
	pStoneTexture->SetSampler(0, STATEOBJ_MGR->g_pPointWarpSS);
	pInstancingMaterials[0]->SetTexture(pStoneTexture);
	
	CTexture *pBrickTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pBrickTexture->SetTexture(0, TextureTag::eBricksD);
	pBrickTexture->SetSampler(0, STATEOBJ_MGR->g_pPointWarpSS);
	pInstancingMaterials[1]->SetTexture(pBrickTexture);

	CTexture *pWoodTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pWoodTexture->SetTexture(0, TextureTag::eWallD);
	pWoodTexture->SetSampler(0, STATEOBJ_MGR->g_pPointWarpSS);
	pInstancingMaterials[2]->SetTexture(pWoodTexture);

	float fSize = 24.0f;

	CMesh *pMeshes[6];
	pMeshes[0] = new CCubeMeshTexturedIlluminated(m_pd3dDevice, fSize, fSize, fSize);
	pMeshes[1] = new CSphereMeshTexturedIlluminated(m_pd3dDevice, fSize, 20, 20);
	pMeshes[2] = new CCubeMeshTexturedIlluminated(m_pd3dDevice, fSize, fSize, fSize);
	pMeshes[3] = new CSphereMeshTexturedIlluminated(m_pd3dDevice, fSize, 20, 20);
	pMeshes[4] = new CCubeMeshTexturedIlluminated(m_pd3dDevice, 100, 100, 100);
	pMeshes[5] = new CSphereMeshTexturedIlluminated(m_pd3dDevice, fSize, 20, 20);

	float fxPitch = fSize * 3.5f;
	float fyPitch = fSize * 3.5f;
	float fzPitch = fSize * 3.5f;

	float fTerrainWidth = 1000;
	float fTerrainLength = 1000;

	//int xObjects = int(fTerrainWidth / (fxPitch * 3.0f)), zObjects = int(fTerrainLength / (fzPitch * 3.0f));
	int xObjects = 3, zObjects = 3;


	m_vecInstancedObjectsShaderContainer.reserve(6);

	CInstancedObjectsShader *pInstancingShaders;
	for (int k = 0; k < 6; k++)
	{
		pInstancingShaders = new CInstancedObjectsShader(xObjects * zObjects);
		pInstancingShaders->SetMesh(pMeshes[k]);
		pInstancingShaders->SetMaterial(pInstancingMaterials[k % 3]);
		pInstancingShaders->BuildObjects(m_pd3dDevice, NULL);
		pInstancingShaders->CreateShader(m_pd3dDevice);

		m_vecInstancedObjectsShaderContainer.push_back(move(pInstancingShaders));
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
				float fHeight = rand() % 200;

				pRotatingObject = new CRotatingObject();
				pRotatingObject->SetMesh(pMeshes[k * 2]);
				pRotatingObject->SetPosition(xPosition, fHeight + 6.0f, zPosition);
				pRotatingObject->CreateBoundingBox(m_pd3dDevice);

				m_vecInstancedObjectsShaderContainer[k * 2]->AddObject(pRotatingObject);
				COLLISION_MGR->m_vecStaticMeshContainer.push_back(pRotatingObject);

				pRotatingObject = new CRotatingObject();
				pRotatingObject->SetMesh(pMeshes[k * 2 + 1]);
				pRotatingObject->SetPosition(xPosition, fHeight + (fyPitch * 4), zPosition);
				pRotatingObject->SetRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
				pRotatingObject->SetRotationSpeed(30.0f);
				pRotatingObject->CreateBoundingBox(m_pd3dDevice);

				m_vecInstancedObjectsShaderContainer[k * 2 + 1]->AddObject(pRotatingObject);
				COLLISION_MGR->m_vecStaticMeshContainer.push_back(pRotatingObject);
			}
		}
	}
#pragma endregion
	*/
#pragma region [Particle System]
	m_pParticleSystem = new CParticleSystem();
	m_pParticleSystem->Initialize(m_pd3dDevice, NULL, m_pParticleSystem->CreateRandomTexture1DSRV(m_pd3dDevice), 200);
	m_pParticleSystem->CreateShader(m_pd3dDevice);
#pragma endregion

	CreateMapDataObject();
	CreateLights();
	CreateConstantBuffers();
	CreateTweakBars();
	CreateUIImage();

	cout << "================================== Scene Loading Complete ===================================" << endl;
	cout << "=============================================================================================" << endl << endl;
}

void CMainScene::CreateMapDataObject()
{
	// Mesh
	CMesh* pMesh;
	CFbxModelMesh* pFbxMesh;

	// Object
	CGameObject* pObject;
	vector<MapData> vecMapData;

#pragma region [Plane]
	pObject = new CGameObject();
	pMesh = new CCubeMeshIlluminated(m_pd3dDevice, 280, 1, 300);

	pObject->SetMesh(pMesh);
	pObject->SetPosition(XMFLOAT3(140, 0, 150));
	pObject->CreateBoundingBox(m_pd3dDevice);

	AddShaderObject(ShaderTag::eNormal, pObject);
#pragma endregion

	
#pragma region [Road]
	// ==============================   Road   ============================== //
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eRoad1);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eRoad, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);
		
		pObject->SetMaterial(1, TextureTag::eRoad1D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eRoad2);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eRoad, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eRoad2D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eCrossRoad);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eCrossRoad, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eCrossRoadD);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTangentTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eCenterRoad);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eCenterRoad, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eCenterRoadD);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTangentTexture, pObject);
	}
#pragma endregion
	
	
#pragma region [Building]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding16);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding16, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding16D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateAxisObject(m_pd3dDevice);
		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding19);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding19, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding19D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding20);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding20, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding20D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding21);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding21, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding21D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);
		pObject->CreateAxisObject(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding22);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding22, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding22D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateAxisObject(m_pd3dDevice);
		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding26);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding26, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding26D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);
		pObject->CreateAxisObject(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding29);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding29, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding29D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);
		pObject->CreateAxisObject(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding30);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding30, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding30D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding33);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding33, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding33D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding34);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding34, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding34D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding50);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding50, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding50D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}
#pragma endregion
	
#pragma region [Bench]
	// ==============================   Road   ============================== //
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBench);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBench, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBenchD);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}
#pragma endregion

#pragma region [Grass]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eGrass);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pMesh = new CCubeMeshTexturedIlluminated(m_pd3dDevice, vecMapData[count].m_Scale.x, vecMapData[count].m_Scale.y, vecMapData[count].m_Scale.z);

		pObject->SetMaterial(1, TextureTag::eGrassD);
		pObject->SetMesh(pMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		//pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}
#pragma endregion

#pragma region [Busstop]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBusStop);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBusStop, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBusStopD);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
	
		pObject->CreateAxisObject(m_pd3dDevice);
		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}
#pragma endregion
	
#pragma region [Street Lamp]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eStreetLamp);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eStreetLamp, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMesh(pFbxMesh);
		pObject->CreateAxisObject(m_pd3dDevice);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormal, pObject);
	}
#pragma endregion
	
}

void CMainScene::CreateTweakBars()
{
	TwBar *tweakBar = TWBAR_MGR->g_tweakBar;

	TwAddVarRW(tweakBar, "Light direction", TW_TYPE_DIR3F, &m_pLights->m_pLights[0].m_d3dxvDirection, "group = Light opened = true axisz = -z");
	TwAddVarRW(tweakBar, "Global Ambient", TW_TYPE_FLOAT, &m_fGlobalAmbient, "group = Light min = 0 max = 1 step = 0.05");
	
	// Option
	TwAddVarRW(tweakBar, "RGB Axis Option", TW_TYPE_BOOLCPP, &GLOBAL_MGR->g_bShowWorldAxis, "group = Option");
	
	TwDefine("TweakBar/Light opened=false ");
	/*
	// Add variables to the tweak bar
	TwAddVarCB(tweakBar, "Level", TW_TYPE_INT32, SetSpongeLevelCB, GetSpongeLevelCB, NULL, "min=0 max=3 group=Sponge keyincr=l keydecr=L");
	TwAddVarCB(tweakBar, "Ambient Occlusion", TW_TYPE_BOOLCPP, SetSpongeAOCB, GetSpongeAOCB, NULL, "group=Sponge key=o");
	TwAddVarRW(tweakBar, "Rotation", TW_TYPE_QUAT4F, &g_SpongeRotation, "opened=true axisz=-z group=Sponge");
	
	
	TwAddVarRW(tweakBar, "Camera distance", TW_TYPE_FLOAT, &g_CamDistance, "min=0 max=4 step=0.01 keyincr=PGUP keydecr=PGDOWN");
	TwAddVarRW(tweakBar, "Background", TW_TYPE_COLOR4F, &g_BackgroundColor, "colormode=hls");
	*/
}

void CMainScene::CreateUIImage()
{
	m_pUIManager = new CUIManager();
	m_pUIManager->Initialize(m_pd3dDevice);

	// Aim
	CUIObject* pAimUI = new CUIObject(TextureTag::eAim);
	POINT aimingPos = POINT{ FRAME_BUFFER_WIDTH / 2 + 5, FRAME_BUFFER_HEIGHT / 2 - 15};		// 오프셋 (1, -10)
	pAimUI->Initialize(m_pd3dDevice, POINT{ aimingPos.x - 20, aimingPos.y - 20 }, POINT{ aimingPos.x + 20, aimingPos.y + 20 }, 0.0f);
	m_pUIManager->AddUIObject(pAimUI); 
	pAimUI->SetActive(false);
}

void CMainScene::ReleaseObjects()
{
	CScene::ReleaseObjects();
	ReleaseConstantBuffers();

	SafeDelete(m_pParticleSystem);
	SafeDelete(m_pBoundingBoxShader);

	for (auto& object : m_vecCharacterContainer)
		SafeDelete(object);
}

void CMainScene::AddShaderObject(ShaderTag tag, CGameObject* pObject)
{
	m_vecBBoxRenderContainer.push_back(pObject);
	m_vecShaderObjectContainer.AddObject(tag, pObject);
	COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
}

void CMainScene::CreateConstantBuffers()
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
	m_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbLights);

	DXUT_SetDebugName(m_pd3dcbLights, "Lights");
}

void CMainScene::UpdateConstantBuffers(LIGHTS *pLights)
{
	m_pLights->m_d3dxcGlobalAmbient = XMFLOAT4(m_fGlobalAmbient, m_fGlobalAmbient, m_fGlobalAmbient, 1.0f);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	m_pd3dDeviceContext->Map(m_pd3dcbLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	LIGHTS *pcbLight = (LIGHTS *)d3dMappedResource.pData;
	memcpy(pcbLight, pLights, sizeof(LIGHTS));
	m_pd3dDeviceContext->Unmap(m_pd3dcbLights, 0);
	m_pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_LIGHT, 1, &m_pd3dcbLights);
}

void CMainScene::ReleaseConstantBuffers()
{
	if (m_pLights) {
		delete m_pLights;
		m_pLights = nullptr;
	}

	ReleaseCOM(m_pd3dcbLights);
}

void CMainScene::CreateLights()
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

void CMainScene::ModifiedSelectObject()
{
	XMFLOAT3 pos = m_pSelectedObject->GetPosition();

	TWBAR_MGR->g_xmf3SelectObjectPosition = pos;
	
//	m_pSelectedObject->SetPosition(TWBAR_MGR->g_xmf3SelectObjectPosition);
//	m_pSelectedObject->SetRotate(TWBAR_MGR->g_xmf3SelectObjectRotate);
	
//	ShowXMFloat3(TWBAR_MGR->g_xmf3SelectObjectRotate);
}

void CMainScene::Update(float fDeltaTime)
{
	COLLISION_MGR->UpdateManager();
	CScene::Update(fDeltaTime);

//	if (m_pSelectedObject)
//		ModifiedSelectObject();

	for (auto& object : m_vecCharacterContainer)
		object->Update(fDeltaTime);

	if (m_pLights && m_pd3dcbLights)
	{
		XMFLOAT3 f3vCameraPosition = m_pCamera->GetPosition();
//		XMStoreFloat4(&m_pLights->m_d3dxvCameraPosition, XMVectorSet(f4vCameraPosition.x, f4vCameraPosition.y, f4vCameraPosition.z, 1.0f));
		
		XMStoreFloat3(&m_pLights->m_pLights[1].m_d3dxvPosition, m_pPlayer->GetvPosition());
		XMStoreFloat3(&m_pLights->m_pLights[1].m_d3dxvDirection, m_pPlayer->GetvLook());

		XMStoreFloat3(&m_pLights->m_pLights[3].m_d3dxvPosition, m_pPlayer->GetvPosition() + XMVectorSet(0.0f, 80.0f, 0.0f, 0.0f));
	}

	// Light Shader Update
	if (m_pLights && m_pd3dcbLights) UpdateConstantBuffers(m_pLights);

	// Particle
	m_fGametime += fDeltaTime;
	m_pParticleSystem->Update(fDeltaTime, m_fGametime);
}

void CMainScene::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	// =============== Deferred Rendering ================== //
	m_GBuffer->OnPreRender(pd3dDeviceContext);

	// ------ Start Scene Rendering ------ //
	CScene::Render(pd3dDeviceContext, pCamera);

	if (m_pTerrain)
		if (m_pTerrain->IsVisible(pCamera))
			m_pTerrain->Render(pd3dDeviceContext, pCamera);

	m_pParticleSystem->Render(pd3dDeviceContext);

	for (auto& object : m_vecCharacterContainer)
		object->Render(m_pd3dDeviceContext, m_pCamera);

	// ------ End Scene Rendering ------ //
	m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);

	m_GBuffer->OnPostRender(pd3dDeviceContext);
	m_GBuffer->OnPrepareForUnpack(pd3dDeviceContext);

	pd3dDeviceContext->OMSetRenderTargets(1, &SCENE_MGR->g_pd3dRenderTargetView, m_GBuffer->GetDepthReadOnlyDSV());

	// ------ Final Scene Rendering ------ //
	m_GBuffer->DeferredRender(pd3dDeviceContext);
	m_pUIManager->RenderAll(pd3dDeviceContext);
	// =============== Rendering Option =================== //

	for (auto& lineObject : GLOBAL_MGR->g_vecLineContainer)
		lineObject->Render(m_pd3dDeviceContext, m_pCamera);

	if (GLOBAL_MGR->g_bShowWorldAxis)
		m_pWorldCenterAxis->Render(pd3dDeviceContext, pCamera);

	if (GLOBAL_MGR->g_vRenderOption.y)
		RenderBoundingBox();

	if (GLOBAL_MGR->g_bShowGBuffer) {
		pd3dDeviceContext->OMSetRenderTargets(1, &SCENE_MGR->g_pd3dRenderTargetView, nullptr);
		m_GBuffer->Render(pd3dDeviceContext);
		pd3dDeviceContext->OMSetRenderTargets(1, &SCENE_MGR->g_pd3dRenderTargetView, m_GBuffer->GetDepthDSV());
	}
}

void CMainScene::RenderBoundingBox()
{
	m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pWireframeRS);
	m_pBoundingBoxShader->OnPrepareSetting(m_pd3dDeviceContext, false);

	for (auto object : m_vecBBoxRenderContainer) {
//		if (m_bIsPreCollisionCheck != object->GetCollisionCheck()) {
//			m_bIsPreCollisionCheck = object->GetCollisionCheck();
//			m_pBoundingBoxShader->OnPrepareSetting(m_pd3dDeviceContext, object->GetCollisionCheck());
//			cout << "충돌해서 한 번 바뀌었다." << endl;
//		}
		m_pBoundingBoxShader->OnPrepareSetting(m_pd3dDeviceContext, object->GetCollisionCheck());
		object->BoundingBoxRender(m_pd3dDeviceContext);
	}

	m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);
}

void CMainScene::RenderAllText(ID3D11DeviceContext *pd3dDeviceContext)
{
	string ppos;
	string rotate;
	string orotate;
	
	// Draw Position
	XMFLOAT3 playerPos = m_pPlayer->GetPosition();
	XMFLOAT3 playerrotate = m_pPlayer->GetLook();
//	XMVECTOR otherrotate = GetCharcontainer()[1]->GetLook(false);

	XMFLOAT3 temp;
//	XMStoreFloat3(&temp, otherrotate);

	ppos = "Player Position : (" + to_string(playerPos.x) + ", " + to_string(playerPos.y) + ", " + to_string(playerPos.z) + ")\n";
	rotate = "player rotate : (" + to_string(playerrotate.x) + ", " + to_string(playerrotate.y) + ", " + to_string(playerrotate.z) + ")\n";
	orotate = "other rotate : (" + to_string(temp.x) + ", " + to_string(temp.y) + ", " + to_string(temp.z) + ")\n";
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(ppos), 30, 20, 50, 0xFFFFFFFF, FW1_LEFT);
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(rotate), 30, 20, 90, 0xFFFFFFFF, FW1_LEFT);
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(orotate), 30, 20, 140, 0xFFFFFFFF, FW1_LEFT);

	// Draw Select Object
	if (m_pSelectedObject) {
		XMFLOAT3 pos = m_pSelectedObject->GetPosition();


	
		ppos = "Position : (" + to_string(pos.x) + ", " + to_string(pos.y) + to_string(pos.z) + ")";


		//TEXT_MGR->RenderText(pd3dDeviceContext, ppos, 30, 20, 80, 0xFFFFFFFF, FW1_LEFT);
	}
}