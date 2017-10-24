#include "stdafx.h"
#include "MainScene.h"
#include "protocol.h"

CMainScene::CMainScene()
{
	m_tagScene = SceneTag::eMainScene;
	m_tagGameMode = GameMode::eDeathMatch;

	m_f3DirectionalColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_f3DirectionalDirection = XMFLOAT3(1.0f, -1.0f, 1.0f);

	m_f3DirectionalAmbientUpperColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	m_f3DirectionalAmbientLowerColor = XMFLOAT3(0.5f, 0.5f, 0.5f);

//	TWBAR_MGR->g_xmf3Offset = XMFLOAT3(0.3f, 0.0f, -0.35f);
//	TWBAR_MGR->g_xmf3Rotate = XMFLOAT3(765.0f, 823, 100.0f);
//	TWBAR_MGR->g_xmf3Quaternion = XMFLOAT4(1305.0f, 30.0f, 90.0f, 45.0f);
	TWBAR_MGR->g_xmf4TestVariable = XMFLOAT4(900.0f, 1600.0f, 0.0f, 0.0f);
//	TWBAR_MGR->g_nSelect = 730;
	
}

CMainScene::~CMainScene()
{
	SafeDelete(m_GBuffer);
	SafeDelete(m_PostFX);
	SafeDelete(m_pSSReflection);

	ReleaseCOM(m_pHDRTexture);
	ReleaseCOM(m_HDRRTV);
	ReleaseCOM(m_HDRSRV);
}

bool CMainScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CScene::OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	m_pPlayer->OnKeyInputUpdate(nMessageID, wParam);

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
		break;
	default:
		break;
	}
	return(false);
}

bool CMainScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CScene::OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	m_pPlayer->OnKeyInputUpdate(nMessageID, wParam);

	switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_F1:
			case VK_F2:
			case VK_F3:
				m_pPlayer->ChangeCamera(m_pd3dDevice, CameraTag(wParam - VK_F1 + 1));
				m_pCamera = m_pPlayer->GetCamera();
				break;
			case VK_F4:	
				m_pPlayer->SetPosition(XMFLOAT3(60.0f, 30.0f, 20.0f));
				//m_pPlayer->SetPosition(XMFLOAT3(60.0f, 30.0f, 210.0f));
				break;
#ifndef USE_SERVER
			case VK_Z:
				// 임의로 죽어보기
				m_pPlayerCharacter->SetLife(0);
				break;
			case VK_X:
				// 임의로 헤드샷 맞기
				m_pPlayerCharacter->SetIsDeadlyAttack(true);
				break;
			case VK_C:
				// 임의로 위급 상황
				m_pPlayerCharacter->SetLife(20);
				break;
			case VK_F:	
				m_vecCharacterContainer.back()->SetIsFire(true);
				break;
			case VK_V:
				m_vecCharacterContainer.back()->SetIsFire(false);
				break;
			case VK_G:
				CreateKillLog(m_vecCharacterContainer[0], "하이하이", true);
				break;
			case VK_B:
				CreateKillLog(m_vecCharacterContainer[1], "dddddddddd", false);
				break;

#endif
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
	m_pSSReflection->Initialize(m_pd3dDevice, LOWORD(lParam), HIWORD(lParam));
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

	InitializePhysX();
	DECAL_MGR->InitializeManager();
	SPRITE_MGR->InitializeManager();
	LIGHT_MGR->InitializeManager();
	LIGHT_MGR->SetGBuffer(m_GBuffer);

	m_pSSReflection = new CSSReflection();

#pragma region [Create SkyBox]
#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
	CTexture *pSkyboxTexture = new CTexture(1, 1, PS_TEXTURE_SLOT_SKYBOX, PS_SAMPLER_SLOT_SKYBOX);
#else
	#ifdef _WITH_SKYBOX_TEXTURE_CUBE
		CTexture *pSkyboxTexture = new CTexture(1, 1, PS_TEXTURE_SLOT_SKYBOX, PS_SAMPLER_SLOT_LINEAR_WRAP);
	#else
		CTexture *pSkyboxTexture = new CTexture(6, 1, PS_TEXTURE_SLOT_SKYBOX, PS_SAMPLER_SLOT_SKYBOX);
	#endif
#endif
	pSkyboxTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWrapSS);
	
	CMaterial *pSkyboxMaterial = new CMaterial(NULL);
	pSkyboxMaterial->SetTexture(pSkyboxTexture);
	OnChangeSkyBoxTextures(m_pd3dDevice, pSkyboxMaterial, 0);

	CSkyBoxMesh *pSkyBoxMesh = new CSkyBoxMesh(m_pd3dDevice, 1000.0f, 1000.0f, 1000.0f);
	CSkyBox *pSkyBox = new CSkyBox(m_pd3dDevice);
	pSkyBox->SetMesh(pSkyBoxMesh, 0);
	pSkyBox->SetMaterial(pSkyboxMaterial);

	CShader *pSkyBoxShader = new CSkyBoxShader();
	pSkyBoxShader->CreateShader(m_pd3dDevice);
	pSkyBox->SetShader(pSkyBoxShader);
	
	m_pSkyBox = move(pSkyBox);
#pragma endregion

#pragma region [Create Character]
	CScene::CreatePlayer();
	m_vecBBoxRenderContainer.push_back(m_pPlayerCharacter);
	m_vecCharacterContainer.push_back(m_pPlayerCharacter);

#ifdef USE_SERVER
	cs_create_charter packet;

	packet.size = sizeof(cs_create_charter);
	packet.type = CS_CreatePlayer;
	strcpy(packet.characterID, m_pPlayerCharacter->GetID().c_str());
	SERVER_MGR->Sendpacket(reinterpret_cast<BYTE *>(&packet));
#endif

#pragma endregion 

#ifndef USE_SERVER
	// ==== Test용 - 총 메쉬 오프셋 찾기용 ==== //
	CCharacterObject* pCharacter = new CTerroristCharacterObject(TeamTag::eBlueTeam);
	pCharacter->SetServerID(1);
	pCharacter->SetID("테스트");
	pCharacter->CreateObjectData(m_pd3dDevice);
	pCharacter->CreateAxisObject(m_pd3dDevice);

#ifdef DEVELOP_MODE
//	pCharacter->SetLife(10000);	// 파티클 테스트용
	pCharacter->SetLife(100);
#else
	pCharacter->SetLife(100);
#endif
	pCharacter->SetPosition(78.0f, 2.8f, 20.0f);

	m_vecBBoxRenderContainer.push_back(pCharacter);
	m_vecCharacterContainer.push_back(pCharacter);
	COLLISION_MGR->m_vecCharacterContainer.push_back(pCharacter);
#endif

#ifdef FASTLOAD_MODE
	CreateTestingObject();
#else
	CreateMapDataInstancingObject();
	PARTICLE_MGR->CreateParticleSystems(m_pd3dDevice);
	CreateLights();
#endif

	CreateConstantBuffers();
	CreateTweakBars();
	CreateUIImage();
//	CreateSound();		 불소리가 안남. 채널 문제인듯

	m_nGameTime = DEATHMATCH_TIME;
	SOUND_MGR->PlayBgm(SoundTag::eBGM_Rain, 1.0f);
	m_dwLastLightningTime = GetTickCount();
	cout << "================================== Scene Loading Complete ===================================" << endl;
	cout << "=============================================================================================" << endl << endl;
}

