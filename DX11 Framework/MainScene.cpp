#include "stdafx.h"
#include "MainScene.h"
#include "protocol.h"

CMainScene::CMainScene()
{
	m_tagScene = SceneTag::eMainScene;
	//TWBAR_MGR->g_xmf3SelectObjectPosition = XMFLOAT3(0, 0, 0);
	TWBAR_MGR->g_xmf3SelectObjectRotate = XMFLOAT3(40, 50, 30.0f);

	m_f3DirectionalColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_f3DirectionalDirection = XMFLOAT3(1.0f, -1.0f, 1.0f);

	m_f3DirectionalAmbientUpperColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	m_f3DirectionalAmbientLowerColor = XMFLOAT3(0.5f, 0.5f, 0.5f);

#ifdef DEVELOP_MODE
	//m_f3DirectionalAmbientUpperColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
#endif
}

CMainScene::~CMainScene()
{
	delete(m_GBuffer);
	delete(m_pLightManager);
	delete(m_PostFX);

	ReleaseCOM(m_pHDRTexture);
	ReleaseCOM(m_HDRRTV);
	ReleaseCOM(m_HDRSRV);
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
			case VK_R: 
				m_pPlayer->SetKeyDown(KeyInput::eReload);
				break;
			case VK_E:
				m_pPlayer->SetKeyDown(KeyInput::eOccupy);
				break;
			case VK_F1:
			case VK_F2:
			case VK_F3:
					m_pPlayer->ChangeCamera(m_pd3dDevice, CameraTag(wParam - VK_F1 + 1));
					m_pCamera = m_pPlayer->GetCamera();
					SCENE_MGR->g_pCamera = m_pCamera;

					if (wParam - VK_F1 == 2)
						m_pUIManager->GetUIObject(TextureTag::eAim)->SetActive(false);
					else 
						m_pUIManager->GetUIObject(TextureTag::eAim)->SetActive(true);
				break;
			case VK_F4:	
				m_pPlayer->SetPosition(XMVectorSet(60, 10, 30, 0));
			//	m_pPlayer->SetPosition(XMVectorSet(60, 10, 100, 0));
				m_pPlayer->SetGravityTimeElpased(0.0f);

				m_pPlayer->SetVelocity(XMFLOAT3(0, 0, 0));
				break;
			case VK_F5:
				TWBAR_MGR->g_OptionHDR.g_fWhite -= 0.1f;
			//	TWBAR_MGR->g_xmf4TestVariable.x = 1.0f;
				break;
			case VK_F6:
				TWBAR_MGR->g_OptionHDR.g_fWhite = 1.5f;
			//	TWBAR_MGR->g_xmf4TestVariable.x = 0.0f;
				break;
			break;
#ifndef USE_SERVER
			case VK_Z:
				// 임의로 죽어보기
				m_pPlayerCharacter->SetDeath();
				break;
			case VK_X:
				// 임의로 헤드샷 맞기
				m_pPlayerCharacter->SetIsDeadlyAttack(true);
				break;
			case VK_C:
				// 임의로 위급 상황
				m_pPlayerCharacter->SetLife(20);
				break;
#endif
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
			case VK_E:	
				m_pPlayer->SetKeyUp(KeyInput::eOccupy);
				break;
			}
			break;
		}

	return(false);
}

