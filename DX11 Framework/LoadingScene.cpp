#include "stdafx.h"
#include "LoadingScene.h"

CLoadingScene::CLoadingScene()
{
	m_tagScene = SceneTag::eLoadingScene;
}

CLoadingScene::~CLoadingScene()
{
}

void CLoadingScene::Initialize()
{
	CreatePlayer();
	CreateUIImage();

	m_dwLoadingStartTime = GetTickCount();
}

void CLoadingScene::CreatePlayer()
{
	m_pPlayer = new CTerrainPlayer();
	m_pPlayer->ChangeCamera(m_pd3dDevice, CameraTag::eThirdPerson);
	m_pCamera = m_pPlayer->GetCamera();

	SCENE_MGR->g_pPlayer = m_pPlayer;
}

void CLoadingScene::CreateUIImage()
{
	m_pUIManager = new CUIManager();
	m_pUIManager->Initialize(m_pd3dDevice);

	// BackGround
	CUIObject* pBackGroundUI = new CUIObject(TextureTag::eLoadingBackGroundD);
	pBackGroundUI->Initialize(m_pd3dDevice, POINT{ 0,0 }, POINT{ FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT }, 0.0f);
	m_pUIManager->SetBackGroundUI(pBackGroundUI);
}

void CLoadingScene::Update(float fDeltaTime)
{
	// 0.5초 후 로딩
	if (GetTickCount() - m_dwLoadingStartTime > 500)
		m_bIsLoadingComplete = true;		
}

void CLoadingScene::Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera, HWND hwnd)
{
	m_pUIManager->RenderAll(pd3dDeviceContext);

	if (m_bIsLoadingComplete) {
		RESOURCE_MGR->LoadResourceAll();
		SCENE_MGR->ChangeScene(SceneTag::eMainScene);
		SCENE_MGR->g_pMainScene->OnChangedWindowsSize(0, 0, 0, SCENE_MGR->g_lParam);
#ifdef USE_SERVER
		//SERVER_MGR->Server_init();
#endif
	}
}