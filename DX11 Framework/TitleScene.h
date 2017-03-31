#pragma once
#include "Scene.h"
#include "LoadingScene.h"
//#include "MainScene.h"

class CTitleScene :	public CScene
{
	TextureTag	m_tagCursorSelectUI = TextureTag::eNone;

public:
	CTitleScene();
	virtual ~CTitleScene();

	virtual void Initialize() override;
	virtual void CreateUIImage() override;

	void IsOnCursorUI(POINT mousePos);
	virtual void IsCollisionUI(POINT mousePos) override;

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera) override;
};