void CMainScene::OnChangedWindowsSize(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	ReleaseCOM(m_pHDRTexture);
	ReleaseCOM(m_HDRRTV);
	ReleaseCOM(m_HDRSRV);

	D3D11_TEXTURE2D_DESC dtd = {
		LOWORD(lParam), HIWORD(lParam),
		1, //UINT MipLevels;
		1, //UINT ArraySize;
		DXGI_FORMAT_R16G16B16A16_TYPELESS, //DXGI_FORMAT Format;
		1, //DXGI_SAMPLE_DESC SampleDesc;
		0,
		D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,//UINT BindFlags;
		0,//UINT CPUAccessFlags;
		0//UINT MiscFlags;    
};
	HR(m_pd3dDevice->CreateTexture2D(&dtd, NULL, &m_pHDRTexture));
	DXUT_SetDebugName(m_pHDRTexture, "HDR Light Accumulation Texture");

	D3D11_RENDER_TARGET_VIEW_DESC rtsvd =
	{
		DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_RTV_DIMENSION_TEXTURE2D
	};
	HR(m_pd3dDevice->CreateRenderTargetView(m_pHDRTexture, &rtsvd, &m_HDRRTV));
	DXUT_SetDebugName(m_HDRRTV, "HDR Light Accumulation RTV");

	D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd = 
	{
		DXGI_FORMAT_R16G16B16A16_FLOAT,	D3D11_SRV_DIMENSION_TEXTURE2D, 0, 0 
	};
	dsrvd.Texture2D.MipLevels = 1;
	HR(m_pd3dDevice->CreateShaderResourceView(m_pHDRTexture, &dsrvd, &m_HDRSRV));
	DXUT_SetDebugName(m_HDRSRV, "HDR Light Accumulation SRV");

	m_GBuffer->Initialize(m_pd3dDevice, LOWORD(lParam), HIWORD(lParam));
	m_PostFX->Initialize(m_pd3dDevice, LOWORD(lParam), HIWORD(lParam));
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
	m_PostFX = new CPostFX();

	m_pLightManager = new CLightManager();
	m_pLightManager->Initialize(m_pd3dDevice);
	m_pLightManager->SetGBuffer(m_GBuffer);
//	m_pSphereObject = new CSphereObject();
//	m_pSphereObject->CreateObjectData(m_pd3dDevice);
//	m_vecObjectsContainer.push_back(m_pSphereObject);

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

#ifdef USE_SERVER
	cs_create_charter packet;

	packet.size = sizeof(cs_create_charter);
	packet.type = 8;

	SERVER_MGR->Sendpacket(reinterpret_cast<BYTE *>(&packet));
#endif

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

#ifndef USE_SERVER
	// ==== Test용 - 총 메쉬 오프셋 찾기용 ==== //
	CCharacterObject* pCharacter = new CTerroristCharacterObject(TeamType::eBlueTeam);
	pCharacter->CreateObjectData(m_pd3dDevice);
	pCharacter->CreateAxisObject(m_pd3dDevice);
//	pCharacter->SetLife(100);
	pCharacter->SetLife(100000);	// 파티클 테스트용
	pCharacter->SetPosition(60.0f, 2.5f, 15.0f);

	m_vecBBoxRenderContainer.push_back(pCharacter);
	m_vecCharacterContainer.push_back(pCharacter);

	COLLISION_MGR->m_vecCharacterContainer.push_back(pCharacter);
#endif

	PARTICLE_MGR->CreateParticleSystems(m_pd3dDevice);

//	CreateMapDataObject();
	CreateMapDataInstancingObject();//	- 테스트용으로 잠시 맵  생성 제거
//	CreateTestingObject();

	CreateLights();
	CreateConstantBuffers();
	CreateTweakBars();
	CreateUIImage();

	SOUND_MGR->PlayBgm(SoundTag::eBGM_Rain);
	m_dwLastLightningTime = GetTickCount();
	cout << "================================== Scene Loading Complete ===================================" << endl;
	cout << "=============================================================================================" << endl << endl;
}

void CMainScene::CreateMapDataObject()
{
	// Mesh
	CMesh* pMesh = nullptr;
	CFbxModelMesh* pFbxMesh = nullptr;

	// Object
	CGameObject* pObject = nullptr;
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
	CMesh* pMesh = nullptr;
	CFbxModelMesh* pFbxMesh = nullptr;

	// Object
	CGameObject* pObject = nullptr;
	vector<MapData> vecMapData;

	// Shader
	CInstancedObjectsShader* pInstancingShaders = nullptr;
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

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
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

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
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

#pragma region [Street Lamp]
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eStreetLamp).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eStreetLamp);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eStreetLampD, TextureTag::eStreetLampN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eStreetLamp);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
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
	pMesh = new CCubeMeshTexturedTangenteIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);
//	pMesh = new CCubePatchMesh(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);
	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eSideWalk1D, TextureTag::eSideWalk1ND);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation.x - 90, vecMapData[count].m_Rotation.y - 180, vecMapData[count].m_Rotation.z);
		pObject->CreateBoundingBox(m_pd3dDevice, pMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
//		pInstancingShaders->AddObject(ShaderTag::eInstanceDisplacementMapping, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [SideWalk2]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk2);
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk2).size());
	pMesh = new CCubeMeshTexturedTangenteIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);

	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eSideWalk2D, TextureTag::eSideWalk2N);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		//	pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion
	
