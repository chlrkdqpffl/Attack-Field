#pragma once
#include "Scene.h"
#include "MainScene.h"

class CLoadingScene : public CScene
{
	DWORD		m_dwLoadingStartTime = 0;
	bool		m_bIsLoadingComplete = false;

public:
	CLoadingScene();
	virtual ~CLoadingScene();

	virtual void Initialize() override;
	virtual void CreateUIImage() override;
	virtual void CreatePlayer() override;

	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera) override;
};