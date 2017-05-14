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
	static bool bTest = false;
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
			case VK_R:
				m_pPlayer->SetKeyDown(KeyInput::eReload);
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
				m_pPlayer->SetPosition(XMVectorSet(60, 30, 30, 0));
				m_pPlayer->SetGravityTimeElpased(0.0f);

				m_pPlayer->SetVelocity(XMFLOAT3(0, 0, 0));
				break;
			case VK_Z:
				m_vecCharacterContainer.back()->SetAnimation(AnimationData::CharacterAnim::eIdle);
				break;
			case VK_X:
				m_vecCharacterContainer.back()->SetAnimation(AnimationData::CharacterAnim::eRun);
				break;
			case VK_C:
				
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
			case VK_R:
				m_pPlayer->SetKeyUp(KeyInput::eReload);
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

	 
#ifndef USE_SERVER
	// ==== Test용 - 총 메쉬 오프셋 찾기용 ==== //
	CTerroristCharacterObject* pCharacter = new CTerroristCharacterObject();
	pCharacter->CreateObjectData(m_pd3dDevice);
	pCharacter->CreateAxisObject(m_pd3dDevice);

	pCharacter->SetPosition(40.0f, 1.9f, 2.0f);

	m_vecBBoxRenderContainer.push_back(pCharacter);
	m_vecCharacterContainer.push_back(pCharacter);

	COLLISION_MGR->m_vecCharacterContainer.push_back(pCharacter);
#endif

#pragma region [Particle System]
	m_pParticleSystem = new CParticleSystem();
	m_pParticleSystem->Initialize(m_pd3dDevice, NULL, m_pParticleSystem->CreateRandomTexture1DSRV(m_pd3dDevice), 200);
	m_pParticleSystem->CreateShader(m_pd3dDevice);
#pragma endregion

//	CreateMapDataObject();
	CreateMapDataInstancingObject();
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
		pObject->CreateAxisObject(m_pd3dDevice);

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
		pObject->CreateAxisObject(m_pd3dDevice);

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

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding77);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding77, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding77D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding78);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding78, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding78D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding100);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding100, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding100D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding103);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding103, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding103D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding104);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding104, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding103D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eParkingLot);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eParkingLot, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eParkingLotD);
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
	
#pragma region [Barricade]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBarricade);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBarricade, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMesh(pFbxMesh);
		pObject->CreateAxisObject(m_pd3dDevice);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}
#pragma endregion
}

void CMainScene::CreateMapDataInstancingObject()
{
	// Mesh
	CMesh* pMesh;
	CFbxModelMesh* pFbxMesh;

	// Object
	CGameObject* pObject;
	vector<MapData> vecMapData;

	// Shader
	CInstancedObjectsShader* pInstancingShaders;
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
	// Road1
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eRoad1).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eRoad);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eRoad1D);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eRoad1);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
	
	// Road2
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eRoad2).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eRoad);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eRoad2D);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eRoad2);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	// Cross Road
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eCrossRoad).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eCrossRoad);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eCrossRoadD);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eCrossRoad);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	// Center Road
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eCenterRoad).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eCenterRoad);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eCenterRoadD);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eCenterRoad);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [Building]
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
		pObject->CreateAxisObject(m_pd3dDevice);

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
		pObject->CreateAxisObject(m_pd3dDevice);

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

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding77);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding77, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding77D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding78);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding78, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding78D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding100);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding100, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding100D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding103);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding103, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding103D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding104);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding104, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eBuilding103D);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eParkingLot);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eParkingLot, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pObject->SetMaterial(1, TextureTag::eParkingLotD);
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);

		pObject->CreateBoundingBox(m_pd3dDevice);

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}
#pragma endregion