#pragma region [StoneWall]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eStoneWall);
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eStoneWall).size());
	pMesh = new CCubeMeshTexturedTangenteIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);

	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eStoneWallD, TextureTag::eStoneWallN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation.x - 90, vecMapData[count].m_Rotation.y - 180, vecMapData[count].m_Rotation.z);
		pObject->CreateBoundingBox(m_pd3dDevice, pMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [Container]
	// Container 1
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eContainer1Red).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eContainer1);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eContainerRedD, TextureTag::eContainerN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eContainer1Red);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eContainer1Blue).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eContainer1);
	pFbxMesh->Initialize(m_pd3dDevice, true);

	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eContainerBlueD, TextureTag::eContainerN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eContainer1Blue);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eContainer1Yellow).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eContainer1);
	pFbxMesh->Initialize(m_pd3dDevice, true);

	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eContainerYellowD, TextureTag::eContainerN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eContainer1Yellow);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	
	// Container 2
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eContainer2Red).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eContainer2);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eContainerRedD, TextureTag::eContainerN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eContainer2Red);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eContainer2Blue).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eContainer2);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eContainerBlueD, TextureTag::eContainerN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eContainer2Blue);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eContainer2Yellow).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eContainer2);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eContainerYellowD, TextureTag::eContainerN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eContainer2Yellow);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	// Container 3
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eContainer3Red).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eContainer3);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eContainerRedD, TextureTag::eContainerN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eContainer3Red);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eContainer3Blue).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eContainer3);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eContainerBlueD, TextureTag::eContainerN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eContainer3Blue);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eContainer3Yellow).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eContainer3);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eContainerYellowD, TextureTag::eContainerN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eContainer3Yellow);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
	
#pragma endregion

#pragma region [Barrel]
	// Fire Barrel 1
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eFireBarrel).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eFireBarrel);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eFireBarrelD, TextureTag::eFireBarrelN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eFireBarrel);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	// Barrel
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eBarrel).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBarrel);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eBarrelD, TextureTag::eBarrelN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBarrel);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	// Occupy Barrel
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eOccupyBarrel).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eOccupyBarrel);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eOccupyBarrelD, TextureTag::eOccupyBarrelN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eOccupyBarrel);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pObject = new CGameObject();
		pObject->SetMesh(pFbxMesh);
		pObject->SetPosition(vecMapData[count].m_Position);
		pObject->Rotate(vecMapData[count].m_Rotation);
		pObject->CreateBoundingBox(m_pd3dDevice, pFbxMesh);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion
}

void CMainScene::CreateTestingObject()
{
	// Mesh
	CMesh* pMesh = nullptr;
	CFbxModelMesh* pFbxMesh = nullptr;

	// Object
	CGameObject* pObject = nullptr;
	vector<MapData> vecMapData;

	// Shader
	CInstancedObjectsShader* pInstancingShaders = nullptr;

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

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
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

		AddShaderObject(ShaderTag::eNormalTexture, pObject);
	}
#pragma endregion


}

