#include "stdafx.h"
#include "WaitScene.h"
#include "protocol.h"


CWaitScene::CWaitScene()
{
	m_tagScene = SceneTag::eWaitScene;
}


CWaitScene::~CWaitScene()
{
}

void CWaitScene::Initialize()
{
	CreatePlayer();
	CreateUIImage();
}

void CWaitScene::CreateUIImage()
{
	m_pUIManager = new CUIManager();
	m_pUIManager->Initialize(m_pd3dDevice);

	CUIObject* TitleBackGroundD = new CUIObject(TextureTag::eTitleBackGroundD);
	TitleBackGroundD->Initialize(m_pd3dDevice, POINT{ 0,0 }, POINT{ FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT }, 0.0f);
	TitleBackGroundD->SetActive(false);
	m_pUIManager->AddUIObject(TitleBackGroundD);

	// Death
	CUIObject* pDeathMatchOn = new CUIObject(TextureTag::eDeathOn);
	pDeathMatchOn->Initialize(m_pd3dDevice, POINT{ 0,0 }, POINT{ FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT }, 0.0f);
	pDeathMatchOn->SetActive(false);
	m_pUIManager->AddUIObject(pDeathMatchOn);

	CUIObject* pOccupyMatchOn = new CUIObject(TextureTag::eOccupyOn);
	pOccupyMatchOn->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2, 0 }, POINT{ FRAME_BUFFER_WIDTH,  FRAME_BUFFER_HEIGHT }, 0.0f);
	pOccupyMatchOn->SetActive(false);
	m_pUIManager->AddUIObject(pOccupyMatchOn);

	CUIObject* pDeathMatchOff = new CUIObject(TextureTag::eDeathOff);
	pDeathMatchOff->Initialize(m_pd3dDevice, POINT{ 0,0 }, POINT{ FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT }, 0.0f);
	pDeathMatchOff->SetActive(true);
	m_pUIManager->AddUIObject(pDeathMatchOff);

	CUIObject* pOccupyMatchOff = new CUIObject(TextureTag::eOccupyOff);
	pOccupyMatchOff->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2, 0 }, POINT{ FRAME_BUFFER_WIDTH,  FRAME_BUFFER_HEIGHT }, 0.0f);
	pOccupyMatchOff->SetActive(true);
	m_pUIManager->AddUIObject(pOccupyMatchOff);




	CUIObject* search1 = new CUIObject(TextureTag::eSearch1);
	search1->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search1->SetActive(false);
	m_pUIManager->AddUIObject(search1);

	CUIObject* search2 = new CUIObject(TextureTag::eSearch2);
	search2->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 -20 , FRAME_BUFFER_HEIGHT / 2 -20 }, POINT{ FRAME_BUFFER_WIDTH / 2 +20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search2->SetActive(false);
	m_pUIManager->AddUIObject(search2);

	CUIObject* ssearch3 = new CUIObject(TextureTag::eSearch3);
	ssearch3->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	ssearch3->SetActive(false);
	m_pUIManager->AddUIObject(ssearch3);

	CUIObject* search4 = new CUIObject(TextureTag::eSearch4);
	search4->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search4->SetActive(false);
	m_pUIManager->AddUIObject(search4);

	CUIObject* search5 = new CUIObject(TextureTag::eSearch5);
	search5->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search5->SetActive(false);
	m_pUIManager->AddUIObject(search5);

	CUIObject* search6 = new CUIObject(TextureTag::eSearch6);
	search6->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search6->SetActive(false);
	m_pUIManager->AddUIObject(search6);

	CUIObject* search7 = new CUIObject(TextureTag::eSearch7);
	search7->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search7->SetActive(false);
	m_pUIManager->AddUIObject(search7);

	CUIObject* search8 = new CUIObject(TextureTag::eSearch8);
	search8->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search8->SetActive(false);
	m_pUIManager->AddUIObject(search8);

	CUIObject* search9 = new CUIObject(TextureTag::eSearch9);
	search9->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search9->SetActive(false);
	m_pUIManager->AddUIObject(search9);

	CUIObject* search10 = new CUIObject(TextureTag::eSearch10);
	search10->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search10->SetActive(false);
	m_pUIManager->AddUIObject(search10);

	CUIObject* search11 = new CUIObject(TextureTag::eSearch11);
	search11->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search11->SetActive(false);
	m_pUIManager->AddUIObject(search11);

	CUIObject* search12 = new CUIObject(TextureTag::eSearch12);
	search12->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search12->SetActive(false);
	m_pUIManager->AddUIObject(search12);

	CUIObject* search13 = new CUIObject(TextureTag::eSearch13);
	search13->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2 - 20 , FRAME_BUFFER_HEIGHT / 2 - 20 }, POINT{ FRAME_BUFFER_WIDTH / 2 + 20,  FRAME_BUFFER_HEIGHT / 2 + 20 }, 0.0f);
	search13->SetActive(false);
	m_pUIManager->AddUIObject(search13);

}


