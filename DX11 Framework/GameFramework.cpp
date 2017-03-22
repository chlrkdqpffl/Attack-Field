//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
{
	m_pDXGISwapChain = NULL;
	m_pd3dRenderTargetView = NULL;

	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dDepthStencilView = NULL;

	m_pCamera = NULL;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	srand(timeGetTime());
	
	// 마우스 정보
	ShowCursor(false);
	m_ptOldCursorPos.x = m_nWndClientWidth / 2;
	m_ptOldCursorPos.y = m_nWndClientHeight / 2;
	SetCursorPos(FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2);	// 정중앙


//#if defined(DEBUG) || defined(_DEBUG)
	#ifdef USE_CONSOLE
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);
	#endif
//#endif
}

CGameFramework::~CGameFramework()
{
#ifdef USE_CONSOLE
	FreeConsole();
#endif
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetBreakAlloc(220);
	//_CrtSetBreakAlloc(307509);
	
//	_CrtSetBreakAlloc(305984);	// 70	 - wstr 에서 오류
//	_CrtSetBreakAlloc(255527);	// 70
//	_CrtSetBreakAlloc(205);		// 16
//	_CrtSetBreakAlloc(206);		// 16
//	_CrtSetBreakAlloc(210);		// 64
#endif

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	if (!CreateDirect3DDisplay()) return(false);

	// Initialize Manager
	STATEOBJ_MGR->InitializeManager();
	GLOBAL_MGR->InitializeManager();
	TEXT_MGR->InitializeManager(m_pd3dDevice, L"Koverwatch");
//	TEXT_MGR->InitializeManager(m_pd3dDevice, L"a반달곰");			// 폰트 여러개 만들 수 있음
	SCENE_MGR->InitializeManager();
	RESOURCE_MGR->InitializeManager();
	TWBAR_MGR->InitializeManager();

	BuildObjects();

	return(true);
}

bool CGameFramework::CreateDirect3DDisplay()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = TRUE;
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH/*0*/;

	UINT dwCreateDeviceFlags = 0;
#ifdef _DEBUG
	// 이 플래그를 활성화 할 경우 오류 발생 -> Windows 10 SDK와 관련 있다 함
//	dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D_DRIVER_TYPE);

	D3D_FEATURE_LEVEL pd3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(pd3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	D3D_DRIVER_TYPE		nd3dDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL	nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	HRESULT				hResult = S_OK;
	IDXGIFactory1		*pdxgiFactory = nullptr;
	IDXGIAdapter		*pAdapter = nullptr;
	IDXGIDevice			*pdxgiDevice = NULL;
	DXGI_ADAPTER_DESC	adapterDesc;
	::ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));

	// Create DXGI Factory
	if (FAILED(hResult = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)&pdxgiFactory))) return(false);

	// Select Default Graphic Drive
	if (FAILED(pdxgiFactory->EnumAdapters(0, (IDXGIAdapter**)&pAdapter)))
		return false;

	pAdapter->GetDesc(&adapterDesc);
	m_ui64VideoMemory = (unsigned __int64)(adapterDesc.DedicatedVideoMemory + adapterDesc.SharedSystemMemory);
	m_wsGraphicBrandName = adapterDesc.Description;
	
#if !defined(DEBUG) && !defined(_DEBUG)
	int gpu_index = 0;
	unsigned __int64 comparison_videoMemory;
	
	// Find the Best Performance Graphic Cards.
	while (pdxgiFactory->EnumAdapters(gpu_index, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
		pAdapter->GetDesc(&adapterDesc);
		comparison_videoMemory = (unsigned __int64)(adapterDesc.DedicatedVideoMemory + adapterDesc.SharedSystemMemory);

		if (comparison_videoMemory > m_ui64VideoMemory)	{
			m_wsGraphicBrandName = adapterDesc.Description;
			m_ui64VideoMemory = comparison_videoMemory;
			break;
		}
		else
			++gpu_index;
	}
#endif
	

	// Create Device
#ifdef _WITH_DEVICE_AND_SWAPCHAIN
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, pd3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pDXGISwapChain, &g_pd3dDevice, &nd3dFeatureLevel, &g_pd3dDeviceContext))) break;
	}