void CMainScene::CreateTweakBars()
{
	TwBar *tweakBar = TWBAR_MGR->g_tweakBar;

	TwAddVarRW(tweakBar, "Light direction", TW_TYPE_DIR3F, &m_f3DirectionalDirection, "group = Light opened = true axisz = -z");
	TwAddVarRW(tweakBar, "Directional Color X", TW_TYPE_FLOAT, &m_f3DirectionalColor.x, "group = Light min = 0 max = 1 step = 0.05");
	TwAddVarRW(tweakBar, "Directional Color Y", TW_TYPE_FLOAT, &m_f3DirectionalColor.y, "group = Light min = 0 max = 1 step = 0.05");
	TwAddVarRW(tweakBar, "Directional Color Z", TW_TYPE_FLOAT, &m_f3DirectionalColor.z, "group = Light min = 0 max = 1 step = 0.05");


	TwAddVarRW(tweakBar, "Ambient Upper Color X", TW_TYPE_FLOAT, &m_f3DirectionalAmbientUpperColor.x, "group = Light min = 0 max = 1 step = 0.05");
	TwAddVarRW(tweakBar, "Ambient Upper Color Y", TW_TYPE_FLOAT, &m_f3DirectionalAmbientUpperColor.y, "group = Light min = 0 max = 1 step = 0.05");
	TwAddVarRW(tweakBar, "Ambient Upper Color Z", TW_TYPE_FLOAT, &m_f3DirectionalAmbientUpperColor.z, "group = Light min = 0 max = 1 step = 0.05");
	TwAddVarRW(tweakBar, "Ambient Lower Color X", TW_TYPE_FLOAT, &m_f3DirectionalAmbientLowerColor.x, "group = Light min = 0 max = 1 step = 0.05");
	TwAddVarRW(tweakBar, "Ambient Lower Color Y", TW_TYPE_FLOAT, &m_f3DirectionalAmbientLowerColor.y, "group = Light min = 0 max = 1 step = 0.05");
	TwAddVarRW(tweakBar, "Ambient Lower Color Z", TW_TYPE_FLOAT, &m_f3DirectionalAmbientLowerColor.z, "group = Light min = 0 max = 1 step = 0.05");

	

	TwDefine("TweakBar/Light opened=true ");
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
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);
	
	// Score
	pUIObject = new CUIObject(TextureTag::eScoreUI);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 600, 0 }, POINT{ 1000, 90 }, 0.5f);
	m_pUIManager->AddUIObject(pUIObject);

	// Life
	pUIObject = new CUIObject(TextureTag::eLifeUI);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 10, FRAME_BUFFER_HEIGHT - 190 }, POINT{ 360, FRAME_BUFFER_HEIGHT - 10 }, 0.5f);
	m_pUIManager->AddUIObject(pUIObject);

	// Magazine
	pUIObject = new CUIObject(TextureTag::eMagazineUI);
	pUIObject->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH - 260, FRAME_BUFFER_HEIGHT - 140 }, POINT{ FRAME_BUFFER_WIDTH - 10, FRAME_BUFFER_HEIGHT - 10}, 0.5f);
	m_pUIManager->AddUIObject(pUIObject);

	// Damaged Character
	pUIObject = new CUIObject(TextureTag::eDamagedCharacterUI);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT }, 0.0f);
	pUIObject->AddOpacity(-1.0f);
	m_pUIManager->AddUIObject(pUIObject);
	m_pDamageUI = pUIObject;

	// Respawn Gage
	pUIObject = new CUIObject(TextureTag::eRespawnGageBar);
	pUIObject->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 305, FRAME_BUFFER_HEIGHT / 2 + 40 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 305 , FRAME_BUFFER_HEIGHT / 2 + 65 }, 0.1f);
	pUIObject->AddOpacity(-0.3f);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eRespawnGageWhiteBar);
	pUIObject->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 300, FRAME_BUFFER_HEIGHT / 2 + 43 }, POINT{ FRAME_BUFFER_WIDTH / 2 - 300 , FRAME_BUFFER_HEIGHT / 2 + 62 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eOccupyGageBar);
	pUIObject->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 305, FRAME_BUFFER_HEIGHT / 2 + 40 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 305 , FRAME_BUFFER_HEIGHT / 2 + 65 }, 0.1f);
	pUIObject->AddOpacity(-0.3f);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eOccupyGageWhiteBar);
	pUIObject->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 300, FRAME_BUFFER_HEIGHT / 2 + 43 }, POINT{ FRAME_BUFFER_WIDTH / 2 - 300 , FRAME_BUFFER_HEIGHT / 2 + 62 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);
	
}

void CMainScene::ReleaseObjects()
{
	CScene::ReleaseObjects();
	ReleaseConstantBuffers();

	SafeDelete(m_pBoundingBoxShader);

	for (auto& object : m_vecCharacterContainer)
		SafeDelete(object);

	m_pLightManager->DeInitialize();
}

void CMainScene::AddShaderObject(ShaderTag tag, CGameObject* pObject)
{
	m_vecBBoxRenderContainer.push_back(pObject);
	m_vecShaderObjectContainer.AddObject(tag, pObject);
	COLLISION_MGR->m_vecStaticMeshContainer.push_back(pObject);
}

void CMainScene::CreateConstantBuffers()
{
	// Test Constant Buffer	
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT4);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = &TWBAR_MGR->g_xmf4TestVariable;
	HR(m_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbTestVariable));

	DXUT_SetDebugName(m_pd3dcbTestVariable, "Test Variable");
}

void CMainScene::UpdateConstantBuffers()
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	m_pd3dDeviceContext->Map(m_pd3dcbTestVariable, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMFLOAT4 *pcbTest = (XMFLOAT4 *)d3dMappedResource.pData;
	memcpy(pcbTest, &TWBAR_MGR->g_xmf4TestVariable, sizeof(XMFLOAT4));
	m_pd3dDeviceContext->Unmap(m_pd3dcbTestVariable, 0);
	m_pd3dDeviceContext->VSSetConstantBuffers(CB_SLOT_TEST, 1, &m_pd3dcbTestVariable);
	m_pd3dDeviceContext->GSSetConstantBuffers(CB_SLOT_TEST, 1, &m_pd3dcbTestVariable);
	m_pd3dDeviceContext->PSSetConstantBuffers(CB_SLOT_TEST, 1, &m_pd3dcbTestVariable);
}

void CMainScene::ReleaseConstantBuffers()
{
	ReleaseCOM(m_pd3dcbLights);
	ReleaseCOM(m_pd3dcbTestVariable);
}

