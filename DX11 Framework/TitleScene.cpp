#include "stdafx.h"
#include "TitleScene.h"


CTitleScene::CTitleScene()
{
	m_tagScene = SceneTag::eTitleScene;
}

CTitleScene::~CTitleScene()
{
}

void CTitleScene::Initialize()
{
	CScene::CreatePlayer();
	CreateUIImage();
}

void CTitleScene::CreateUIImage()
{
	m_pUIManager = new CUIManager();
	m_pUIManager->Initialize(m_pd3dDevice);

	// BackGround
	CUIObject* pBackGroundUI = new CUIObject(TextureTag::eTitleBackGroundD);
	pBackGroundUI->Initialize(m_pd3dDevice, POINT{ 0,0 }, POINT{ FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT }, 0.99f);
	m_pUIManager->SetBackGroundUI(pBackGroundUI);

	// Start
	CUIObject* pStartButtonOn = new CUIObject(TextureTag::eStartButtonOn);
	pStartButtonOn->Initialize(m_pd3dDevice, POINT{ 1050, 450 }, POINT{ 1400, 550 }, 0.1f);
	pStartButtonOn->SetActive(false);
	m_pUIManager->AddUIObject(pStartButtonOn);

	CUIObject* pStartButtonOff = new CUIObject(TextureTag::eStartButtonOff);
	pStartButtonOff->Initialize(m_pd3dDevice, POINT{ 1050, 450 }, POINT{ 1400, 550 }, 0.1f);
	m_pUIManager->AddUIObject(pStartButtonOff);

	// Exit
	CUIObject* pExitButtonOn = new CUIObject(TextureTag::eExitButtonOn);
	pExitButtonOn->Initialize(m_pd3dDevice, POINT{ 1050, 580 }, POINT{ 1400, 680 }, 0.1f);
	pExitButtonOn->SetActive(false);
	m_pUIManager->AddUIObject(pExitButtonOn);

	CUIObject* pExitButtonOff = new CUIObject(TextureTag::eExitButtonOff);
	pExitButtonOff->Initialize(m_pd3dDevice, POINT{ 1050, 580 }, POINT{ 1400, 680 }, 0.1f);
	m_pUIManager->AddUIObject(pExitButtonOff);
}

void CTitleScene::IsOnCursorUI(POINT mousePos)
{
	auto findTag = m_pUIManager->FindCollisionUIObject(mousePos);

	switch (findTag) {
	case TextureTag::eNone:
		m_tagCursorSelectUI = TextureTag::eNone;
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(false);

		break;
	case TextureTag::eStartButtonOff:
		m_tagCursorSelectUI = TextureTag::eStartButtonOn;
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(true);

		break;
	case TextureTag::eExitButtonOff:
		m_tagCursorSelectUI = TextureTag::eExitButtonOn;
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(true);

		break;
	}
}

void CTitleScene::IsCollisionUI(POINT mousePos)
{
	switch (m_tagCursorSelectUI) {
	case TextureTag::eStartButtonOn:
	//	SCENE_MGR->ChangeScene(new CMainScene());
		SCENE_MGR->ChangeScene(new CLoadingScene());
		break;
	
	case TextureTag::eExitButtonOn:
		::PostQuitMessage(0);
		break;
	}
}

bool CTitleScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	float resizeRatioX = (float)FRAME_BUFFER_WIDTH / GetClientSize().x;
	float resizeRatioY = (float)FRAME_BUFFER_HEIGHT / GetClientSize().y;

	m_resizeMousePos = POINT{ (LONG)(LOWORD(lParam) * resizeRatioX), (LONG)(HIWORD(lParam) * resizeRatioY) };

	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		IsCollisionUI(m_resizeMousePos);
		break;
	case WM_MOUSEMOVE:
	
		break;
	case WM_RBUTTONUP:
		break;
	default:
		break;
	}
	return(false);
}

void CTitleScene::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	IsOnCursorUI(m_resizeMousePos);

	m_pUIManager->RenderAll(pd3dDeviceContext);
}