#else
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
#if defined(DEBUG) || defined(_DEBUG)	// 그래픽 디버깅 도구는 외장 그래픽에서 지원이 안됨
		if (SUCCEEDED(hResult = D3D11CreateDevice(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, pd3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) 
#else
		if (SUCCEEDED(hResult = D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, dwCreateDeviceFlags, pd3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext)))
#endif
			break;
		else {
			MessageBox(m_hWnd, L"D3D11 Device 생성 실패", L"", MB_OK);
			exit(0);
		}
	}
	if (!m_pd3dDevice) return(false);

	if (FAILED(hResult = m_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_n4xMSAAQualities))) return(false);
#ifdef _WITH_MSAA4_MULTISAMPLING
	dxgiSwapChainDesc.SampleDesc.Count = 4;
	dxgiSwapChainDesc.SampleDesc.Quality = m_n4xMSAAQualities - 1;
#else
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
#endif
	if (FAILED(hResult = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pdxgiDevice))) return(false);
	if (FAILED(hResult = pdxgiFactory->CreateSwapChain(pdxgiDevice, &dxgiSwapChainDesc, &m_pDXGISwapChain))) return(false);
	if (pdxgiDevice) pdxgiDevice->Release();
	if (pdxgiFactory) pdxgiFactory->Release();
	if (pAdapter) pAdapter->Release();
#endif

	DXUT_SetDebugName(m_pd3dDevice, "D3D11Device");
	DXUT_SetDebugName(m_pd3dDeviceContext, "D3D11DeviceContext");
	DXUT_SetDebugName(m_pDXGISwapChain, "DXGISwapChain");

	STATEOBJ_MGR->g_pd3dDevice = m_pd3dDevice;
	STATEOBJ_MGR->g_pd3dImmediateDeviceContext = m_pd3dDeviceContext;

	if (!CreateRenderTargetDepthStencilView()) return(false);

	return(true);
}

bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;
	
	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	DXUT_SetDebugName(m_pd3dRenderTargetView, "MainRenderTargetView");

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;
	ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dDepthStencilBufferDesc.Width = m_nWndClientWidth;
	d3dDepthStencilBufferDesc.Height = m_nWndClientHeight;
	d3dDepthStencilBufferDesc.MipLevels = 1;
	d3dDepthStencilBufferDesc.ArraySize = 1;
	d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
#ifdef _WITH_MSAA4_MULTISAMPLING
	d3dDepthStencilBufferDesc.SampleDesc.Count = 4;
	d3dDepthStencilBufferDesc.SampleDesc.Quality = m_n4xMSAAQualities - 1;
#else
	d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
	d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