void CMainScene::CreateLights()
{
	m_pLightManager->ClearLights();

	vector<MapData> vecMapData;
	XMFLOAT3 pos;
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eStreetLamp);
	
	// Street Lamp
	for (auto light : vecMapData) {
		pos = light.m_Position;
		pos.y += 10.0f;
		m_pLightManager->AddSpotLight(pos, XMFLOAT3(0, -1, 0), 45.0f, 50.0f, 30.0f, XMFLOAT3(1, 1, 1));
	}
	
	// Player Light
	{
		pos = m_pPlayer->GetPosition();
		XMFLOAT3 look = m_pPlayerCharacter->GetFireDirection();
		//m_pLightManager->AddSpotLight(pos, look, TWBAR_MGR->g_xmf3SelectObjectRotate.x, TWBAR_MGR->g_xmf3SelectObjectRotate.y, TWBAR_MGR->g_xmf3SelectObjectRotate.z, XMFLOAT3(1, 1, 1));
		m_pLightManager->AddSpotLight(pos, look, 35.0f, 30.0f, 10.0f, XMFLOAT3(1, 1, 1));

		XMVECTOR vPos = m_pPlayer->GetvPosition();
		vPos += 1.3f * m_pPlayer->GetvLook();

		XMStoreFloat3(&pos, vPos);
		m_pLightManager->AddPointLight(pos, 5.0f, XMFLOAT3(1, 1, 1));
	}
}

void CMainScene::ModifiedSelectObject()
{
	XMFLOAT3 pos = m_pSelectedObject->GetPosition();

//	TWBAR_MGR->g_xmf3SelectObjectPosition = pos;
	
//	m_pSelectedObject->SetPosition(TWBAR_MGR->g_xmf3SelectObjectPosition);
//	m_pSelectedObject->SetRotate(TWBAR_MGR->g_xmf3SelectObjectRotate);
	
	//cout << "Select Object Postion : "; ShowXMFloat3(pos);

	m_pSelectedObject = nullptr;
}

void CMainScene::CalcTime()
{
	if (GetTickCount() - m_dwTime > 1000) {
		if (m_nGameTime <= 0)
			return;
		m_nGameTime--;
		m_dwTime = GetTickCount();
	}
}

void CMainScene::PrepareRenderUI()
{
	// 중복해서 그리는 문제 존재(m_pUIManager 에서 추가 렌더링)
	m_pUIManager->Render(m_pd3dDeviceContext, TextureTag::eRespawnGageBar);
	m_pUIManager->Render(m_pd3dDeviceContext, TextureTag::eRespawnGageWhiteBar);

	m_pUIManager->Render(m_pd3dDeviceContext, TextureTag::eOccupyGageBar);
	m_pUIManager->Render(m_pd3dDeviceContext, TextureTag::eOccupyGageWhiteBar);
}

void CMainScene::RenderUI()
{
	ShowOccupyUI();
	ShowDeadlyAttackUI();
	ShowDeadlyUI();
	ShowDeathRespawnUI();

	// ------ UI ----- // 뎁스스텐실 뷰 오류로 인하여 미리 그려줌
	m_pUIManager->RenderAll(m_pd3dDeviceContext);
}

void CMainScene::ShowOccupyUI()
{

	CUIObject* pGageUI = m_pUIManager->GetUIObject(TextureTag::eOccupyGageBar);
	CUIObject* pWhiteGageUI = m_pUIManager->GetUIObject(TextureTag::eOccupyGageWhiteBar);

	if (!m_pPlayerCharacter->GetIsOccupy()) {
		pWhiteGageUI->SetActive(false);
		pGageUI->SetActive(false);
		return;
	}


	if (m_cOccupyteam != static_cast<int>(SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->GetTagTeam()))
	{
		pGageUI->SetActive(true);
		pWhiteGageUI->SetActive(true);
	}
	else
	{
		pGageUI->SetActive(false);
		pWhiteGageUI->SetActive(false);
	}
	const UINT gageLength = 600;	// UI x축 길이 600 
	float percentage = (float)(GetTickCount() - m_pPlayerCharacter->GetOccupyTime()) / OCCUPY_TIME;

	pWhiteGageUI->SetEndPos(POINT{ FRAME_BUFFER_WIDTH / 2 - 300 + (LONG)(percentage * gageLength), FRAME_BUFFER_HEIGHT / 2 + 62 });

	if (percentage >= 1)
	{
#ifdef	USE_SERVER
		{
			sc_occupy packet;
			packet.size = sizeof(sc_occupy);
			packet.type = 9;
			packet.redteam = static_cast<int>(SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->GetTagTeam());

			//cout << packet.redteam << endl;
			SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&packet));
		}
#endif
		pGageUI->SetActive(false);
		pWhiteGageUI->SetActive(false);

	}
}