void CMainScene::CreateMapDataInstancingObject()
{
	// Mesh
	CMesh* pMesh = nullptr;
	CFbxModelMesh* pFbxMesh = nullptr;

	// Object
	CGameObject* pObject = nullptr;
	CPhysXObject* pPhysXObject = nullptr;
	vector<MapData> vecMapData;

	// Shader
	CInstancedObjectsShader* pInstancingShaders = nullptr;
#pragma region [Plane]
	pObject = new CGameObject();
	pMesh = new CCubeMeshIlluminated(m_pd3dDevice, 280, 1, 300);

	pObject->SetMesh(pMesh);
	pObject->SetPosition(XMFLOAT3(140, -0.5f, 150));
	pObject->CreateBoundingBox(m_pd3dDevice);

	AddShaderObject(ShaderTag::eNormal, pObject);

	// PhysX Plane바닥 생성
	PxRigidStatic* groundPlane = PxCreatePlane(*m_pPxPhysicsSDK, PxPlane(0, 1, 0, 0), *m_pPxMaterial);
	m_pPxScene->addActor(*groundPlane);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_CubeMesh("Road1", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_CubeMesh("Road2", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);


	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eCrossRoad);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eCrossRoad, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject = new CPhysXObject();
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eCrossRoadD);
		pPhysXObject->CreatePhysX_CubeMesh("CrossRoad", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eCenterRoad);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eCenterRoad, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject = new CPhysXObject();
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eCenterRoadD);
		pPhysXObject->CreatePhysX_CubeMesh("eCenterRoad", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}
#pragma endregion

#pragma region [SideWalk1]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk1);
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk1).size());
	pMesh = new CCubeMeshTexturedTangenteIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);
	//	pMesh = new CCubePatchMesh(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);
	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(XMFLOAT2(2.0f, 2.0f), 2, TextureTag::eSideWalk1D, TextureTag::eSideWalk1ND);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pMesh);
		pPhysXObject->CreatePhysX_CubeMesh("SideWalk1", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [SideWalk2]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk2);
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk2).size());
	pMesh = new CCubeMeshTexturedTangenteIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);

	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(XMFLOAT2(2.0f, 2.0f), 2, TextureTag::eSideWalk2D, TextureTag::eSideWalk2N);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pMesh);
		pPhysXObject->CreatePhysX_CubeMesh("SideWalk2", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pMesh);
		pPhysXObject->CreatePhysX_CubeMesh("StoneWall", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation.x - 90, vecMapData[count].m_Rotation.y - 180, vecMapData[count].m_Rotation.z);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Barricade", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		//pPhysXObject->CreatePhysX_CubeMesh("Barricade", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Fire Barrel 1", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_CubeMesh("Occupy Barrel", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 1 Red", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 1 Blue", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 1 Yellow", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 2 Red", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 2 Blue", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 2 Yellow", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 3 Red", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 3 Blue", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 3 Yellow", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_CubeMesh("Street Lamp", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}

	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

#pragma endregion

#pragma region [Grass]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eGrass);
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eGrass).size());
	pMesh = new CCubeMeshTexturedIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);
	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eGrassD);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pMesh = new CCubeMeshTexturedIlluminated(m_pd3dDevice, vecMapData[count].m_Scale.x, vecMapData[count].m_Scale.y, vecMapData[count].m_Scale.z);
		pPhysXObject->SetMesh(pMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eGrassD);

		pPhysXObject->CreatePhysX_CubeMesh("Grass", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [Building]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding19);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding19, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding19D);
		pPhysXObject->CreatePhysX_TriangleMesh("Building19", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding20);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding20, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding20D);
		pPhysXObject->CreatePhysX_TriangleMesh("Building20", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding21);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding21, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding21D);
		pPhysXObject->CreatePhysX_TriangleMesh("Building21", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding22);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding22, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding22D);
		pPhysXObject->CreatePhysX_TriangleMesh("Building22", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding30);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding30, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding30D);
		pPhysXObject->CreatePhysX_TriangleMesh("Building30", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding33);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding33, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding33D);
		pPhysXObject->CreatePhysX_CubeMesh("Building33", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding34);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding34, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding34D);
		pPhysXObject->CreatePhysX_CubeMesh("Building34", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding77);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding77, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding77D);
		pPhysXObject->CreatePhysX_CubeMesh("Building77", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding78);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding78, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding78D);
		pPhysXObject->CreatePhysX_CubeMesh("Building78", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding100);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding100, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding100D);
		pPhysXObject->CreatePhysX_TriangleMesh("Building100", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding103);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding103, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding103D);
		pPhysXObject->CreatePhysX_TriangleMesh("Building103", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eBuilding104);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eBuilding104, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eBuilding103D);
		pPhysXObject->CreatePhysX_TriangleMesh("Building104", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}
#pragma endregion

#pragma region [WoodBox]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eWoodBox);
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eWoodBox).size());
	pMesh = new CCubeMeshTexturedTangenteIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);
	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eWoodBoxD, TextureTag::eWoodBoxN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pMesh);
		pPhysXObject->CreatePhysX_CubeMesh("WoodBox", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [WoodBoard]
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eWoodBoard).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eWoodBoard);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eWoodBoardD, TextureTag::eWoodBoardN);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eWoodBoard);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_CubeMesh("WoodBoard", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [Stairs]
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eStair).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eStair);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eStairD);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eStair);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Stair", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	// Stair2
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eStair2).size());
	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eStair2);
	pFbxMesh->Initialize(m_pd3dDevice);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(1, TextureTag::eStairD);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eStair2);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Stair2", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
	CPhysXObject* pPhysXObject = nullptr;
	CGameObject* pObject = nullptr;
	vector<MapData> vecMapData;

	// Shader
	CInstancedObjectsShader* pInstancingShaders = nullptr;

#pragma region [Plane]
	pObject = new CGameObject();
	pMesh = new CCubeMeshIlluminated(m_pd3dDevice, 280, 1, 300);

	pObject->SetMesh(pMesh);
	pObject->SetPosition(XMFLOAT3(140, -0.5f, 150));
	pObject->CreateBoundingBox(m_pd3dDevice);

	AddShaderObject(ShaderTag::eNormal, pObject);

	// PhysX Plane바닥 생성
	PxRigidStatic* groundPlane = PxCreatePlane(*m_pPxPhysicsSDK, PxPlane(0, 1, 0, 0), *m_pPxMaterial);
	m_pPxScene->addActor(*groundPlane);

#pragma endregion

