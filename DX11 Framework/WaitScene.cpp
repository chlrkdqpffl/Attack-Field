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

	// Death
	CUIObject* pDeathMatch = new CUIObject(TextureTag::eDeath);
	pDeathMatch->Initialize(m_pd3dDevice, POINT{ 0,0 }, POINT{ FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT  }, 0.0f);
	m_pUIManager->AddUIObject(pDeathMatch);

	CUIObject* pOccupyMatch = new CUIObject(TextureTag::eOccupy);
	pOccupyMatch->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2, 0 }, POINT{ FRAME_BUFFER_WIDTH,  FRAME_BUFFER_HEIGHT }, 0.0f);
	m_pUIManager->AddUIObject(pOccupyMatch);

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
	case TextureTag::eDeath:
		m_tagCursorSelectUI = TextureTag::eDeath;

		break;
	case TextureTag::eOccupy:
		m_tagCursorSelectUI = TextureTag::eOccupy;
		break;
	}
}

bool CWaitScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	POINT mousePos{ LOWORD(lParam), HIWORD(lParam) };
	IsOnCursorUI(mousePos, hWnd);

	switch (nMessageID) {
	case WM_LBUTTONDOWN:
		IsCollisionUI(mousePos , hWnd);
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

void CWaitScene::IsCollisionUI(POINT mousePos , HWND hwnd)
{
	switch (m_tagCursorSelectUI) {
	case TextureTag::eDeath:

#ifdef USE_SERVER
		cs_Gamemode packet;
		packet.size = sizeof(cs_Gamemode);
		packet.mode = 1;
		packet.type = 5;

		if (!m_mouseclick)
		{
			SERVER_MGR->Sendpacket(reinterpret_cast<BYTE*>(&packet));
			m_mouseclick = true;
		}
#else
		SCENE_MGR->ChangeScene(SceneTag::eLoadingScene);
#endif
	
		

		break;

	case TextureTag::eOccupy:

		break;


	case TextureTag::eExitButtonOn:
		::PostQuitMessage(0);
		break;
	}
}


void CWaitScene::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_pUIManager->RenderAll(pd3dDeviceContext);
}