void CMainScene::ShowDeathRespawnUI()
{
	CUIObject* pGageUI = m_pUIManager->GetUIObject(TextureTag::eRespawnGageBar);
	CUIObject* pWhiteGageUI = m_pUIManager->GetUIObject(TextureTag::eRespawnGageWhiteBar);

	if (!m_pPlayerCharacter->GetIsDeath()) {
		pWhiteGageUI->SetActive(false);
		pGageUI->SetActive(false);
		return;
	}

	m_pDamageUI->AddOpacity(-1.0f);
	pGageUI->SetActive(true);
	pWhiteGageUI->SetActive(true);

	const UINT gageLength = 600;	// UI x축 길이 600 
	float percentage = (float)(GetTickCount() - m_pPlayerCharacter->GetDeathTime()) / RESPAWN_TIME;

	pWhiteGageUI->SetEndPos(POINT{ FRAME_BUFFER_WIDTH / 2 - 300 + (LONG)(percentage * gageLength), FRAME_BUFFER_HEIGHT / 2 + 62 });
}

void CMainScene::ShowDeadlyUI()
{
	if (!m_pPlayer->GetIsDeadly())
		return;

	static bool bIsReverse = false;
	float opacityValue = 0.8f;

	if (bIsReverse) {
		m_pDamageUI->AddOpacity(-1 * opacityValue * m_fDeltaTime);

		if (m_pDamageUI->GetOpacity() <= 0.0f)
			bIsReverse = false;
	}
	else {
		m_pDamageUI->AddOpacity(opacityValue * m_fDeltaTime);

		if (m_pDamageUI->GetOpacity() >= 1.0f)
			bIsReverse = true;
	}
}

void CMainScene::ShowDeadlyAttackUI()
{
	if (!m_pPlayer->GetIsDeadlyAttack())
		return;

	static bool bIsDeadlyAttack = true;
	float opacityValue = 0.8f;

	if (bIsDeadlyAttack) {
		m_pDamageUI->AddOpacity(1.0f);
		bIsDeadlyAttack = false;
	}
	
	m_pDamageUI->AddOpacity(-1 * opacityValue * m_fDeltaTime);
	
	if (m_pDamageUI->GetOpacity() <= 0.0f) {
		bIsDeadlyAttack = true;
		m_pPlayer->SetIsDeadlyAttack(false);
	}
}

void CMainScene::Update_Lightning(float fDeltaTime)
{
	static bool isFirstLightning = false;
	static bool isLightning = false;
	DWORD dwFirstLightningInterval;
	static const DWORD dwLightningPeriod = 5000;

	/*
	cout << TWBAR_MGR->g_OptionHDR.g_fWhite << endl;

	if (GetTickCount() - m_dwLastLightningTime > dwLightningPeriod) {

		dwFirstLightningInterval = GetTickCount();

		TWBAR_MGR->g_OptionHDR.g_fWhite = 0.4f;
		isFirstLightning = true;

		cout << "ㅇㅇㅇㅇ"<< endl;
		if (GetTickCount() - dwFirstLightningInterval > 1000) {
			m_dwLastLightningTime = GetTickCount();
			isLightning = true;
			cout << "ㄴㄴㄴ" << endl;
		}
	}
	
	
	if (isFirstLightning) {
		TWBAR_MGR->g_OptionHDR.g_fWhite += 0.2f;

		if (TWBAR_MGR->g_OptionHDR.g_fWhite >= TWBAR_MGR->g_cfWhite)
			isFirstLightning = false;

	}
	*/

	if (isLightning) {
		TWBAR_MGR->g_OptionHDR.g_fWhite += 0.01f;

		if (TWBAR_MGR->g_OptionHDR.g_fWhite >= TWBAR_MGR->g_cfWhite)
			isLightning = false;
	}
	
	if (GetTickCount() - m_dwLastLightningTime> dwLightningPeriod) {	
		m_dwLastLightningTime = GetTickCount();

		TWBAR_MGR->g_OptionHDR.g_fWhite = 0.15f;
		isLightning = true;
	}
	
}

