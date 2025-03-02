#pragma once
#include "Scene.h"
#include "LoadingScene.h"
//#include "MainScene.h"

class CTitleScene :	public CScene
{
	TextureTag	m_tagCursorSelectUI = TextureTag::eNone;
	string m_ID, m_Password, m_scretpassword;
	//char m_ID[10]{ '\0' };
	//char m_Password[10]{ '\0' };
	bool m_Idclick =false;
public:
	CTitleScene();
	virtual ~CTitleScene();

	virtual void Initialize() override;
	virtual void CreateUIImage() override;
	virtual void CreatePlayer() override;

	void IsOnCursorUI(POINT mousePos, HWND hwnd);
	virtual void IsCollisionUI(POINT mousePos , HWND hwnd) override;

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;

	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera) override;
};