#pragma region [Bench]
	// Bench
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eBench).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBench);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eBenchD);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBench);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [Grass]
	/*
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eGrass).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBench);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eGrassD);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eGrass);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	*/

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
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eBusStop).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBusStop);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eBusStopD);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBusStop);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [Street Lamp]
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eStreetLamp).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eStreetLamp);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eStreetLamp);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [Barricade]
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eBarricade).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBarricade);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eBarricadeD);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBarricade);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [SideWalk1]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk1);
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk1).size());
	pMesh = new CCubeMeshTexturedIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);

	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eSideWalk1D);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
	//	pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [SideWalk2]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk2);
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk2).size());
	pMesh = new CCubeMeshTexturedIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);

	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eSideWalk2D);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		//	pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
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
	CUIObject* pUIObject;
	// Aim
	pUIObject = new CUIObject(TextureTag::eAim);
	POINT aimingPos = POINT{ FRAME_BUFFER_WIDTH / 2 + 3, FRAME_BUFFER_HEIGHT / 2 - 22};		// 오프셋 (3, -14)			// +가 오른쪽, +가 아래쪽
	pUIObject->Initialize(m_pd3dDevice, POINT{ aimingPos.x - 20, aimingPos.y - 20 }, POINT{ aimingPos.x + 20, aimingPos.y + 20 }, 0.0f);
	m_pUIManager->AddUIObject(pUIObject);
	pUIObject->SetActive(false);
	
	// Score
	pUIObject = new CUIObject(TextureTag::eScoreUI);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 600, 0 }, POINT{ 1000, 90 }, 0.0f);
	m_pUIManager->AddUIObject(pUIObject);

	// Life
	pUIObject = new CUIObject(TextureTag::eLifeUI);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 10, FRAME_BUFFER_HEIGHT - 190 }, POINT{ 360, FRAME_BUFFER_HEIGHT - 10 }, 0.0f);
	m_pUIManager->AddUIObject(pUIObject);

	// Magazine
	pUIObject = new CUIObject(TextureTag::eMagazineUI);
	pUIObject->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH - 260, FRAME_BUFFER_HEIGHT - 140 }, POINT{ FRAME_BUFFER_WIDTH - 10, FRAME_BUFFER_HEIGHT - 10}, 0.0f);
	m_pUIManager->AddUIObject(pUIObject);
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

void CMainScene::CalcTime()
{
	if (GetTickCount() - m_dwTime > 1000) {
		m_nGameTime--;
		m_dwTime = GetTickCount();
	}
}

void CMainScene::Update(float fDeltaTime)
{
	// 충돌 정보 갱신
	COLLISION_MGR->InitCollisionInfo();
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

	CalcTime();
}

void CMainScene::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{

#ifdef USE_DEFERRD_RENDER
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
#else
	CScene::Render(pd3dDeviceContext, pCamera);

	if (m_pTerrain)
		if (m_pTerrain->IsVisible(pCamera))
			m_pTerrain->Render(pd3dDeviceContext, pCamera);

	m_pParticleSystem->Render(pd3dDeviceContext);

	for (auto& object : m_vecCharacterContainer)
		object->Render(m_pd3dDeviceContext, m_pCamera);

	// ------ End Scene Rendering ------ //
	m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);

	m_pUIManager->RenderAll(pd3dDeviceContext);
	// =============== Rendering Option =================== //

	for (auto& lineObject : GLOBAL_MGR->g_vecLineContainer)
		lineObject->Render(m_pd3dDeviceContext, m_pCamera);

	if (GLOBAL_MGR->g_bShowWorldAxis)
		m_pWorldCenterAxis->Render(pd3dDeviceContext, pCamera);

	if (GLOBAL_MGR->g_vRenderOption.y)
		RenderBoundingBox();
#endif
}

void CMainScene::RenderBoundingBox()
{
	m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pWireframeRS);
	m_pBoundingBoxShader->OnPrepareSetting(m_pd3dDeviceContext, false);

	for (auto object : m_vecBBoxRenderContainer) {
		if (m_bIsPreCollisionCheck != object->GetCollisionCheck()) {
			m_bIsPreCollisionCheck = object->GetCollisionCheck();
			m_pBoundingBoxShader->OnPrepareSetting(m_pd3dDeviceContext, object->GetCollisionCheck());
		}
//		m_pBoundingBoxShader->OnPrepareSetting(m_pd3dDeviceContext, object->GetCollisionCheck());
		object->BoundingBoxRender(m_pd3dDeviceContext);
	}

	m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);
}