void CWaitScene::CreatePlayer()
{
	/*
	카메라만 필요하지만 캐릭터까지 만들어 주는데 이는 수정이 필요한 부분
	현재는 무조건 플레이어를 거쳐서 카메라를 만들어 준다.
	*/


	m_pPlayer = new CTerrainPlayer();
	m_pPlayer->ChangeCamera(m_pd3dDevice, CameraTag::eThirdPerson);
	m_pCamera = m_pPlayer->GetCamera();

	SCENE_MGR->g_pPlayer = m_pPlayer;

}



void CWaitScene::IsOnCursorUI(POINT mousePos, HWND hwnd)
{
	auto findTag = m_pUIManager->FindCollisionUIObject(mousePos);

	switch (findTag) {

	case TextureTag::eNone:
		if (!m_mouseclick)
		{
			m_tagCursorSelectUI = TextureTag::eNone;
			m_pUIManager->GetUIObject(TextureTag::eDeathOn)->SetActive(false);
			m_pUIManager->GetUIObject(TextureTag::eOccupyOn)->SetActive(false);
			m_pUIManager->GetUIObject(TextureTag::eDeathOff)->SetActive(true);
			m_pUIManager->GetUIObject(TextureTag::eOccupyOff)->SetActive(true);
		}

		break;

		//case TextureTag::eDeathOn:

		//	break;
		//case TextureTag::eOccupyOn:

		//	break;
	case TextureTag::eDeathOff:
		m_tagCursorSelectUI = TextureTag::eDeathOn;
		m_pUIManager->GetUIObject(TextureTag::eDeathOff)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eOccupyOn)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eOccupyOff)->SetActive(true);
		m_pUIManager->GetUIObject(TextureTag::eDeathOn)->SetActive(true);
		break;
	case TextureTag::eOccupyOff:
		m_tagCursorSelectUI = TextureTag::eOccupyOn;
		m_pUIManager->GetUIObject(TextureTag::eDeathOn)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eOccupyOff)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eDeathOff)->SetActive(true);
		m_pUIManager->GetUIObject(TextureTag::eOccupyOn)->SetActive(true);
		break;


	}
}