#pragma region [Road]
	// ==============================   Road   ============================== //
	// Road1
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eRoad1).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eRoad);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eRoad1D, TextureTag::eRoad1N);

	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eRoad1);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_CubeMesh("Road1", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	// Road2
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eRoad2).size());

	pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eRoad);
	pFbxMesh->Initialize(m_pd3dDevice, true);
	pInstancingShaders->SetMesh(pFbxMesh);
	pInstancingShaders->SetMaterial(2, TextureTag::eRoad2D, TextureTag::eRoad2N);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eRoad2);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_CubeMesh("Road2", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);

	
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eCrossRoad);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eCrossRoad, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice);

		pPhysXObject = new CPhysXObject();
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(1, TextureTag::eCrossRoadD);
		pPhysXObject->CreatePhysX_CubeMesh("CrossRoad", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTexture, pPhysXObject);
	}

	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eCenterRoad);
	for (int count = 0; count < vecMapData.size(); ++count) {
		pFbxMesh = new CFbxModelMesh(m_pd3dDevice, MeshTag::eCenterRoad, vecMapData[count].m_Scale);
		pFbxMesh->Initialize(m_pd3dDevice, true);

		pPhysXObject = new CPhysXObject();
		pPhysXObject->CreateBoundingBox(m_pd3dDevice);
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->SetMaterial(2, TextureTag::eCenterRoadD, TextureTag::eCenterRoadN);
		pPhysXObject->CreatePhysX_CubeMesh("CenterRoad", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		AddShaderObject(ShaderTag::eNormalTangentTexture, pPhysXObject);
	}
#pragma endregion
	
#pragma region [SideWalk1]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk1);
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk1).size());
	pMesh = new CCubeMeshTexturedTangenteIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);
	//	pMesh = new CCubePatchMesh(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);
	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(XMFLOAT2(2.0f, 2.0f), 2, TextureTag::eSideWalk1D, TextureTag::eSideWalk1ND);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pMesh);
		pPhysXObject->CreatePhysX_CubeMesh("SideWalk1", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
#pragma endregion

#pragma region [SideWalk2]
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk2);
	pInstancingShaders = new CInstancedObjectsShader(MAPDATA_MGR->GetDataVector(ObjectTag::eSideWalk2).size());
	pMesh = new CCubeMeshTexturedTangenteIlluminated(m_pd3dDevice, vecMapData[0].m_Scale.x, vecMapData[0].m_Scale.y, vecMapData[0].m_Scale.z);

	pInstancingShaders->SetMesh(pMesh);
	pInstancingShaders->SetMaterial(XMFLOAT2(2.0f, 2.0f), 2, TextureTag::eSideWalk2D, TextureTag::eSideWalk2N);
	pInstancingShaders->BuildObjects(m_pd3dDevice);
	pInstancingShaders->CreateShader(m_pd3dDevice);

	for (int count = 0; count < vecMapData.size(); ++count) {
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pMesh);
		pPhysXObject->CreatePhysX_CubeMesh("SideWalk2", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pMesh);
		pPhysXObject->CreatePhysX_CubeMesh("StoneWall", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation.x - 90, vecMapData[count].m_Rotation.y - 180, vecMapData[count].m_Rotation.z);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 1 Red", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 1 Blue", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 1 Yellow", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 2 Red", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 2 Blue", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 2 Yellow", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 3 Red", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 3 Blue", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Container 3 Yellow", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_TriangleMesh("Fire Barrel 1", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
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
		pPhysXObject = new CPhysXObject();
		pPhysXObject->SetMesh(pFbxMesh);
		pPhysXObject->CreatePhysX_CubeMesh("Occupy Barrel", m_pPxPhysicsSDK, m_pPxScene, m_pPxMaterial, m_pPxCooking);
		pPhysXObject->SetPosition(vecMapData[count].m_Position);
		pPhysXObject->SetRotate(vecMapData[count].m_Rotation);

		pInstancingShaders->AddObject(ShaderTag::eInstanceNormalTangentTexture, pPhysXObject);
		COLLISION_MGR->m_vecStaticMeshContainer.push_back(pPhysXObject);
	}
	m_vecInstancedObjectsShaderContainer.push_back(pInstancingShaders);
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
}

void CMainScene::CreateUIImage()
{
	m_pUIManager = new CUIManager();
	m_pUIManager->Initialize(m_pd3dDevice);
	CUIObject* pUIObject;

	m_pAimObject = new CAimObject(15, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	m_pAimObject->Initialize(m_pd3dDevice, POINT{ 801, 450 }, 1);

	// Zoom Scope
	pUIObject = new CUIObject(TextureTag::eZoomScopeD);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT }, 0.0f);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	// Score
	pUIObject = new CUIObject(TextureTag::eScoreUI);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 600, 0 }, POINT{ 1000, 90 }, 0.5f);
	m_pUIManager->AddUIObject(pUIObject);

	// Life
	pUIObject = new CUIObject(TextureTag::eLifeUI);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 10, 710 }, POINT{ 360, 890 }, 0.5f);
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

	// Weapon Kill UI
	pUIObject = new CUIObject(TextureTag::eRifleKill);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eRifleKill2);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eRifleKill3);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eRifleKill4);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eRifleKill5);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eRifleKill6);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eSniperKill);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eSniperKill2);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eSniperKill3);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eSniperKill4);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eSniperKill5);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eSniperKill6);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 0, 0 }, POINT{ 30, 30 }, 0.0f, true);
	pUIObject->SetActive(false);
	m_pUIManager->AddUIObject(pUIObject);

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

	// ===== Damage Direction ===== //
	pUIObject = new CUIObject(TextureTag::eDamageDirection_Top);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 650, 180 }, POINT{ 950, 280 }, 0.1f);
	pUIObject->AddOpacity(-1.0f);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eDamageDirection_Bottom);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 650, 580 }, POINT{ 950, 680 }, 0.1f);
	pUIObject->AddOpacity(-1.0f);
	m_pUIManager->AddUIObject(pUIObject);
	
	pUIObject = new CUIObject(TextureTag::eDamageDirection_Left);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 555, 280 }, POINT{ 655, 580 }, 0.1f);
	pUIObject->AddOpacity(-1.0f);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eDamageDirection_Right);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 945, 280 }, POINT{ 1045, 580 }, 0.1f);
	pUIObject->AddOpacity(-1.0f);
	m_pUIManager->AddUIObject(pUIObject);
	
	pUIObject = new CUIObject(TextureTag::eDamageDirection_LeftTop);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 515, 135 }, POINT{ 815, 435 }, 0.1f);
	pUIObject->AddOpacity(-1.0f);
	m_pUIManager->AddUIObject(pUIObject);

	pUIObject = new CUIObject(TextureTag::eDamageDirection_RightTop);
	pUIObject->Initialize(m_pd3dDevice, POINT{ 785, 135 }, POINT{ 1085, 435 }, 0.1f);
	pUIObject->AddOpacity(-1.0f);
	m_pUIManager->AddUIObject(pUIObject);
}

void CMainScene::CreateLights()
{
	vector<MapData> vecMapData;
	XMFLOAT3 pos;
	vecMapData = MAPDATA_MGR->GetDataVector(ObjectTag::eStreetLamp);

	// Street Lamp
	for (auto light : vecMapData) {
		pos = light.m_Position;
		pos.y += 10.0f;
		LIGHT_MGR->AddSpotLight(pos, XMFLOAT3(0, -1, 0), 45.0f, 50.0f, 30.0f, XMFLOAT3(1, 1, 1), true);
	}
}