#endif
	d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3dDepthStencilBufferDesc.CPUAccessFlags = 0;
	d3dDepthStencilBufferDesc.MiscFlags = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return(false);

	DXUT_SetDebugName(m_pd3dDepthStencilBuffer, "DepthStencilBuffer");

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;
#ifdef _WITH_MSAA4_MULTISAMPLING
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
#else
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
#endif
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return(false);

	DXUT_SetDebugName(m_pd3dDepthStencilView, "DepthStencilView");

	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);


	// Create Off Screen
	D3D11_TEXTURE2D_DESC d3dTextureBufferDesc;
	ZeroMemory(&d3dTextureBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTextureBufferDesc.Width				= m_nWndClientWidth;
	d3dTextureBufferDesc.Height				= m_nWndClientHeight;
	d3dTextureBufferDesc.MipLevels			= 1;
	d3dTextureBufferDesc.ArraySize			= 1;
	d3dTextureBufferDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dTextureBufferDesc.SampleDesc.Count	= 1;
	d3dTextureBufferDesc.SampleDesc.Quality = 0;
	d3dTextureBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	d3dTextureBufferDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	d3dTextureBufferDesc.CPUAccessFlags		= 0;
	d3dTextureBufferDesc.MiscFlags			= 0;

	ID3D11Texture2D* offScreenTexture = nullptr;
	HR(m_pd3dDevice->CreateTexture2D(&d3dTextureBufferDesc, 0, &offScreenTexture));
	HR(m_pd3dDevice->CreateShaderResourceView(offScreenTexture, 0, &m_pd3dSRVOffScreen));
	HR(m_pd3dDevice->CreateUnorderedAccessView(offScreenTexture, 0, &m_pd3dUAVOffScreen));
	HR(m_pd3dDevice->CreateRenderTargetView(offScreenTexture, 0, &m_pd3dRTVOffScreen));

	offScreenTexture->Release();

	DXUT_SetDebugName(offScreenTexture, "OffScreenTexture2D");
	DXUT_SetDebugName(m_pd3dSRVOffScreen, "SRVOffScreen");
	DXUT_SetDebugName(m_pd3dUAVOffScreen, "UAVOffScreen");
	DXUT_SetDebugName(m_pd3dRTVOffScreen, "RTVOffScreen");


	d3dTextureBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;;
	HR(m_pd3dDevice->CreateTexture2D(&d3dTextureBufferDesc, 0, &offScreenTexture));;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRViewDesc;
	ZeroMemory(&d3dSRViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSRViewDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dSRViewDesc.ViewDimension			= D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSRViewDesc.Texture2D.MipLevels	= 1;
	d3dSRViewDesc.Texture2D.MostDetailedMip = 0;

	HR(m_pd3dDevice->CreateShaderResourceView(offScreenTexture, &d3dSRViewDesc, &m_pd3dSRVTexture));

	D3D11_UNORDERED_ACCESS_VIEW_DESC d3dUAViewDesc;
	ZeroMemory(&d3dUAViewDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	d3dUAViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dUAViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	d3dUAViewDesc.Texture2D.MipSlice = 0;

	HR(m_pd3dDevice->CreateUnorderedAccessView(offScreenTexture, &d3dUAViewDesc, &m_pd3dUAVTexture));
	offScreenTexture->Release();

	DXUT_SetDebugName(m_pd3dSRVTexture, "SRVTexture");
	DXUT_SetDebugName(m_pd3dUAVTexture, "UAVTexture");

	return(true);
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDevice->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();

	// Relesed Manager
	STATEOBJ_MGR->ReleseInstance();
	GLOBAL_MGR->ReleseInstance();
	TEXT_MGR->ReleseInstance();
	SCENE_MGR->ReleseInstance();
	RESOURCE_MGR->ReleseInstance();
	TWBAR_MGR->ReleseInstance();

#if defined(DEBUG) || defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif

}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	SCENE_MGR->g_nowScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID) {
	case WM_LBUTTONDOWN:
		if (false == m_bMouseBindFlag) {
			GetCursorPos(&m_ptOldCursorPos);
			SetCapture(hWnd);
		}
		break;
	case WM_LBUTTONUP:
		if (false == m_bMouseBindFlag) {
			ReleaseCapture();
		}
		break;
	case WM_MOUSEMOVE:

		break;
	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	SCENE_MGR->g_nowScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_CONTROL:
			if (m_bMouseBindFlag == false){
				ShowCursor(true);
				ReleaseCapture();
			}
			m_bMouseBindFlag = true;
			break;
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		case VK_F5:
		{
			ScreenCapture(m_pd3dRenderTargetView);
		}
		break;
		case VK_F9:
		{
			BOOL bFullScreenState = FALSE;
			m_pDXGISwapChain->GetFullscreenState(&bFullScreenState, NULL);
			if (!bFullScreenState)
			{
				DXGI_MODE_DESC dxgiTargetParameters;
				dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				dxgiTargetParameters.Width = m_nWndClientWidth;
				dxgiTargetParameters.Height = m_nWndClientHeight;
				dxgiTargetParameters.RefreshRate.Numerator = 0;
				dxgiTargetParameters.RefreshRate.Denominator = 0;
				dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
				dxgiTargetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				m_pDXGISwapChain->ResizeTarget(&dxgiTargetParameters);
			}
			m_pDXGISwapChain->SetFullscreenState(!bFullScreenState, NULL);
		}
		break;
		case VK_F10:
		
			cout << "F10 안된다.";
		
		break;
		default:
			break;
		}
		break;
	default:
		break;
	case WM_KEYUP:
		switch (wParam)	{
			case VK_CONTROL:
				m_bMouseBindFlag = false;
				ShowCursor(false);
				break;
			}
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (false == m_bTweakInit) {
		// 최초 한 번 호출 -> 호출하지 않는다면 TwDraw 오류 발생
		TwEventWin(m_hWnd, nMessageID, wParam, lParam);
		m_bTweakInit = true;
	}

	if (m_bMouseBindFlag)
		TwEventWin(m_hWnd, nMessageID, wParam, lParam);
	else 
		TwEventWin(m_hWnd, 0, wParam, lParam);

	switch (nMessageID) {
	case WM_SIZE:
	{
		m_nWndClientWidth = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);

//		m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();		// 왜 최소화 하면 안만들어지는가 ?
		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

		m_pDXGISwapChain->ResizeBuffers(1, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		CreateRenderTargetDepthStencilView();

		if (m_pCamera) m_pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);
		break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

void CGameFramework::ScreenCapture(ID3D11Resource* resource)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	char save_path[128];
	sprintf_s(save_path, "ScreenShot/%d.%d.%d_%d-%d-%d.jpg", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	HRESULT hr = -1;
	hr = D3DX11SaveTextureToFileA(m_pd3dDeviceContext, resource, D3DX11_IFF_JPG, save_path);
	if (hr == S_OK)
		cout << "스크린샷 저장 완료" << endl;
	else
		cout << "스크린샷 저장 실패" << endl;
}

void CGameFramework::ScreenCapture(ID3D11View* resourceView)
{
	// 현재 날짜
	SYSTEMTIME st;
	GetLocalTime(&st);

	char save_path[128];
	sprintf_s(save_path, "ScreenShot/%d.%d.%d_%d-%d-%d.jpg", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	HRESULT hr = -1;
	ID3D11Resource* tempResource = nullptr;
	resourceView->GetResource(&tempResource);
	hr = D3DX11SaveTextureToFileA(m_pd3dDeviceContext, tempResource, D3DX11_IFF_JPG, save_path);
	tempResource->Release();
	if (hr == S_OK)
		cout << "스크린샷 저장 완료" << endl;
	else
		cout << "스크린샷 저장 실패" << endl;
}

void CGameFramework::SceneBlurring(int nBlurCount)
{
	if (nBlurCount == 0)
		return;

	UINT cxGroups = (UINT)ceilf(m_nWndClientWidth / 256.0f);
	UINT cyGroups = (UINT)ceilf(m_nWndClientHeight / 256.0f);

	ID3D11ShaderResourceView *pd3dNullResourceViews[1] = { nullptr };
	ID3D11UnorderedAccessView *pd3dNullUnorderedViews[1] = { nullptr };

	SetGaussianWeights(2);

	for (int i = 0; i < nBlurCount; ++i)
	{
		m_pd3dDeviceContext->CSSetShaderResources(CS_TEXTURE_SLOT_BLUR, 1, &m_pd3dSRVOffScreen);
		m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, &m_pd3dUAVTexture, nullptr);
		m_pd3dDeviceContext->CSSetShader(m_pHorizontalBlurShader, nullptr, 0);
		m_pd3dDeviceContext->Dispatch(cxGroups, m_nWndClientHeight, 1);

		m_pd3dDeviceContext->CSSetShaderResources(0, 1, pd3dNullResourceViews);
		m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, pd3dNullUnorderedViews, nullptr);

		m_pd3dDeviceContext->CSSetShaderResources(CS_TEXTURE_SLOT_BLUR, 1, &m_pd3dSRVTexture);
		m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, &m_pd3dUAVOffScreen, nullptr);
		m_pd3dDeviceContext->CSSetShader(m_pVerticalBlurShader, nullptr, 0);
		m_pd3dDeviceContext->Dispatch(m_nWndClientWidth, cyGroups, 1);

		m_pd3dDeviceContext->CSSetShaderResources(0, 1, pd3dNullResourceViews);
		m_pd3dDeviceContext->CSSetUnorderedAccessViews(0, 1, pd3dNullUnorderedViews, nullptr);
		m_pd3dDeviceContext->CSSetShader(nullptr, nullptr, NULL);
	}
}