void CWaitScene::SearchRender()
{
	switch (m_tagCursorSelectUI)
	{
	case TextureTag::eSearch1:
		m_tagCursorSelectUI = TextureTag::eSearch2;
		m_pUIManager->GetUIObject(TextureTag::eSearch1)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch2)->SetActive(true);
		break;
	case TextureTag::eSearch2:
		m_tagCursorSelectUI = TextureTag::eSearch3;
		m_pUIManager->GetUIObject(TextureTag::eSearch2)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch3)->SetActive(true);
		break;
	case TextureTag::eSearch3:
		m_tagCursorSelectUI = TextureTag::eSearch4;
		m_pUIManager->GetUIObject(TextureTag::eSearch3)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch4)->SetActive(true);
		break;
	case TextureTag::eSearch4:
		m_tagCursorSelectUI = TextureTag::eSearch5;
		m_pUIManager->GetUIObject(TextureTag::eSearch4)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch5)->SetActive(true);
		break;
	case TextureTag::eSearch5:
		m_tagCursorSelectUI = TextureTag::eSearch6;
		m_pUIManager->GetUIObject(TextureTag::eSearch5)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch6)->SetActive(true);
		break;
	case TextureTag::eSearch6:
		m_tagCursorSelectUI = TextureTag::eSearch7;
		m_pUIManager->GetUIObject(TextureTag::eSearch6)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch7)->SetActive(true);
		break;
	case TextureTag::eSearch7:
		m_tagCursorSelectUI = TextureTag::eSearch8;
		m_pUIManager->GetUIObject(TextureTag::eSearch7)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch8)->SetActive(true);
		break;
	case TextureTag::eSearch8:
		m_tagCursorSelectUI = TextureTag::eSearch9;
		m_pUIManager->GetUIObject(TextureTag::eSearch8)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch9)->SetActive(true);
		break;
	case TextureTag::eSearch9:
		m_tagCursorSelectUI = TextureTag::eSearch10;
		m_pUIManager->GetUIObject(TextureTag::eSearch9)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch10)->SetActive(true);
		break;
	case TextureTag::eSearch10:
		m_tagCursorSelectUI = TextureTag::eSearch11;
		m_pUIManager->GetUIObject(TextureTag::eSearch10)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch11)->SetActive(true);
		break;
	case TextureTag::eSearch11:
		m_tagCursorSelectUI = TextureTag::eSearch12;
		m_pUIManager->GetUIObject(TextureTag::eSearch11)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch12)->SetActive(true);
		break;
	case TextureTag::eSearch12:
		m_tagCursorSelectUI = TextureTag::eSearch13;
		m_pUIManager->GetUIObject(TextureTag::eSearch12)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch13)->SetActive(true);
		break;
	case TextureTag::eSearch13:
		m_tagCursorSelectUI = TextureTag::eSearch1;
		m_pUIManager->GetUIObject(TextureTag::eSearch13)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eSearch1)->SetActive(true);
		break;
	}
}

bool CWaitScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	POINT mousePos{ LOWORD(lParam), HIWORD(lParam) };
	IsOnCursorUI(mousePos, hWnd);

	switch (nMessageID) {
	case WM_LBUTTONDOWN:
		IsCollisionUI(mousePos, hWnd);
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

void CWaitScene::IsCollisionUI(POINT mousePos, HWND hwnd)
{
	switch (m_tagCursorSelectUI) {
	case TextureTag::eDeathOn:
	{
#ifdef USE_SERVER

		cs_Gamemode packet;
		packet.size = sizeof(cs_Gamemode);
		packet.mode = 1;
		packet.type = 5;

		if (!m_mouseclick)
		{
			SERVER_MGR->Sendpacket(reinterpret_cast<BYTE*>(&packet));
			m_mouseclick = true;

			m_pUIManager->GetUIObject(TextureTag::eDeathOn)->SetActive(false);
			m_pUIManager->GetUIObject(TextureTag::eDeathOff)->SetActive(false);
			m_pUIManager->GetUIObject(TextureTag::eOccupyOn)->SetActive(false);
			m_pUIManager->GetUIObject(TextureTag::eOccupyOff)->SetActive(false);

			m_tagCursorSelectUI = TextureTag::eSearch1;
			m_pUIManager->GetUIObject(TextureTag::eTitleBackGroundD)->SetActive(true);
			m_pUIManager->GetUIObject(TextureTag::eSearch1)->SetActive(true);
			
		}
#else
		SCENE_MGR->ChangeScene(SceneTag::eLoadingScene);
#endif

	break;
	}


	case TextureTag::eOccupyOn:
	{
		cs_Gamemode packet;
		packet.size = sizeof(cs_Gamemode);
		packet.mode = 2;
		packet.type = 5;

		if (!m_mouseclick)
		{
			SERVER_MGR->Sendpacket(reinterpret_cast<BYTE*>(&packet));
			m_mouseclick = true;

			m_tagCursorSelectUI = TextureTag::eSearch1;
			m_pUIManager->GetUIObject(TextureTag::eSearch1)->SetActive(true);
		}

		break;
	}
	case TextureTag::eOccupyOff:

		break;

	case TextureTag::eDeathOff:

		break;

	case TextureTag::eExitButtonOn:
		::PostQuitMessage(0);
		break;
	}
}


void CWaitScene::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_pUIManager->RenderAll(pd3dDeviceContext);
	SearchRender();
}