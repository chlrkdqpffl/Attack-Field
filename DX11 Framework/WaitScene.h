#pragma once
#include "Scene.h"
class CWaitScene : public CScene
{
	TextureTag	m_tagCursorSelectUI = TextureTag::eNone;
	bool m_mouseclick = false;

public:
	CWaitScene();
	virtual ~CWaitScene();

	virtual void Initialize() override;
	virtual void CreateUIImage() override;
	virtual void CreatePlayer() override;

	//virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera) override;


	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	void IsOnCursorUI(POINT mousePos , HWND hwnd);
	virtual void IsCollisionUI(POINT mousePos , HWND hwnd) override;

};