void CGameFramework::SetGaussianWeights(float fSigma)
{
	float fSum = 0.0f, f2Sigma = 2.0f * fSigma * fSigma;
	for (int i = 0; i < 11; ++i)
	{
		m_fGaussianWeights[i] = expf(-(float)(i*i) / f2Sigma);
		fSum += m_fGaussianWeights[i];
	}
	for (int i = 0; i < 11; ++i) {
		m_fGaussianWeights[i] /= fSum;

	}

	cout << fSum << endl;
	D3D11_MAPPED_SUBRESOURCE d3dMapResource;
	m_pd3dDeviceContext->Map(m_pd3dcbWeights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMapResource);

	float* pcbWeights = (float*)d3dMapResource.pData;
	memcpy(pcbWeights, m_fGaussianWeights, sizeof(float) * 11);

	m_pd3dDeviceContext->Unmap(m_pd3dcbWeights, 0);
	m_pd3dDeviceContext->CSSetConstantBuffers(CS_SLOT_WEIGHTS, 1, &m_pd3dcbWeights);
}

void CGameFramework::CreateConstantBuffer_Weights()
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(float) * 16;		// 16의 배수
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HR(m_pd3dDevice->CreateBuffer(&d3dBufferDesc, nullptr, &m_pd3dcbWeights));

	DXUT_SetDebugName(m_pd3dcbWeights, "Weights");
}

