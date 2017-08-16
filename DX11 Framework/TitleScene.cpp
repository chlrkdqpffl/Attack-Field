#include "stdafx.h"
#include "TitleScene.h"
#include "protocol.h"


CTitleScene::CTitleScene()
{
	m_tagScene = SceneTag::eTitleScene;
}

CTitleScene::~CTitleScene()
{
}

void CTitleScene::Initialize()
{
	CreatePlayer();
	CreateUIImage();
}

void CTitleScene::CreatePlayer()
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
	pExitButtonOn->SetActive(false);
	m_pUIManager->AddUIObject(pExitButtonOff);


	CUIObject* pLoginBack = new CUIObject(TextureTag::eLoginBack);
	pLoginBack->Initialize(m_pd3dDevice, POINT{ 940, 505 }, POINT{ 1500, 675 }, 0.1f);
	pLoginBack->SetActive(false);
	m_pUIManager->AddUIObject(pLoginBack);


	//IDUI
	CUIObject* pID = new CUIObject(TextureTag::eID);
	pID->Initialize(m_pd3dDevice, POINT{ 950, 515 }, POINT{ 1320, 585 }, 0.1f);
	pID->SetActive(false);
	m_pUIManager->AddUIObject(pID);

	//IDClick
	CUIObject* pIDClick = new CUIObject(TextureTag::eIDClick);
	pIDClick->Initialize(m_pd3dDevice, POINT{ 950, 515 }, POINT{ 1320, 585 }, 0.1f);
	pIDClick->SetActive(false);
	m_pUIManager->AddUIObject(pIDClick);


	//Password UI
	CUIObject* pPassword = new CUIObject(TextureTag::ePassword);
	pPassword->Initialize(m_pd3dDevice, POINT{ 950, 595 }, POINT{ 1320, 665 }, 0.1f);
	pPassword->SetActive(false);
	m_pUIManager->AddUIObject(pPassword);


	//Password Click
	CUIObject* pPasswordClick = new CUIObject(TextureTag::ePasswordClick);
	pPasswordClick->Initialize(m_pd3dDevice, POINT{ 950, 595 }, POINT{ 1320, 665 }, 0.1f);
	pPasswordClick->SetActive(false);
	m_pUIManager->AddUIObject(pPasswordClick);

	//start 버튼 누르면 로그인 UI뜨게 만든다.
	CUIObject* pLogin = new CUIObject(TextureTag::eLogin);
	pLogin->Initialize(m_pd3dDevice, POINT{ 1330, 515 }, POINT{ 1490, 665 }, 0.1f);
	pLogin->SetActive(false);
	m_pUIManager->AddUIObject(pLogin);

	CUIObject* pLoginOn = new CUIObject(TextureTag::eLoginon);
	pLoginOn->Initialize(m_pd3dDevice, POINT{ 1330, 515 }, POINT{ 1490, 665 }, 0.1f);
	pLoginOn->SetActive(false);
	m_pUIManager->AddUIObject(pLoginOn);




}

void CTitleScene::IsOnCursorUI(POINT mousePos, HWND hWnd)
{
	auto findTag = m_pUIManager->FindCollisionUIObject(mousePos);


	//InvalidateRect(hWnd, NULL, TRUE);

	switch (findTag) {
	case TextureTag::eNone:
		m_tagCursorSelectUI = TextureTag::eNone;
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eLoginon)->SetActive(false);


		break;
	case TextureTag::eStartButtonOff:
		m_tagCursorSelectUI = TextureTag::eStartButtonOn;
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(true);

		break;
	case TextureTag::eExitButtonOff:
		m_tagCursorSelectUI = TextureTag::eExitButtonOn;
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(true);
		break;

	case TextureTag::eLogin:

		m_tagCursorSelectUI = TextureTag::eLoginon;
		m_pUIManager->GetUIObject(TextureTag::eLoginon)->SetActive(true);
		break;

	case TextureTag::eID:
		m_tagCursorSelectUI = TextureTag::eID;
		break;

	case TextureTag::ePassword:
		m_tagCursorSelectUI = TextureTag::ePassword;
		break;

	case TextureTag::eIDClick:
		m_tagCursorSelectUI = TextureTag::eIDClick;
		break;

	case TextureTag::ePasswordClick:
		m_tagCursorSelectUI = TextureTag::ePasswordClick;
		break;

	case TextureTag::eLoginBack:
	{
		if (mousePos.x > 950 && mousePos.x < 1320)
		{
			if(mousePos.y > 515 && mousePos.y< 585)
				m_tagCursorSelectUI = TextureTag::eID;

			else if(mousePos.y > 595 && mousePos.y< 665)
				m_tagCursorSelectUI = TextureTag::ePassword;

			m_pUIManager->GetUIObject(TextureTag::eLoginon)->SetActive(false);

		}
		else if (mousePos.x > 1330 && mousePos.x < 1490)
		{
			if (mousePos.y > 515 && mousePos.y < 665)
			{
				m_pUIManager->GetUIObject(TextureTag::eLoginon)->SetActive(true);
				m_tagCursorSelectUI = TextureTag::eLogin;
			}
		}
		break;
	}
	}
}