void CMainScene::ReleaseObjects()
{
	CScene::ReleaseObjects();
	ReleaseConstantBuffers();

	SafeDelete(m_pBoundingBoxShader);

	for (auto& object : m_vecCharacterContainer)
		SafeDelete(object);

	m_vecCharacterContainer.clear();
	m_vecReflectObjectContainer.clear();
	
	SafeDelete(m_pAimObject);
	ReleasePhysX();

	DECAL_MGR->ReleseInstance();
	SPRITE_MGR->ReleseInstance();
	LIGHT_MGR->ReleseInstance();
	COLLISION_MGR->ClearContainer();
	PARTICLE_MGR->ReleseManager();
	SOUND_MGR->ClearContainer();
	SOUND_MGR->StopBGMSound();
	SOUND_MGR->StopSound();
	TWBAR_MGR->g_OptionHDR.g_fWhite = TWBAR_MGR->g_cfWhite;
	CCharacterObject::g_nCharacterCount = 0;

	GLOBAL_MGR->g_vRenderOption = XMFLOAT4(0, 0, 0, 1.0f);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Map(GLOBAL_MGR->g_pd3dcbRenderOption, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMFLOAT4 *pcbRenderOption = (XMFLOAT4 *)d3dMappedResource.pData;
	*pcbRenderOption = GLOBAL_MGR->g_vRenderOption;
	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Unmap(GLOBAL_MGR->g_pd3dcbRenderOption, 0);
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
	if (GetTickCount() - m_dwTime > 1000 / m_fFrameSpeed) {
		if (m_nGameTime <= 0) {
			m_bIsGameRoundOver = true;
			return;
		}

		m_nGameTime--;
		CalcOccupyTime();
		m_dwTime = GetTickCount();
	}
}

void CMainScene::CalcOccupyTime()
{
	if (TeamTag::eNone != m_tagOccupyTeam)
		m_OccupyTime++;

	if (m_OccupyTime >= OCCUPY_TIME / 1000) 
	{
		if (m_tagOccupyTeam == TeamTag::eRedTeam)
			m_nRedScore++;
		else if (m_tagOccupyTeam == TeamTag::eBlueTeam)
			m_nBlueScore++;

		if (m_nRedScore == TOTAL_OCCUPYSCORE || m_nBlueScore == TOTAL_OCCUPYSCORE) {
			// 게임 종료됨
			//SCENE_MGR->ChangeScene(SceneTag::eWaitScene);
		}

		m_bIsGameRoundOver = true;
	}
}

void CMainScene::GameRoundOver(float fDeltaTime)
{
	static bool bIsGameRoundOverTimer = false;

	// 게임 라운드 종료시 슬로우되며 하얗게 변한다.
	if (false == bIsGameRoundOverTimer) {
		bIsGameRoundOverTimer = true;
		m_dwGameRoundOverTime = GetTickCount();
		m_fFrameSpeed = 0.3f;
		TWBAR_MGR->g_OptionHDR.g_fWhite = TWBAR_MGR->g_cfWhite;

#ifdef USE_SERVER
		//라운드 종료후 패킷 보낸다.
		cs_round_over packet;

		packet.size = sizeof(cs_round_over);
		packet.type = 12;
		packet.OccupyTeam = static_cast<BYTE>(m_tagOccupyTeam);

		SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&packet));
#endif
	}

	m_fFrameSpeed -= 0.2f * fDeltaTime;
	TWBAR_MGR->g_OptionHDR.g_fWhite -= 1.5f * fDeltaTime;

	if (m_fFrameSpeed < 0.1f)
		m_fFrameSpeed = 0.1f;

	if (TWBAR_MGR->g_OptionHDR.g_fWhite < 0.01f)
		TWBAR_MGR->g_OptionHDR.g_fWhite = 0.01f;


	// 라운드 종료 - 6초 뒤 새 게임 시작
	if (GetTickCount() - m_dwGameRoundOverTime > ROUNDWAIT_TIME) {
		m_OccupyTime = 0;
		m_tagOccupyTeam = TeamTag::eNone;
		m_nGameTime = DEATHMATCH_TIME;

		m_bIsGameRoundOver = false;
		m_fFrameSpeed = 1.0f;
		bIsGameRoundOverTimer = false;
		m_nGameTime = DEATHMATCH_TIME;
		TWBAR_MGR->g_OptionHDR.g_fWhite = TWBAR_MGR->g_cfWhite;

		XMFLOAT3 redTeamStartPosition = XMFLOAT3(65, 2.4f, 12);
		XMFLOAT3 blueTeamStartPosition = XMFLOAT3(270, 2.4f, 230);

		if(m_pPlayerCharacter->GetTagTeam() == TeamTag::eRedTeam)
			m_pPlayer->SetPosition(redTeamStartPosition);
		else
			m_pPlayer->SetPosition(blueTeamStartPosition);

		m_pPlayerCharacter->SetWeaponBulletMax();
		m_pPlayerCharacter->SetLife(PLAYER_HP);
		m_pPlayerCharacter->SetArmorPoint(PLAYER_HP);
	}
}

void CMainScene::RenderUI()
{
	ShowAimUI();
	ShowZoomScope();
	ShowDeadlyAttackUI();
	ShowDeadlyUI();
	ShowDeathRespawnUI();
	ShowDamageDirection();
	// ------ UI ----- //
	m_pUIManager->RenderAll(m_pd3dDeviceContext);
}

void CMainScene::ShowAimUI()
{
	// 1인칭 시점에만 UI Render
	if (m_pCamera->GetCameraTag() != CameraTag::eFirstPerson)
		return;

	// 살아있는 상태에만 UI Render
	if (!m_pPlayerCharacter->GetAlive())
		return;
	
	if (m_pPlayer->GetIsZoom())
		m_pAimObject->SetActive(false);
	else
		m_pAimObject->SetActive(true);

	// 평시 Aim 10
	// 앉기 Aim 5
	// 이동 Aim 15
	// 최대 Aim 30
	m_pAimObject->SetAimSize(10 * m_pPlayerCharacter->GetWeaponCalcRecoil());
	m_pAimObject->Render(m_pd3dDeviceContext);
}

/*
void CMainScene::ShowHitUI()
{
}
*/

void CMainScene::ShowZoomScope()
{
	CUIObject* pScope = m_pUIManager->GetUIObject(TextureTag::eZoomScopeD);

	if (m_pPlayer->GetIsZoom())
		pScope->SetActive(true);
	else
		pScope->SetActive(false);
}

void CMainScene::ShowDamageDirection()
{
	CUIObject* pDamageDirection_TopUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_Top);
	CUIObject* pDamageDirection_LeftUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_Left);
	CUIObject* pDamageDirection_LeftTopUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_LeftTop);
	CUIObject* pDamageDirection_RightUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_Right);
	CUIObject* pDamageDirection_RightTopUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_RightTop);
	CUIObject* pDamageDirection_BottomUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_Bottom);

	float fAddOpacity = -0.3f;
	pDamageDirection_TopUI->AddOpacity(fAddOpacity * m_fDeltaTime);
	pDamageDirection_LeftUI->AddOpacity(fAddOpacity * m_fDeltaTime);
	pDamageDirection_LeftTopUI->AddOpacity(fAddOpacity* m_fDeltaTime);
	pDamageDirection_RightUI->AddOpacity(fAddOpacity* m_fDeltaTime);
	pDamageDirection_RightTopUI->AddOpacity(fAddOpacity * m_fDeltaTime);
	pDamageDirection_BottomUI->AddOpacity(fAddOpacity * m_fDeltaTime);

	CalcDamagedDirection();
}

void CMainScene::CalcDamagedDirection()
{
	if (!m_pPlayer->GetIsDamage())
		return;

	XMFLOAT3 damagedPosition = m_pPlayer->GetDamageInfo().m_f3DamagedPosition;
	XMVECTOR damagedDirection = XMLoadFloat3(&damagedPosition) - m_pPlayer->GetvPosition();
	damagedDirection = XMVector3Normalize(damagedDirection);

	float fDot = XMVectorGetX(XMVector3Dot(m_pPlayer->GetvLook(), damagedDirection));
	float fDegree = XMConvertToDegrees(acosf(fDot));
	XMVECTOR vCross = XMVector3Cross(m_pPlayer->GetvLook(), damagedDirection);

	CUIObject* pDamageDirection_TopUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_Top);
	CUIObject* pDamageDirection_LeftUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_Left);
	CUIObject* pDamageDirection_LeftTopUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_LeftTop);
	CUIObject* pDamageDirection_RightUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_Right);
	CUIObject* pDamageDirection_RightTopUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_RightTop);
	CUIObject* pDamageDirection_BottomUI = m_pUIManager->GetUIObject(TextureTag::eDamageDirection_Bottom);

	// 정면 각도 +- 15도
	if (0 <= fDegree && fDegree < 15) {
		pDamageDirection_TopUI->AddOpacity(1.0f);
	}
	else if (15 <= fDegree && fDegree < 80) {
		if (XMVectorGetY(vCross) > 0)
			pDamageDirection_RightTopUI->AddOpacity(1.0f);
		else
			pDamageDirection_LeftTopUI->AddOpacity(1.0f);
	}
	else if (80 <= fDegree && fDegree < 100) {
		if (XMVectorGetY(vCross) > 0)
			pDamageDirection_RightUI->AddOpacity(1.0f);
		else
			pDamageDirection_LeftUI->AddOpacity(1.0f);
	}
	else {
		pDamageDirection_BottomUI->AddOpacity(1.0f);
	}
	m_pPlayer->SetIsDamage(false);
}