void CGameFramework::CreateComputeShader(ID3D11Device* pd3dDevice)
{
	CShader* pComputeShader = new CShader();
	pComputeShader->CreateComputeShaderFromFile(pd3dDevice, L"Shader HLSL File/Blurring.hlsli", "HorzBlurCS", "cs_5_0", &m_pHorizontalBlurShader);
	pComputeShader->CreateComputeShaderFromFile(pd3dDevice, L"Shader HLSL File/Blurring.hlsli", "VertBlurCS", "cs_5_0", &m_pVerticalBlurShader);

	SafeDelete(pComputeShader);
}

void CGameFramework::DrawBlurredSceneToScreen(ID3D11DeviceContext* pd3dDeviceContext)
{
	m_pScreenShader->SetTexture(m_pd3dSRVTexture);
	m_pScreenShader->Render(m_pd3dDeviceContext);
}

void CGameFramework::BuildObjects()
{
	CreateConstantBuffers();

	CScene* m_pScene = new CScene_Main();
	SCENE_MGR->g_nowScene = m_pScene;
	m_pScene->SetDevice(m_pd3dDevice);
	m_pScene->SetDeviceContext(m_pd3dDeviceContext);
	m_pScene->BuildObjects(m_pd3dDevice);

	m_pCamera = m_pScene->GetPlayer()->GetCamera();
	m_pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	m_pCamera->GenerateViewMatrix();

	m_pScene->SetCamera(m_pCamera);


	SCENE_MGR->g_pCamera = m_pCamera;

	// Screen Shader
	m_pScreenShader = new CScreenShader();
	m_pScreenShader->CreateMesh(m_pd3dDevice);
	m_pScreenShader->CreateShader(m_pd3dDevice);
	
	// Create ComputeShader
	CreateComputeShader(m_pd3dDevice);
}

void CGameFramework::ReleaseObjects()
{
	ReleaseConstantBuffers();

	SCENE_MGR->g_nowScene->ReleaseObjects();
	delete SCENE_MGR->g_nowScene;

	SafeDelete(m_pScreenShader);
}

void CGameFramework::CreateConstantBuffers()
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	HR(m_pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &CGameObject::m_pd3dcbWorldMatrix));

	d3dBufferDesc.ByteWidth = sizeof(XMVECTOR) * 4;
	HR(m_pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &CGameObject::m_pd3dcbMaterialColors));

	DXUT_SetDebugName(CGameObject::m_pd3dcbWorldMatrix, "WorldMatrix");
	DXUT_SetDebugName(CGameObject::m_pd3dcbMaterialColors, "MaterialColors");

	CCamera::CreateShaderVariables(m_pd3dDevice);
	CTexture::CreateShaderVariables(m_pd3dDevice);

	CreateConstantBuffer_Weights();
}

void CGameFramework::ReleaseConstantBuffers()
{
	if(m_pd3dcbWeights) m_pd3dcbWeights->Release();

	CGameObject::ReleaseConstantBuffers();
	CCamera::ReleaseShaderVariables();
	CTexture::ReleaseShaderVariables();
}

void CGameFramework::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;

//	if (GetKeyboardState(pKeysBuffer) && SCENE_MGR->m_nowScene) bProcessedByScene = SCENE_MGR->m_nowScene->ProcessInput(pKeysBuffer);
	if (!bProcessedByScene)
	{
		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;
		
		if (false == m_bMouseBindFlag) {
			if (GetCapture() == m_hWnd) {
				GetCursorPos(&ptCursorPos);
				cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
				cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
				SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
			}
			else {
				//	ScreenToClient(m_hWnd, POINT(FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2));
				SetCursorPos(FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2);
			}
		}
		
		if ((cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				if (pKeysBuffer[VK_RBUTTON] & 0xF0)
					SCENE_MGR->g_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
				else
					SCENE_MGR->g_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
			}
		}
	}
	
}

