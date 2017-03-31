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
	CScene::CreatePlayer();
	CreateUIImage();
}

void CLoadingScene::CreateUIImage()
{
	m_pUIManager = new CUIManager();
	m_pUIManager->Initialize(m_pd3dDevice);

	// BackGround
	CUIObject* pBackGroundUI = new CUIObject(TextureTag::eLoadingBackGroundD);
	pBackGroundUI->Initialize(m_pd3dDevice, POINT{ 0,0 }, POINT{ FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT }, 0.99f);
	m_pUIManager->SetBackGroundUI(pBackGroundUI);
}

void CLoadingScene::Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	m_pUIManager->RenderAll(pd3dDeviceContext);

//	if (GLOBAL_MGR->SetTimer(m_dwLoadingStartTime, 5000)) {
//		cout << "¤¾¤·" << endl;
//		SCENE_MGR->ChangeScene(new CMainScene());
//	}
}