void CMainScene::ShowOccupyGageUI()
{
	CUIObject* pGageUI = m_pUIManager->GetUIObject(TextureTag::eOccupyGageBar);
	CUIObject* pWhiteGageUI = m_pUIManager->GetUIObject(TextureTag::eOccupyGageWhiteBar);

	if (!m_pPlayerCharacter->GetIsOccupy()) {
			pWhiteGageUI->SetActive(false);
			pGageUI->SetActive(false);
			return;
	}

	if (m_pPlayerCharacter->GetTagTeam() == m_tagOccupyTeam)
		return;

	pGageUI->SetActive(true);
	pWhiteGageUI->SetActive(true);

	const UINT gageLength = 600;	// UI x축 길이 600 
	float percentage = (float)(GetTickCount() - m_pPlayerCharacter->GetOccupyTime()) / OCCUPY_TRYTIME;

	pWhiteGageUI->SetEndPos(POINT{ FRAME_BUFFER_WIDTH / 2 - 300 + (LONG)(percentage * gageLength), FRAME_BUFFER_HEIGHT / 2 + 62 });

	if (percentage >= 1) {
#ifdef	USE_SERVER
		sc_occupy packet;
		packet.size = sizeof(sc_occupy);
		packet.type = CS_OCCUPYTEAM;
		packet.OccupyTeam = static_cast<int>(m_pPlayerCharacter->GetTagTeam());
		SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&packet));
#endif
		m_OccupyTime = 0;
		m_tagOccupyTeam = m_pPlayerCharacter->GetTagTeam();
		pGageUI->SetActive(false);
		pWhiteGageUI->SetActive(false);
	}
}

void CMainScene::ShowOccupyPointUI()
{
	// 위치 계산
	XMVECTOR playerPos = m_pPlayer->GetvPosition();
	XMVECTOR occupyPos = XMLoadFloat3(&m_cf3OccupyPosition);
	float distance = XMVectorGetX(XMVector3Length(playerPos - occupyPos));

	if (6.0f <= distance)
		m_pPlayerCharacter->SetOccupy(false);
	else {
		if (m_pPlayerCharacter->GetTagTeam() != m_tagOccupyTeam)
			if(false == m_pPlayerCharacter->GetIsOccupy())
				TEXT_MGR->RenderText(m_pd3dDeviceContext, "Press E Button", 50, 810, 520, 0xFFFFFFFF, FW1_CENTER);
	}
	// UI
	D3D11_VIEWPORT viewport = m_pCamera->GetViewport();
	XMVECTOR vOut = XMVector3Project(XMLoadFloat3(&m_cf3OccupyPosition), viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth, m_pCamera->GetProjectionMatrix(), m_pCamera->GetViewMatrix(), XMMatrixIdentity());
	XMFLOAT3 vScreenPos; XMStoreFloat3(&vScreenPos, vOut);

	vScreenPos.x = clamp((int)vScreenPos.x, 50, FRAME_BUFFER_WIDTH - 50);
	vScreenPos.y = clamp((int)vScreenPos.y, 10, FRAME_BUFFER_HEIGHT - 70);

	// 50m부터 최소 크기
	float fontSize = (1 - distance / 50.0f) * 100;
	fontSize = clamp(fontSize, 0.0f, 25.0f);


	XMVECTOR Direction = XMLoadFloat3(&m_cf3OccupyPosition) - m_pPlayer->GetvPosition();
	Direction = XMVector3Normalize(Direction);

	float fDot = XMVectorGetX(XMVector3Dot(m_pPlayer->GetvLook(), Direction));
	float fDegree = XMConvertToDegrees(acosf(fDot));

	string str = to_string((int)distance) + "m";
	if (0 <= fDegree && fDegree < 80)
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 25 + fontSize, vScreenPos.x, vScreenPos.y, 0xFFFFFFFF, FW1_CENTER);
	else if (80 <= fDegree && fDegree < 89)
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 25 + fontSize, vScreenPos.x, FRAME_BUFFER_HEIGHT - vScreenPos.y, 0xFFFFFFFF, FW1_CENTER);
	else 
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 25 + fontSize, FRAME_BUFFER_WIDTH - vScreenPos.x, FRAME_BUFFER_HEIGHT - 70, 0xFFFFFFFF, FW1_CENTER);
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

	CUIObject* pDamageUI = m_pUIManager->GetUIObject(TextureTag::eDamagedCharacterUI);

	pDamageUI->AddOpacity(-1.0f);
	pGageUI->SetActive(true);
	pWhiteGageUI->SetActive(true);

	const UINT gageLength = 600;	// UI x축 길이 600 
	float percentage = (float)(GetTickCount() - m_pPlayerCharacter->GetDeathTime()) / RESPAWN_TIME;

	pWhiteGageUI->SetEndPos(POINT{ FRAME_BUFFER_WIDTH / 2 - 300 + (LONG)(percentage * gageLength), FRAME_BUFFER_HEIGHT / 2 + 62 });
}

void CMainScene::ShowDeadlyUI()
{
	if (m_pPlayer->GetPlayerLife() <= 0 || PLAYER_HP <= m_pPlayer->GetPlayerLife()) {
		CUIObject* pDamageUI = m_pUIManager->GetUIObject(TextureTag::eDamagedCharacterUI);
		pDamageUI->AddOpacity(-1.0f);
		return;
	}

	if (false == m_pPlayer->GetIsDeadly())
		return;

	CUIObject* pDamageUI = m_pUIManager->GetUIObject(TextureTag::eDamagedCharacterUI);
	static bool bIsReverse = false;
	float opacityValue = 0.8f;

	if (bIsReverse) {
		pDamageUI->AddOpacity(-1 * opacityValue * m_fDeltaTime);

		if (pDamageUI->GetOpacity() <= 0.0f)
			bIsReverse = false;
	}
	else {
		pDamageUI->AddOpacity(opacityValue * m_fDeltaTime);

		if (pDamageUI->GetOpacity() >= 1.0f)
			bIsReverse = true;
	}
}

void CMainScene::ShowDeadlyAttackUI()
{
	if (false == m_pPlayer->GetIsDeadlyAttack())
		return;

	CUIObject* pDamageUI = m_pUIManager->GetUIObject(TextureTag::eDamagedCharacterUI);
	static bool bIsDeadlyAttack = true;
	float opacityValue = 0.8f;

	if (bIsDeadlyAttack) {
		pDamageUI->AddOpacity(1.0f);
		bIsDeadlyAttack = false;
	}
	
	pDamageUI->AddOpacity(-1 * opacityValue * m_fDeltaTime);
	
	if (pDamageUI->GetOpacity() <= 0.0f) {
		bIsDeadlyAttack = true;
		m_pPlayer->SetIsDeadlyAttack(false);
	}
}

