#pragma once

#include "Timer.h"
//#include "TitleScene.h"
//#include "MainScene.h"
#include "ScreenShader.h"

class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	bool CreateRenderTargetDepthStencilView();
	bool CreateDirect3DDisplay();

	void CreateConstantBuffers();
	void ReleaseConstantBuffers();
	
	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput();
	void UpdateObjects();
	void FrameAdvance();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void ScreenCapture(ID3D11Resource* resource);
	void ScreenCapture(ID3D11View* shaderResourceView);
	void RenderDebugText();
	void SetTitleName();

private:
	HINSTANCE						m_hInstance;
	HWND							m_hWnd;
	HMENU							m_hMenu;

	POINT							m_nWndClientSize;

	IDXGISwapChain					*m_pDXGISwapChain;
	ID3D11RenderTargetView			*m_pd3dRenderTargetView;
	ID3D11Device					*m_pd3dDevice			= nullptr;
	ID3D11DeviceContext				*m_pd3dDeviceContext	= nullptr;

	UINT							m_n4xMSAAQualities		= 0;

	ID3D11Texture2D					*m_pd3dDepthStencilBuffer = nullptr;
	ID3D11DepthStencilView			*m_pd3dDepthStencilView = nullptr;

	CGameTimer						*m_pGameTimer			= nullptr;
	CCamera							*m_pCamera				= nullptr;

	POINT							m_ptOldCursorPos;
	wstring							m_strTitleName;


	// Graphic Card Info
	wstring							m_wsGraphicBrandName;
	unsigned __int64				m_ui64VideoMemory		= 0;
	bool							m_bIsShowDebugInfo		= false;

	// TweakBar
	bool							m_bTweakInit			= false;

	// FrameRate
	UINT							m_nFrameRate = 0;
	float							m_fDeltaTime = 0.0f;

	// ResizeRatio
	float							m_fResizeRatioX = 1.0f;
	float							m_fResizeRatioY = 1.0f;
};