void CMainScene::RenderAllText(ID3D11DeviceContext *pd3dDeviceContext)
{
	string str;

	// Draw Position
	XMFLOAT3 playerPos = m_pPlayer->GetPosition();
	DWORD	 HP = m_pPlayer->GetPlayerLife();
	DWORD	 ID = SERVER_MGR->GetId();

	XMVECTOR temp = XMVector3LengthEst(m_pPlayer->GetvPosition());
	str = "Player Position : (" + to_string(playerPos.x) + ", " + to_string(playerPos.y) + ", " + to_string(playerPos.z) + ")\n";
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 30, 20, 50, 0xFFFFFFFF, FW1_LEFT);

	str = "원점으로부터의 거리 : (" + to_string(XMVectorGetX(temp)) + ")\n";		// 3D 사운드 용
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 30, 20, 90, 0xFFFFFFFF, FW1_LEFT);

	str = "HP : (" + to_string(HP) + ")\n";
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 30, 20, 130, 0xFFFFFFFF, FW1_LEFT);

	str = "ID : (" + to_string(ID) + ")\n";
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 30, 500, 50, 0xFFFFFFFF, FW1_LEFT);

	// Draw Select Object
	if (m_pSelectedObject) {
		XMFLOAT3 pos = m_pSelectedObject->GetPosition();
		str = "Position : (" + to_string(pos.x) + ", " + to_string(pos.y) + to_string(pos.z) + ")";

		//TEXT_MGR->RenderText(pd3dDeviceContext, ppos, 30, 20, 80, 0xFFFFFFFF, FW1_LEFT);
	}

	// ================ Draw UI Text ================ //

	// ----- Magazine ------ //
	str = "M16A1";
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 46, 1430, 765, 0xFFCCCCCC, FW1_LEFT);

	UINT nBulletCount = m_pPlayer->GetWeaponBulletCount();
	str = to_string(nBulletCount);
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 53, 1465, 823, 0xFFFFFFFF, FW1_RIGHT);

	UINT nMaxBulletCount = m_pPlayer->GetWeaponMaxBulletCount();
	str = " /  " + to_string(nMaxBulletCount);
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 53, 1470, 823, 0xFFBBBBBB, FW1_LEFT);

	// ----- Life, ArmorPoint ----- //
	UINT nPlayerLife = m_pPlayer->GetPlayerLife();
	str = to_string(nPlayerLife);
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 48, 210, 743, 0xFFFFFFFF, FW1_RIGHT);

	UINT nPlayerArmorPoint = m_pPlayer->GetPlayerArmorPoint();
	str = to_string(nPlayerArmorPoint);
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 48, 255, 815, 0xFFFFFFFF, FW1_RIGHT);

	// ----- Time ----- //
	UINT nMinute = m_nGameTime / 60;
	UINT nSecond = m_nGameTime % 60;
	if (nMinute < 10)
		str = '0' + to_string(nMinute);
	else
		str = to_string(nMinute);
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 48, 793, 85, 0xFFFFFFFF, FW1_RIGHT);

	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(":"), 48, 804.3f, 85, 0xFFFFFFFF, FW1_RIGHT);

	if (nSecond < 10)
		str = '0' + to_string(nSecond);
	else
		str = to_string(nSecond);
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 48, 843, 85, 0xFFFFFFFF, FW1_RIGHT);

	// ----- Total Kill ----- //
	str = to_string(m_nRedTeamTotalKill);
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 60, 698, 10, 0xFF0020FF, FW1_CENTER);

	str = to_string(m_nBlueTeamTotalKill);
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 60, 913, 10, 0xFFFF4500, FW1_CENTER);
}