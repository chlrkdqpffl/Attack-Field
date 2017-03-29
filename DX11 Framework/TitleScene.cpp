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
	CScene::Initialize();
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
	pStartButtonOn->Initialize(m_pd3dDevice, POINT{ 1000, 450 }, POINT{ 1400, 550 }, 0.1f);
	pStartButtonOn->SetActive(false);
	m_pUIManager->AddUIObject(pStartButtonOn);

	CUIObject* pStartButtonOff = new CUIObject(TextureTag::eStartButtonOff);
	pStartButtonOff->Initialize(m_pd3dDevice, POINT{ 1000, 450 }, POINT{ 1400, 550 }, 0.1f);
	m_pUIManager->AddUIObject(pStartButtonOff);

	// Exit
	CUIObject* pExitButtonOn = new CUIObject(TextureTag::eExitButtonOn);
	pExitButtonOn->Initialize(m_pd3dDevice, POINT{ 1000, 580 }, POINT{ 1400, 680 }, 0.1f);
	pExitButtonOn->SetActive(false);
	m_pUIManager->AddUIObject(pExitButtonOn);

	CUIObject* pExitButtonOff = new CUIObject(TextureTag::eExitButtonOff);
	pExitButtonOff->Initialize(m_pd3dDevice, POINT{ 1000, 580 }, POINT{ 1400, 680 }, 0.1f);
	m_pUIManager->AddUIObject(pExitButtonOff);
}

void CTitleScene::IsCollisionUI(POINT mousePos)
{
	if (m_pUIManager->IsEqualTag(mousePos, m_tagSelectUI))
		return;
	왜 안되지, 동일한 위치에 있으면 안바껴야하는데
	auto findTag = m_pUIManager->FindCollisionUIObject(mousePos);

	switch (findTag) {
	case TextureTag::eStartButtonOff:
		m_tagSelectUI = TextureTag::eStartButtonOff;
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOff)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(true);

		break;
	case TextureTag::eStartButtonOn:
		m_tagSelectUI = TextureTag::eStartButtonOn;
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOff)->SetActive(true);

		break;
	case TextureTag::eExitButtonOff:
		m_tagSelectUI = TextureTag::eExitButtonOff;
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOff)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(true);

		break;
	case TextureTag::eExitButtonOn:
		m_tagSelectUI = TextureTag::eExitButtonOn;
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOff)->SetActive(true);

		break;

//	default:
//		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(false);
//		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(false);
//		break;
	}
}

bool CTitleScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	float resizeRatioX = (float)FRAME_BUFFER_WIDTH / GetClientSize().x;
	float resizeRatioY = (float)FRAME_BUFFER_HEIGHT / GetClientSize().y;

	POINT resizeMousePos = { LOWORD(lParam) * resizeRatioX, HIWORD(lParam) * resizeRatioY };

	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		m_pUIManager->IsCollision(resizeMousePos);
		break;
	case WM_MOUSEMOVE:
		
		IsCollisionUI(resizeMousePos);
	
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
	m_pUIManager->RenderAll(pd3dDeviceContext);
}