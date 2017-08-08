#include "stdafx.h"
#include "WaitScene.h"



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
	pDeathMatch->Initialize(m_pd3dDevice, POINT{ 0,0 }, POINT{ FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT }, 0.0f);
	m_pUIManager->AddUIObject(pDeathMatch);

	CUIObject* pOccupyMatch = new CUIObject(TextureTag::eOccupy);
	pOccupyMatch->Initialize(m_pd3dDevice, POINT{ FRAME_BUFFER_WIDTH / 2, 0 }, POINT{ FRAME_BUFFER_WIDTH,  FRAME_BUFFER_HEIGHT }, 0.0f);
	m_pUIManager->AddUIObject(pOccupyMatch);

}


void CWaitScene::CreatePlayer()
{
	/*
	ī�޶� �ʿ������� ĳ���ͱ��� ����� �ִµ� �̴� ������ �ʿ��� �κ�
	����� ������ �÷��̾ ���ļ� ī�޶� ����� �ش�.
	*/


	m_pPlayer = new CTerrainPlayer();
	m_pPlayer->ChangeCamera(m_pd3dDevice, CameraTag::eThirdPerson);
	m_pCamera = m_pPlayer->GetCamera();

	SCENE_MGR->g_pPlayer = m_pPlayer;

}



void CWaitScene::IsOnCursorUI(POINT mousePos)
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
	IsOnCursorUI(mousePos);

	switch (nMessageID) {
	case WM_LBUTTONDOWN:
		IsCollisionUI(mousePos);
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

void CWaitScene::IsCollisionUI(POINT mousePos)
{
	switch (m_tagCursorSelectUI) {
	case TextureTag::eDeath:

		SCENE_MGR->ChangeScene(SceneTag::eLoadingScene);

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