void CMainScene::ShowKillUI(KillLog log, UINT activateLine)
{
	if (nullptr == log.pWeaponUI)
		return;
	
	log.pWeaponUI->SetActive(true);
	log.pWeaponUI->SetStartPos(POINT{ 1305, 20 + 50 * (LONG)activateLine });
	log.pWeaponUI->SetEndPos(POINT{ 1395, 65 + 50 * (LONG)activateLine });
}

void CMainScene::CreateKillLog(CCharacterObject* killerPlayer, string diedID, bool bIsHead)
{
	KillLog log;
	log.dwCreateTime = GetTickCount();

	if (killerPlayer->GetTagTeam() == m_pPlayerCharacter->GetTagTeam())
		log.bIsFriendly = true;
	else
		log.bIsFriendly = false;
	log.killerID = killerPlayer->GetID();
	log.diedID = diedID;
	log.bIsHeadShoot = bIsHead;
	log.weaponTag = killerPlayer->GetWeapon()->GetWeaponTag();

	CUIObject* pWeaponUI = nullptr;
	if (log.weaponTag == WeaponTag::eRifle) {
		if (false == m_pUIManager->GetUIObject(TextureTag::eRifleKill)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eRifleKill);
		else if (false == m_pUIManager->GetUIObject(TextureTag::eRifleKill2)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eRifleKill2);
		else if (false == m_pUIManager->GetUIObject(TextureTag::eRifleKill3)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eRifleKill3);
		else if (false == m_pUIManager->GetUIObject(TextureTag::eRifleKill4)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eRifleKill4);
		else if (false == m_pUIManager->GetUIObject(TextureTag::eRifleKill5)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eRifleKill5);
		else if (false == m_pUIManager->GetUIObject(TextureTag::eRifleKill6)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eRifleKill6);
	}
	else if (log.weaponTag == WeaponTag::eSniperRifle) {
		if (false == m_pUIManager->GetUIObject(TextureTag::eSniperKill)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eSniperKill);
		else if (false == m_pUIManager->GetUIObject(TextureTag::eSniperKill2)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eSniperKill2);
		else if (false == m_pUIManager->GetUIObject(TextureTag::eSniperKill3)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eSniperKill3);
		else if (false == m_pUIManager->GetUIObject(TextureTag::eSniperKill4)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eSniperKill4);
		else if (false == m_pUIManager->GetUIObject(TextureTag::eSniperKill5)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eSniperKill5);
		else if (false == m_pUIManager->GetUIObject(TextureTag::eSniperKill6)->GetActive())
			pWeaponUI = m_pUIManager->GetUIObject(TextureTag::eSniperKill6);
	}
	log.pWeaponUI = pWeaponUI;

	m_listKillLog.push_back(log);
}

void CMainScene::Show_KillLog()
{
	UINT startYPos = 30;
	UINT activateLog = 0;

	for (auto killLog : m_listKillLog) {
		if (GetTickCount() - killLog.dwCreateTime < 5000) {	// 5초간 렌더링
			string str = killLog.killerID;
			
			if(killLog.bIsHeadShoot)
				TEXT_MGR->RenderText(m_pd3dDeviceContext, "[Head Shoot]", 18, 1385, 50 + 50 * activateLog, 0xFF2424FF, FW1_RIGHT);

			if(killLog.bIsFriendly)
				TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 35, 1300, 25 + 50 * activateLog, 0xFFFF8A36, FW1_RIGHT);
			else 
				TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 35, 1300, 25 + 50 * activateLog, 0xFF2478FF, FW1_RIGHT);

			ShowKillUI(killLog, activateLog);

			str = killLog.diedID;
			if (killLog.bIsFriendly)
				TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 35, 1400, 25 + 50 * activateLog, 0xFF2478FF, FW1_LEFT);
			else
				TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 35, 1400, 25 + 50 * activateLog, 0xFFFF8A36, FW1_LEFT);

			activateLog++;
		}
		else {
			if (m_listKillLog.front().pWeaponUI) {
				m_listKillLog.front().pWeaponUI->SetActive(false);
				m_listKillLog.front().pWeaponUI = nullptr;
			}
			m_listKillLog.pop_front();
		}
	}
}

void CMainScene::Update_Light()
{
	LIGHT_MGR->ClearLights();
	XMFLOAT3 pos;

	// Player Light
	{
		pos = m_pPlayer->GetPosition();
		XMFLOAT3 look = m_pPlayerCharacter->GetFireDirection();
		//m_pLightManager->AddSpotLight(pos, look, TWBAR_MGR->g_xmf3SelectObjectRotate.x, TWBAR_MGR->g_xmf3SelectObjectRotate.y, TWBAR_MGR->g_xmf3SelectObjectRotate.z, XMFLOAT3(1, 1, 1));
		//	LIGHT_MGR->AddSpotLight(pos, look, 35.0f, 30.0f, 10.0f, XMFLOAT3(1, 1, 1));

		XMVECTOR vPos = m_pPlayer->GetvPosition();
		vPos += 1.3f * m_pPlayer->GetvLook();

		XMStoreFloat3(&pos, vPos);
		LIGHT_MGR->AddPointLight(pos, 5.0f, XMFLOAT3(1, 1, 1));
	}

	// Capturing
	if (m_tagOccupyTeam == TeamTag::eRedTeam) {
		LIGHT_MGR->AddSpotLight(m_cf3OccupyPosition, XMFLOAT3(0, -1, 0), 100.0f, 55.0f, 50.0f, XMFLOAT3(1, 0, 0));

		LIGHT_MGR->AddSpotLight(XMFLOAT3(100.0f, 25.0f, 140.0f), XMFLOAT3(0, -1, 0), 100.0f, 55.0f, 50.0f, XMFLOAT3(1, 0, 0));
		LIGHT_MGR->AddSpotLight(XMFLOAT3(150.0f, 25.0f, 140.0f), XMFLOAT3(0, -1, 0), 100.0f, 55.0f, 50.0f, XMFLOAT3(1, 0, 0));
		LIGHT_MGR->AddSpotLight(XMFLOAT3(100.0f, 25.0f, 200.0f), XMFLOAT3(0, -1, 0), 100.0f, 55.0f, 50.0f, XMFLOAT3(1, 0, 0));
		LIGHT_MGR->AddSpotLight(XMFLOAT3(150.0f, 25.0f, 200.0f), XMFLOAT3(0, -1, 0), 100.0f, 55.0f, 50.0f, XMFLOAT3(1, 0, 0));
	}
	else if (m_tagOccupyTeam == TeamTag::eBlueTeam) {
		LIGHT_MGR->AddSpotLight(m_cf3OccupyPosition, XMFLOAT3(0, -1, 0), 100.0f, 55.0f, 50.0f, XMFLOAT3(0, 0, 1));

		LIGHT_MGR->AddSpotLight(XMFLOAT3(100.0f, 25.0f, 140.0f), XMFLOAT3(0, -1, 0), 100.0f, 55.0f, 50.0f, XMFLOAT3(0, 0, 1));
		LIGHT_MGR->AddSpotLight(XMFLOAT3(150.0f, 25.0f, 140.0f), XMFLOAT3(0, -1, 0), 100.0f, 55.0f, 50.0f, XMFLOAT3(0, 0, 1));
		LIGHT_MGR->AddSpotLight(XMFLOAT3(100.0f, 25.0f, 200.0f), XMFLOAT3(0, -1, 0), 100.0f, 55.0f, 50.0f, XMFLOAT3(0, 0, 1));
		LIGHT_MGR->AddSpotLight(XMFLOAT3(150.0f, 25.0f, 200.0f), XMFLOAT3(0, -1, 0), 100.0f, 55.0f, 50.0f, XMFLOAT3(0, 0, 1));
	}
}