void CMainScene::Update(float fDeltaTime)
{
	// 충돌 정보 갱신
	COLLISION_MGR->InitCollisionInfo();	// 현재 플레이어만 적용되고있어서 주석처리함
//	COLLISION_MGR->UpdateManager();

	// ====== Update ===== //
	GLOBAL_MGR->UpdateManager();
	UpdateConstantBuffers();
	Update_Lightning(fDeltaTime);

	// ====== Object ===== //
	CScene::Update(fDeltaTime);

	for (auto& object : m_vecCharacterContainer)
		object->Update(fDeltaTime);

	// ====== Particle ===== //
	PARTICLE_MGR->UpdateManager(fDeltaTime);

	// ====== Timer ===== //
	CalcTime();

	// ====== Deferred ===== //
	float fAdaptationNorm;
	static bool s_bFirstTime = true;
	if (s_bFirstTime) {
		fAdaptationNorm = 0.0f;
		s_bFirstTime = false;
	}
	else {
		fAdaptationNorm = min(TWBAR_MGR->g_OptionHDR.g_fAdaptation < 0.0001f ? 1.0f : fDeltaTime / TWBAR_MGR->g_OptionHDR.g_fAdaptation, 0.9999f);
		//	fAdaptationNorm = fDeltaTime / 3;			// 1부터 10까지가 샘플 프로그램임
	}
	m_PostFX->SetParameters(TWBAR_MGR->g_OptionHDR.g_fMiddleGrey, TWBAR_MGR->g_OptionHDR.g_fWhite, fAdaptationNorm,
		TWBAR_MGR->g_OptionHDR.g_fBloomThreshold, TWBAR_MGR->g_OptionHDR.g_fBloomScale, TWBAR_MGR->g_OptionHDR.g_fDOFFarStart, TWBAR_MGR->g_OptionHDR.g_fDOFFarRange);

	// ===== Light ===== //
	m_pLightManager->SetAmbient(m_f3DirectionalAmbientLowerColor, m_f3DirectionalAmbientUpperColor);
	m_pLightManager->SetDirectional(m_f3DirectionalDirection, m_f3DirectionalColor);

	CreateLights();
}

void CMainScene::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
#ifdef USE_DEFERRD_RENDER
	// =============== Deferred Rendering ================== //
	if (GLOBAL_MGR->g_bEnablePostFX) {
		float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		//float fClearColor[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
		m_pd3dDeviceContext->ClearRenderTargetView(m_HDRRTV, fClearColor);
	}
	m_GBuffer->OnPreRender(pd3dDeviceContext);
#endif
	// ------ Start Scene Rendering ------ // 
	CScene::Render(pd3dDeviceContext, pCamera);

	for (auto& object : m_vecCharacterContainer)
		if(object->IsVisible(pCamera))
			object->Render(m_pd3dDeviceContext, m_pCamera);

	// ------ End Scene Rendering ------ //
	m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);

#ifdef USE_DEFERRD_RENDER
	m_GBuffer->OnPostRender(pd3dDeviceContext);
	m_GBuffer->OnPrepareForUnpack(pd3dDeviceContext);

	pd3dDeviceContext->OMSetRenderTargets(1, GLOBAL_MGR->g_bEnablePostFX ? &m_HDRRTV : &SCENE_MGR->g_pd3dRenderTargetView, m_GBuffer->GetDepthReadOnlyDSV());

	// ------ Final Scene Rendering ------ //
	m_GBuffer->DeferredRender(pd3dDeviceContext);
	m_pLightManager->DoLighting(pd3dDeviceContext, pCamera);

	// ----- UI ----- // 
	PrepareRenderUI();

	// ----- Particle System ----- //
	PARTICLE_MGR->RenderAllEffect(m_pd3dDeviceContext);
	
	if (GLOBAL_MGR->g_bShowLightVolume)
		m_pLightManager->DrawLightVolume(pd3dDeviceContext);

	if (GLOBAL_MGR->g_bEnablePostFX) {
		m_PostFX->PostProcessing(pd3dDeviceContext, m_HDRSRV, m_GBuffer->GetDepthView(), SCENE_MGR->g_pd3dRenderTargetView);
		pd3dDeviceContext->OMSetRenderTargets(1, &SCENE_MGR->g_pd3dRenderTargetView, m_GBuffer->GetDepthDSV());
	}
#endif

	PARTICLE_MGR->RenderAllNoEffect(m_pd3dDeviceContext);

	// =============== Rendering Option =================== //
	for (auto& lineObject : GLOBAL_MGR->g_vecLineContainer)
		lineObject->Render(m_pd3dDeviceContext, m_pCamera);

	if (GLOBAL_MGR->g_bShowWorldAxis)
		m_pWorldCenterAxis->Render(pd3dDeviceContext, pCamera);

	if (GLOBAL_MGR->g_vRenderOption.y)
		RenderBoundingBox();

	// Etc
	RenderUI();
	RenderAllText(m_pd3dDeviceContext);
	m_pd3dDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);

#ifdef USE_DEFERRD_RENDER
	if (GLOBAL_MGR->g_bShowGBuffer) {
		pd3dDeviceContext->OMSetRenderTargets(1, &SCENE_MGR->g_pd3dRenderTargetView, nullptr);
		m_GBuffer->Render(pd3dDeviceContext);
		pd3dDeviceContext->OMSetRenderTargets(1, &SCENE_MGR->g_pd3dRenderTargetView, m_GBuffer->GetDepthDSV());
	}
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
	DWORD HP = m_pPlayer->GetPlayerLife();
	DWORD ID = SERVER_MGR->GetId();

