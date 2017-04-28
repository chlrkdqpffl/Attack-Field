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

	// Update를 상속받아서 1초 뒤에 로딩 시작하도록 만들어보기. -> Space Conquerer에서 사용한 방법
	if (RESOURCE_MGR->LoadResourceAll()) {
		if(SCENE_MGR->LoadSceneData())
			SCENE_MGR->ChangeScene(SceneTag::eMainScene);
	}
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

void CLoadingScene::Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	m_pUIManager->RenderAll(pd3dDeviceContext);
}