void CMainScene::Update_LightningStrikes(float fDeltaTime)
{
	static bool isFirstLightning = false;
	static bool isLightning = false;
	static bool isFirstThunderSound = false;
	static bool isThunderSound = false;
	static UINT nSoundCount = 0;
	static DWORD dwFirstLightningTime = 0;
	static DWORD dwLightningPeriod = 20000 / m_fFrameSpeed;
	//static const DWORD dwLightningPeriod = 7000;

	if (GetTickCount() - m_dwLastLightningTime > dwLightningPeriod - 4000 / m_fFrameSpeed) {
		// 최초 번개 시작시 잠깐 밝아짐
		if (false == isFirstLightning) {
			isFirstLightning = true;
			TWBAR_MGR->g_OptionHDR.g_fWhite = 0.3f;
			dwFirstLightningTime = GetTickCount();
		}
		else {
			if (false == isLightning) {
				// 0.5초 뒤 소리
				if (false == isFirstThunderSound) {
					if (GetTickCount() - dwFirstLightningTime > 500 / m_fFrameSpeed) {
						if(nSoundCount % 2 == 0)
							SOUND_MGR->Play2DSound(SoundTag::eThunderStrike2);
						else
							SOUND_MGR->Play2DSound(SoundTag::eThunderStrike3);
						isFirstThunderSound = true;
					}
				}
				// 1초 뒤 번개
				if (GetTickCount() - dwFirstLightningTime > 1000 / m_fFrameSpeed) {
					isLightning = true;
					TWBAR_MGR->g_OptionHDR.g_fWhite = 0.15f;
				}
			}
		}
		// 1.5초 뒤 소리
		if (false == isThunderSound) {
			if (GetTickCount() - dwFirstLightningTime > 1500 / m_fFrameSpeed) {
				if (nSoundCount % 2 == 0)
					SOUND_MGR->Play2DSound(SoundTag::eThunderStrike);
				else
					SOUND_MGR->Play2DSound(SoundTag::eThunderStrike4);
				isThunderSound = true;
			}
		}

		// 번개치고 밝아짐 계산
		if (TWBAR_MGR->g_OptionHDR.g_fWhite < TWBAR_MGR->g_cfWhite) {
			if (isFirstLightning && (isLightning == false))
				TWBAR_MGR->g_OptionHDR.g_fWhite += 1.5f * fDeltaTime;

			TWBAR_MGR->g_OptionHDR.g_fWhite += 0.5f * fDeltaTime;
		}

		// 초기화
		if (GetTickCount() - m_dwLastLightningTime > dwLightningPeriod) {
			m_dwLastLightningTime = GetTickCount();
			isFirstLightning = false;
			isLightning = false;
			isThunderSound = false;
			isFirstThunderSound = false;
			nSoundCount++;

			TWBAR_MGR->g_OptionHDR.g_fWhite = TWBAR_MGR->g_cfWhite;
		}
	}
}

void CMainScene::Update(float fDeltaTime)
{
	float fCalcDeltatime = m_fFrameSpeed * fDeltaTime;
	if (m_pPxScene) {
		m_pPxScene->simulate(1 / 60.f);
		m_pPxScene->fetchResults(true);
	}

	COLLISION_MGR->InitCollisionInfo();	// 현재 플레이어만 적용되고있어서 주석처리함
//	COLLISION_MGR->UpdateManager();

	// ====== Update ===== //
	GLOBAL_MGR->UpdateManager();
	UpdateConstantBuffers();
	Update_Light();

	if (m_bIsGameRoundOver)
		GameRoundOver(fCalcDeltatime);
	else {
		Update_LightningStrikes(fCalcDeltatime);
	}
	// ====== Object ===== //
	CScene::Update(fCalcDeltatime);

	for (auto& object : m_vecCharacterContainer)
		object->Update(fCalcDeltatime);

	SPRITE_MGR->UpdateManager(fCalcDeltatime);
	DECAL_MGR->PreRender(m_pd3dDeviceContext);

	// ====== Particle ===== //
	PARTICLE_MGR->UpdateManager(fCalcDeltatime);

	// ====== Timer ===== //
	CalcTime();

	// ====== Deferred ===== //
	float fAdaptationNorm;
	static bool s_bFirstTime = true;
	if (s_bFirstTime) {
		fAdaptationNorm = 1.0f;
		s_bFirstTime = false;
	}
	else 
		fAdaptationNorm = min(TWBAR_MGR->g_OptionHDR.g_fAdaptation < 0.0001f ? 1.0f : fCalcDeltatime / TWBAR_MGR->g_OptionHDR.g_fAdaptation, 0.9999f);
	
	m_PostFX->SetParameters(TWBAR_MGR->g_OptionHDR.g_fMiddleGrey, TWBAR_MGR->g_OptionHDR.g_fWhite, fAdaptationNorm,
		TWBAR_MGR->g_OptionHDR.g_fBloomThreshold, TWBAR_MGR->g_OptionHDR.g_fBloomScale, TWBAR_MGR->g_OptionHDR.g_fDOFFarStart, TWBAR_MGR->g_OptionHDR.g_fDOFFarRange);

	// ===== Light ===== //
	LIGHT_MGR->SetAmbient(m_f3DirectionalAmbientLowerColor, m_f3DirectionalAmbientUpperColor);
	LIGHT_MGR->SetDirectional(m_f3DirectionalDirection, m_f3DirectionalColor);
	m_pSSReflection->SetParameters(TWBAR_MGR->g_xmf3Quaternion.x, TWBAR_MGR->g_xmf3Quaternion.y, TWBAR_MGR->g_xmf3Quaternion.z, TWBAR_MGR->g_xmf3Quaternion.w);
}

void CMainScene::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	ID3D11DepthStencilState* pPrevDepthState; UINT nPrevStencil;
	pd3dDeviceContext->OMGetDepthStencilState(&pPrevDepthState, &nPrevStencil);

#ifdef USE_DEFERRD_RENDER
	// =============== Deferred Rendering ================== //
	if (GLOBAL_MGR->g_bEnablePostFX) {
		float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		m_pd3dDeviceContext->ClearRenderTargetView(m_HDRRTV, fClearColor);
	}
	m_GBuffer->OnPreRender(pd3dDeviceContext);
#endif
	// ------ Start Scene Rendering ------ // 
	CScene::Render(pd3dDeviceContext, pCamera);

	for (auto& object : m_vecCharacterContainer)
		if(object->IsVisible(pCamera))
			object->Render(m_pd3dDeviceContext, m_pCamera);

	DECAL_MGR->Render(pd3dDeviceContext, GLOBAL_MGR->g_bShowWireFrame);

	// ------ End Scene Rendering ------ //
	m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);