#ifdef DEVELOP_MODE
	str = "Player Position : (" + to_string(playerPos.x) + ", " + to_string(playerPos.y) + ", " + to_string(playerPos.z) + ")\n";
	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 30, 20, 50, 0xFFFFFFFF, FW1_LEFT);
#endif
//	str = "원점으로부터의 거리 : (" + to_string(XMVectorGetX(temp)) + ")\n";		// 3D 사운드 용
//	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 30, 20, 90, 0xFFFFFFFF, FW1_LEFT);

//	str = "ID : (" + to_string(ID) + ")\n";
//	TEXT_MGR->RenderText(pd3dDeviceContext, s_to_ws(str), 30, 500, 50, 0xFFFFFFFF, FW1_LEFT);
	
	// Draw Select Object
	if (m_pSelectedObject) {
		XMFLOAT3 pos = m_pSelectedObject->GetPosition();
		str = "Position : (" + to_string(pos.x) + ", " + to_string(pos.y) + to_string(pos.z) + ")";

		//TEXT_MGR->RenderText(pd3dDeviceContext, ppos, 30, 20, 80, 0xFFFFFFFF, FW1_LEFT);
	}

	// ================ Draw UI Text ================ //

	// ----- Magazine ------ //
	str = "M16A1";
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 46, 1430, 765, 0xFFCCCCCC, FW1_LEFT);

	UINT nBulletCount = m_pPlayer->GetWeaponBulletCount();
	str = to_string(nBulletCount);
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 53, 1465, 823, 0xFFFFFFFF, FW1_RIGHT);

	UINT nMaxBulletCount = m_pPlayer->GetWeaponMaxBulletCount();
	str = " /  " + to_string(nMaxBulletCount);
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 53, 1470, 823, 0xFFBBBBBB, FW1_LEFT);

	// ----- Life, ArmorPoint ----- //
	UINT nPlayerLife = m_pPlayer->GetPlayerLife();
	str = to_string(nPlayerLife);
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 48, 210, 743, 0xFFFFFFFF, FW1_RIGHT);

	UINT nPlayerArmorPoint = m_pPlayer->GetPlayerArmorPoint();
	str = to_string(nPlayerArmorPoint);
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 48, 255, 815, 0xFFFFFFFF, FW1_RIGHT);

	// ----- Time ----- //
	UINT nMinute = m_nGameTime / 60;
	UINT nSecond = m_nGameTime % 60;
	if (nMinute < 10)
		str = '0' + to_string(nMinute);
	else
		str = to_string(nMinute);
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 48, 793, 85, 0xFFFFFFFF, FW1_RIGHT);

	TEXT_MGR->RenderText(pd3dDeviceContext, ":", 48, 804.3f, 85, 0xFFFFFFFF, FW1_RIGHT);

	if (nSecond < 10)
		str = '0' + to_string(nSecond);
	else
		str = to_string(nSecond);
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 48, 843, 85, 0xFFFFFFFF, FW1_RIGHT);

	// ----- Total Kill ----- //
	str = to_string(m_nRedTeamTotalKill);
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 60, 698, 10, 0xFF0020FF, FW1_CENTER);

	str = to_string(m_nBlueTeamTotalKill);
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 60, 913, 10, 0xFFFF4500, FW1_CENTER);

	str = to_string(TOTAL_KILLS);
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 65, 800, 10, 0xFFFFFFFF, FW1_CENTER);

	// ----- Respawn ------ //
	if (m_pPlayerCharacter->GetIsDeath()) {
		str = "RESPAWN";
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 730, 430, 0xFFFFFFFF, FW1_LEFT);
	}

	/*
	if (m_pPlayerCharacter->GetIsOccupy()) {
		str = "점령중";
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 730, 430, 0xFFFFFFFF, FW1_LEFT);
	}
	*/
	if (m_cOccupyteam == 1) {
		str = "Red팀 점령";
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 1430, 430, 0xFFFFFFFF, FW1_LEFT);
	}
	if (m_cOccupyteam == 2) {
		str = "Blue팀 점령";
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 1400, 430, 0xFFFFFFFF, FW1_LEFT);
	}

	if (m_cOccupyteam != 0)
	{
		str = to_string(m_OccupyTime);
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 1500, 500, 0xFFFFFFFF, FW1_LEFT);
	}
//	if (m_pPlayerCharacter->GetIsOccupy()) {
//		str = "점령중";
//		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 730, 430, 0xFFFFFFFF, FW1_CENTER);
//	}
}