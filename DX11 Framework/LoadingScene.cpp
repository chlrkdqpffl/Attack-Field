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
	CreateUIImage();
}

void CLoadingScene::CreateUIImage()
{

}

void CLoadingScene::IsCollisionUI(POINT mousePos)
{
	auto findTag = m_pUIManager->FindCollisionUIObject(mousePos);

	switch (findTag) {
	case TextureTag::eTitleBackGroundD:
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(false);
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(false);

		break;
	case TextureTag::eStartButtonOff:
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(true);
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOff)->SetActive(false);

		break;
	case TextureTag::eStartButtonOn:
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOff)->SetActive(true);
		m_pUIManager->GetUIObject(TextureTag::eStartButtonOn)->SetActive(false);

		break;
	case TextureTag::eExitButtonOff:
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(true);
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOff)->SetActive(false);

		break;
	case TextureTag::eExitButtonOn:
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOff)->SetActive(true);
		m_pUIManager->GetUIObject(TextureTag::eExitButtonOn)->SetActive(false);

		break;
	}
}

bool CLoadingScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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

void CLoadingScene::Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	m_pUIManager->RenderAll(pd3dDeviceContext);
}