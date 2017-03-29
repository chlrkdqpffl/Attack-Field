#pragma once
#include "Scene.h"

class CTitleScene :	public CScene
{
	DWORD		m_dwCycleTime = 0;
	TextureTag	m_tagSelectUI = TextureTag::eNone;

public:
	CTitleScene();
	virtual ~CTitleScene();

	virtual void Initialize() override;
	virtual void CreateUIImage() override;
	virtual void IsCollisionUI(POINT mousePos) override;

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera) override;
};