void CTitleScene::IsCollisionUI(POINT mousePos, HWND hwnd)
{
	switch (m_tagCursorSelectUI) {
	case TextureTag::eStartButtonOn:

		m_pUIManager->GetUIObject(TextureTag::eLoginBack)->SetActive(true);
		m_pUIManager->GetUIObject(TextureTag::eID)->SetActive(true);
		m_pUIManager->GetUIObject(TextureTag::ePassword)->SetActive(true);
		m_pUIManager->GetUIObject(TextureTag::eLogin)->SetActive(true);


		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOff)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOff)->SetActive(false);


		break;
	case TextureTag::eID:
		m_tagCursorSelectUI = TextureTag::eIDClick;
		m_Idclick = true;
		m_pUIManager->GetUIObject(TextureTag::eID)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eIDClick)->SetActive(true);

		break;

	case TextureTag::ePassword:
		m_tagCursorSelectUI = TextureTag::ePasswordClick;
		m_Idclick = false;
		m_pUIManager->GetUIObject(TextureTag::ePassword)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::ePasswordClick)->SetActive(true);
		break;

	case TextureTag::eLogin:

#ifdef USE_SERVER
		SERVER_MGR->Server_init();




		cs_login packet;
		strcpy(packet.id, m_ID.c_str());
		strcpy(packet.password, m_Password.c_str());

		packet.size = sizeof(packet);
		packet.strlen = strlen(packet.id);
		packet.passstrlen = strlen(packet.password);
		packet.type = 6;

//		SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&packet));

#endif

		m_pUIManager->GetUIObject(TextureTag::eStartButtonOff)->SetActive(true);
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOff)->SetActive(true);



		//m_pUIManager->GetUIObject(TextureTag::eID)->SetActive(false);
		//m_pUIManager->GetUIObject(TextureTag::ePassword)->SetActive(false);
		//m_pUIManager->GetUIObject(TextureTag::eLogin)->SetActive(false);

		if (!SCENE_MGR->m_loginfail)
			SCENE_MGR->ChangeScene(SceneTag::eWaitScene);
		break;

	case TextureTag::eIDClick:
		m_Idclick = true;
		m_tagCursorSelectUI = TextureTag::eIDClick;
		break;

	case TextureTag::ePasswordClick:
		m_Idclick = false;
		m_tagCursorSelectUI = TextureTag::ePasswordClick;
		break;
	case TextureTag::eLoginBack:
		if (mousePos.x > 950 && mousePos.x < 1320)
		{
			if (mousePos.y > 515 && mousePos.y < 585)
			{
				m_Idclick = true;
				m_pUIManager->GetUIObject(TextureTag::eID)->SetActive(false);
				m_pUIManager->GetUIObject(TextureTag::eIDClick)->SetActive(true);
			}
			else if (mousePos.y > 595 && mousePos.y < 665)
			{
				m_Idclick = false;
				m_pUIManager->GetUIObject(TextureTag::ePassword)->SetActive(false);
				m_pUIManager->GetUIObject(TextureTag::ePasswordClick)->SetActive(true);
			}
		}

		break;
	case TextureTag::eExitButtonOn:
		::PostQuitMessage(0);
		break;
	}
}

bool CTitleScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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

bool CTitleScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	static int i = 0;
	static int j = 0;
	if (nMessageID == WM_CHAR)
	{
		if (wParam != VK_BACK)
		{
			if (m_Idclick)
			{

				m_ID += wParam;
//				m_ID[i] = wParam;
	
				i++;

				if (m_ID.size() > 9)
					m_ID.pop_back();
					//i = 9;

			}
			else if (!m_Idclick)
			{
				m_Password += wParam;
				m_scretpassword += '*';
//				m_Password[j] = wParam;
				j++;

				if (m_Password.size() > 9)
				{
					m_Password.pop_back();
					m_scretpassword.pop_back();
				}
					//j = 9;
			}


		}
	}
	else if (nMessageID == WM_KEYDOWN)
	{
		if (wParam == VK_BACK)
		{
			if (m_Idclick)
			{
				if( m_ID.size() > 0)
					m_ID.pop_back();
				//i--;
					i = 0;
				//m_ID[i] = '\0';
			}
			else if (!m_Idclick)
			{
				if (m_Password.size() > 0)
				{
					m_Password.pop_back();
					m_scretpassword.pop_back();
				}
				//j--;
				if (j < 0)
					j = 0;
				//m_Password[j] = '\0';
			}
		}

	}


	
	return false;
}

void CTitleScene::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_pUIManager->RenderAll(pd3dDeviceContext);

	

	TEXT_MGR->RenderText(m_pd3dDeviceContext, m_ID, 40, 1020, 530, 0xFFFFFFFF, FW1_LEFT);
	TEXT_MGR->RenderText(m_pd3dDeviceContext, m_scretpassword, 40, 1025, 615, 0xFFFFFFFF, FW1_LEFT);
}