#ifdef USE_DEFERRD_RENDER
	m_GBuffer->OnPostRender(pd3dDeviceContext);
	m_GBuffer->OnPrepareForUnpack(pd3dDeviceContext);

	pd3dDeviceContext->OMSetRenderTargets(1, GLOBAL_MGR->g_bEnablePostFX ? &m_HDRRTV : &SCENE_MGR->g_pd3dRenderTargetView, m_GBuffer->GetDepthReadOnlyDSV());

	// ------ Final Scene Rendering ------ //
	m_GBuffer->DeferredRender(pd3dDeviceContext);
	LIGHT_MGR->DoLighting(pd3dDeviceContext, pCamera);

	// Reflection
	if (TWBAR_MGR->g_bIsReflection) {
		m_pSSReflection->PreRenderReflection(pd3dDeviceContext, m_HDRSRV, m_GBuffer->GetDepthView(), m_GBuffer->GetNormalView(), m_GBuffer->GetDepthReadOnlyDSV());
		RenderReflection(pd3dDeviceContext, pCamera);
		m_pSSReflection->PostRenderReflection(pd3dDeviceContext);

		// Restore the previous depth state
		//pd3dDeviceContext->OMSetDepthStencilState(pPrevDepthState, nPrevStencil);
		pd3dDeviceContext->OMSetDepthStencilState(STATEOBJ_MGR->g_pDefaultDSS, nPrevStencil);

		// Add the reflections on top of the scene
		pd3dDeviceContext->OMSetRenderTargets(1, GLOBAL_MGR->g_bEnablePostFX ? &m_HDRRTV : &SCENE_MGR->g_pd3dRenderTargetView, m_GBuffer->GetDepthReadOnlyDSV());

		m_pSSReflection->DoReflectionBlend(pd3dDeviceContext);
	}

	// ----- Particle System ----- //
	PARTICLE_MGR->RenderAllEffect(m_pd3dDeviceContext);
	
	if (GLOBAL_MGR->g_bShowLightVolume)
		LIGHT_MGR->DrawLightVolume(pd3dDeviceContext);

	if (GLOBAL_MGR->g_bEnablePostFX) {
		m_PostFX->PostProcessing(pd3dDeviceContext, m_HDRSRV, m_GBuffer->GetDepthView(), SCENE_MGR->g_pd3dRenderTargetView);
		pd3dDeviceContext->OMSetRenderTargets(1, &SCENE_MGR->g_pd3dRenderTargetView, m_GBuffer->GetDepthDSV());
	}
#endif

	PARTICLE_MGR->RenderAllNoEffect(m_pd3dDeviceContext);
	SPRITE_MGR->RenderAll(m_pd3dDeviceContext, pCamera);
	
	// =============== Rendering Option =================== //
	for (auto& lineObject : GLOBAL_MGR->g_vecLineContainer)
		lineObject->Render(m_pd3dDeviceContext, m_pCamera);

	if (GLOBAL_MGR->g_bShowWorldAxis)
		m_pWorldCenterAxis->Render(pd3dDeviceContext, pCamera);

	if (GLOBAL_MGR->g_vRenderOption.y)
		RenderBoundingBox();

	// Etc
	RenderUI();
	ShowOccupyPointUI();
	ShowOccupyGageUI();
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

void CMainScene::RenderReflection(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	for (auto& object : m_vecReflectObjectContainer)
		object->Render(pd3dDeviceContext, pCamera);
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

	// Draw Select Object
	if (m_pSelectedObject) {
		XMFLOAT3 pos = m_pSelectedObject->GetPosition();
		str = "Position : (" + to_string(pos.x) + ", " + to_string(pos.y) + to_string(pos.z) + ")";

		//TEXT_MGR->RenderText(pd3dDeviceContext, ppos, 30, 20, 80, 0xFFFFFFFF, FW1_LEFT);
	}

	// ================ Draw UI Text ================ //

	// ----- Magazine ------ //
	if(m_pPlayerCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eRifle)
		str = "M16A1";
	else if (m_pPlayerCharacter->GetWeapon()->GetWeaponTag() == WeaponTag::eSniperRifle)
		str = "TRG - 21";
	TEXT_MGR->RenderText(pd3dDeviceContext, str, 51, 1470, 765, 0xFFCCCCCC, FW1_CENTER);

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
	if (m_tagGameMode == GameMode::eDeathMatch) {
		str = to_string(m_nRedTeamTotalKill);
		TEXT_MGR->RenderText(pd3dDeviceContext, str, 60, 698, 10, 0xFF0020FF, FW1_CENTER);

		str = to_string(m_nBlueTeamTotalKill);
		TEXT_MGR->RenderText(pd3dDeviceContext, str, 60, 913, 10, 0xFFFF4500, FW1_CENTER);

		str = to_string(TOTAL_KILLSCORE);
		TEXT_MGR->RenderText(pd3dDeviceContext, str, 65, 800, 10, 0xFFFFFFFF, FW1_CENTER);
	}
	else if ( m_tagGameMode == GameMode::eOccupy) {   // 점령모드일때
		str = to_string(m_nRedScore);
		TEXT_MGR->RenderText(pd3dDeviceContext, str, 60, 698, 10, 0xFF0020FF, FW1_CENTER);

		str = to_string(m_nBlueScore);
		TEXT_MGR->RenderText(pd3dDeviceContext, str, 60, 913, 10, 0xFFFF4500, FW1_CENTER);

		str = to_string(TOTAL_OCCUPYSCORE);
		TEXT_MGR->RenderText(pd3dDeviceContext, str, 65, 800, 10, 0xFFFFFFFF, FW1_CENTER);
	}


	// ----- Respawn ------ //
	if (m_pPlayerCharacter->GetIsDeath()) {
		str = "RESPAWN";
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 800, 430, 0xFFFFFFFF, FW1_CENTER);
	}

	if (m_pPlayerCharacter->GetIsOccupy()) {
		if (m_pPlayerCharacter->GetTagTeam() != m_tagOccupyTeam) {
			str = "점령중";
			TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 800, 430, 0xFFFFFFFF, FW1_CENTER);
		}
	}
	
	if (m_tagOccupyTeam == TeamTag::eRedTeam) {
		str = "Red팀 점령";
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 1430, 430, 0xFFFFFFFF, FW1_LEFT);

		str = to_string(m_OccupyTime);
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 1500, 500, 0xFFFFFFFF, FW1_LEFT);
	}
	else if (m_tagOccupyTeam == TeamTag::eBlueTeam) {
		str = "Blue팀 점령";
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 1400, 430, 0xFFFFFFFF, FW1_LEFT);

		str = to_string(m_OccupyTime);
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 50, 1500, 500, 0xFFFFFFFF, FW1_LEFT);
	}

	// ----- Player ID ----- //
	D3D11_VIEWPORT viewport = m_pCamera->GetViewport();
	for (auto chracter : m_vecCharacterContainer) {
		if (chracter->GetObjectID() == 0)	// 자기 자신 제외
			continue;

		if (chracter->GetTagTeam() != m_pPlayerCharacter->GetTagTeam())	// 자신의 팀만
			continue;

		float distance = XMVectorGetX(XMVector3Length(m_pPlayer->GetvPosition() - chracter->GetvPosition()));
		// 100m부터 최대 높이
		float idHeightPercent = distance / 100;
		idHeightPercent = clamp(idHeightPercent, 0.0f, 1.0f);

		float fontSize = (1 - distance / 10.0f) * 100;
		fontSize = clamp(fontSize, 0.0f, 30.0f);
	
		XMVECTOR vOut = XMVector3Project(chracter->GetvPosition() + XMVectorSet(0.0f, 1.6f + 1.8f * idHeightPercent, 0.0f, 0.0f), 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f, m_pCamera->GetProjectionMatrix(), m_pCamera->GetViewMatrix(), XMMatrixIdentity());
		XMFLOAT3 vScreenPos; XMStoreFloat3(&vScreenPos, vOut);

		XMVECTOR Direction = chracter->GetvPosition() - m_pPlayer->GetvPosition();
		Direction = XMVector3Normalize(Direction);

		float fDot = XMVectorGetX(XMVector3Dot(m_pPlayer->GetvLook(), Direction));
		float fDegree = XMConvertToDegrees(acosf(fDot));

		str = chracter->GetID();
		if (0 <= fDegree && fDegree < 80)
			TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 15 + fontSize, vScreenPos.x, vScreenPos.y, 0xFF41FF3A, FW1_CENTER);
	}

	// ----- Kill Log ----- //
	Show_KillLog();
}