void CGameFramework::UpdateObjects()
{
	m_fTimeElapsed = m_GameTimer.GetTimeElapsed();
	m_nFrameRate = m_GameTimer.GetFrameRate();

	SCENE_MGR->g_fTimeElapsed = m_fTimeElapsed;
	SCENE_MGR->g_nowScene->SetTimeElapsed(m_fTimeElapsed);
	SCENE_MGR->g_nowScene->UpdateObjects(m_fTimeElapsed);
}

//#define _WITH_PLAYER_TOP

void CGameFramework::FrameAdvance()
{
	m_GameTimer.Tick(60);
//	m_GameTimer.Tick();

	ProcessInput();
	UpdateObjects();

	SCENE_MGR->g_nowScene->OnPreRender(m_pd3dDeviceContext);
	
	/*
	// 블러링
	ID3D11RenderTargetView *pd3dRenderTargetView[1] = { m_pd3dRTVOffScreen };
	m_pd3dDeviceContext->OMSetRenderTargets(1, pd3dRenderTargetView, m_pd3dDepthStencilView);

	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	if (m_pd3dRenderTargetView) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRTVOffScreen, fClearColor);
	if (m_pd3dDepthStencilView) m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	if (m_pPlayer) m_pPlayer->UpdateShaderVariables(m_pd3dDeviceContext);
	m_pCamera->SetViewport(m_pd3dDeviceContext);

	// WireFrame Mode
	if (GetAsyncKeyState('3') & 0x8000)
		m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pWireframeRS);
	else 
		m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);
	

	SCENE_MGR->m_nowScene->Render(m_pd3dDeviceContext, m_pCamera);

#ifdef _WITH_PLAYER_TOP
	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
#endif

	pd3dRenderTargetView[0] = m_pd3dRenderTargetView;
	m_pd3dDeviceContext->OMSetRenderTargets(1, pd3dRenderTargetView, m_pd3dDepthStencilView);

	SceneBlurring(3);

	if (m_pd3dRenderTargetView) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	if (m_pd3dDepthStencilView) m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	DrawBlurredSceneToScreen(m_pd3dDeviceContext);
	*/
	
	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	if (m_pd3dRenderTargetView) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	if (m_pd3dDepthStencilView) m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pCamera = SCENE_MGR->g_nowScene->GetPlayer()->GetCamera();
	m_pCamera->SetViewport(m_pd3dDeviceContext);

	// WireFrame Mode
	if (GetAsyncKeyState('3') & 0x8000)
		m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pWireframeRS);
	else
		m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);

	SCENE_MGR->g_nowScene->Render(m_pd3dDeviceContext, m_pCamera);
#ifdef _WITH_PLAYER_TOP
	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
#endif
	
	SCENE_MGR->g_nowScene->RenderAllText(m_pd3dDeviceContext);		// 텍스트는 마지막에 렌더링
	RenderAllText();

	// Draw tweak bars
	if (true == m_bMouseBindFlag) {
		if (0 == TwDraw()) {
			MessageBoxA(m_hWnd, TwGetLastError(), "TwDraw Error!", MB_OK | MB_ICONERROR);
			exit(0);
		}	
	}
	m_pDXGISwapChain->Present(0, 0);

	m_GameTimer.SetTitleName(m_strTitleName);
	::SetWindowText(m_hWnd, m_strTitleName.c_str());
	m_pd3dDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void CGameFramework::RenderAllText()
{
	string str;

	// Draw FrameRate
	UINT fps = m_nFrameRate;
	str = to_string(fps) + " FPS";

	if (60 <= fps)
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 40, 1500, 20, 0xFFFFFFFF, FW1_LEFT);
	else if (30 <= fps && fps < 60)
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 40, 1500, 20, 0xFF1270FF, FW1_LEFT);
	else if (fps < 30)
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 40, 1500, 20, 0xFF0000FF, FW1_LEFT);

	// Graphic Crad Info
	TEXT_MGR->RenderText(m_pd3dDeviceContext, m_wsGraphicBrandName, 30, 20, 830, 0xFF41FF3A, FW1_LEFT);
	TEXT_MGR->RenderText(m_pd3dDeviceContext, "Video Memory : " + to_string(m_ui64VideoMemory / 1048576) + "MB", 30, 20, 860, 0xFF0000FF, FW1_LEFT);

}