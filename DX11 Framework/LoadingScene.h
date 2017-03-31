#pragma once
#include "Scene.h"
#include "MainScene.h"

class CLoadingScene : public CScene
{
	DWORD		m_dwLoadingStartTime = 0;

public:
	CLoadingScene();
	virtual ~CLoadingScene();

	virtual void Initialize() override;
	virtual void CreateUIImage() override;

	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera) override;
};