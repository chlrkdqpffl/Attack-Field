#pragma once

#include "Timer.h"
#include "Scene_Main.h"
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
	// ----- Blurring ----- //
	void SceneBlurring(int nBlurCount);
	void SetGaussianWeights(float fSigma);
	void CreateConstantBuffer_Weights();
	void DrawBlurredSceneToScreen(ID3D11DeviceContext* pd3dDeviceContext);
	void CreateComputeShader(ID3D11Device* pd3dDevice);
	// -------------------- //
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
private:
	HINSTANCE						m_hInstance;
	HWND							m_hWnd;
	HMENU							m_hMenu;

	int								m_nWndClientWidth;
	int								m_nWndClientHeight;

	IDXGISwapChain					*m_pDXGISwapChain;
	ID3D11RenderTargetView			*m_pd3dRenderTargetView;
	ID3D11Device					*m_pd3dDevice			= nullptr;
	ID3D11DeviceContext				*m_pd3dDeviceContext	= nullptr;

	// ----- Blurring ----- //
	ID3D11ShaderResourceView		*m_pd3dSRVOffScreen = nullptr;
	ID3D11UnorderedAccessView		*m_pd3dUAVOffScreen = nullptr;
	ID3D11RenderTargetView			*m_pd3dRTVOffScreen = nullptr;

	ID3D11ShaderResourceView		*m_pd3dSRVTexture	= nullptr;
	ID3D11UnorderedAccessView		*m_pd3dUAVTexture	= nullptr;

	float							m_fGaussianWeights[11];
	ID3D11Buffer					*m_pd3dcbWeights	= nullptr;
	CScreenShader					*m_pScreenShader	= nullptr;

	ID3D11ComputeShader				*m_pHorizontalBlurShader	= nullptr;
	ID3D11ComputeShader				*m_pVerticalBlurShader		= nullptr;
	UINT							m_nBlurCount = 0;
	// -------------------- //

	UINT							m_n4xMSAAQualities;

	ID3D11Texture2D					*m_pd3dDepthStencilBuffer;
	ID3D11DepthStencilView			*m_pd3dDepthStencilView;

	CGameTimer						m_GameTimer;

//	CPlayer							*m_pPlayer;
	CCamera							*m_pCamera;

	POINT							m_ptOldCursorPos;
	wstring							m_strTitleName;


	// Graphic Card Info
	wstring							m_wsGraphicBrandName;
	unsigned __int64				m_ui64VideoMemory		= 0;

	// TweakBar
	bool							m_bMouseBindFlag		= false;
	bool							m_bTweakInit			= false;
};
