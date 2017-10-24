#include "stdafx.h"
#include "GameFramework.h"
#include "TitleScene.h"
#include "MainScene.h"

CGameFramework::CGameFramework()
{
	m_pDXGISwapChain = NULL;
	m_pd3dRenderTargetView = NULL;

	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dDepthStencilView = NULL;

	m_pCamera = NULL;

	m_nWndClientSize.x  = FRAME_BUFFER_WIDTH;
	m_nWndClientSize.y = FRAME_BUFFER_HEIGHT;

	srand(timeGetTime());
	
	// 마우스 정보
	ShowCursor(true);
	m_ptOldCursorPos.x = m_nWndClientSize.x / 2;
	m_ptOldCursorPos.y = m_nWndClientSize.y / 2;
	SetCursorPos(FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2);	// 정중앙


#ifdef USE_CONSOLE
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);
		freopen("CONIN$", "rt", stdin);
		std::ios::sync_with_stdio();
#endif
#ifdef USE_SERVER
	ShowCursor(true);
	ReleaseCapture();
#endif
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
//	_CrtSetBreakAlloc(909230);
//	_CrtSetBreakAlloc(909363);
	
//	_CrtSetBreakAlloc(205);		// 16
//	_CrtSetBreakAlloc(206);		// 16
//	_CrtSetBreakAlloc(210);		// 64
	
#endif

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;
	
	if (!CreateDirect3DDisplay()) return(false);

	// Initialize Manager
	SERVER_MGR->InitializeManager();
	STATEOBJ_MGR->InitializeManager();
	GLOBAL_MGR->InitializeManager();
	TEXT_MGR->InitializeManager(m_pd3dDevice);
	SCENE_MGR->InitializeManager();
	TWBAR_MGR->InitializeManager();
	COLLISION_MGR->InitializeManager();

	MAPDATA_MGR->InitializeManager();
	RESOURCE_MGR->InitializeManager();
	SOUND_MGR->InitializeManager();

	BuildObjects();
	m_pGameTimer = new CGameTimer();
	SCENE_MGR->g_pGameTimer = m_pGameTimer;
	
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
	MAPDATA_MGR->ReleseInstance();
	COLLISION_MGR->ReleseInstance();
	SOUND_MGR->ReleseInstance();
	SERVER_MGR->ReleseInstance();
	PARTICLE_MGR->ReleseInstance();
	SPRITE_MGR->ReleseInstance();

	SafeDelete(m_pGameTimer);

#if defined(DEBUG) || defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif

}

bool CGameFramework::CreateDirect3DDisplay()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientSize.x = rcClient.right - rcClient.left;
	m_nWndClientSize.y = rcClient.bottom - rcClient.top;

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientSize.x;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientSize.y;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
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
		// 외장 그래픽으로 디바이스 생성시 전체화면이 안됨!
		// 개발할 때에는 전체 화면이 필요 없으므로 실행하도록 하고 시연용으로는 파일을 외장그래픽으로 수동 실행하여야 함
		if (SUCCEEDED(hResult = D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, dwCreateDeviceFlags, pd3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext)))
//		if (SUCCEEDED(hResult = D3D11CreateDevice(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, pd3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext)))
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
	DXUT_SetDebugName(pd3dBackBuffer, "MainRenderTargetTexture2D");
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	DXUT_SetDebugName(m_pd3dRenderTargetView, "MainRenderTargetView");
	SCENE_MGR->g_pd3dRenderTargetView = m_pd3dRenderTargetView;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;
	ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dDepthStencilBufferDesc.Width = m_nWndClientSize.x;
	d3dDepthStencilBufferDesc.Height = m_nWndClientSize.y;
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
	SCENE_MGR->g_pd3dDepthStencilView = m_pd3dDepthStencilView;

	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

	return(true);
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	SCENE_MGR->g_nowScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID) {
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		if (GetFocus())
			SetCapture(hWnd);
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
		case VK_ESCAPE:
			SCENE_MGR->g_bMouseBindFlag = !SCENE_MGR->g_bMouseBindFlag;
			ShowCursor(true);
			ReleaseCapture();
			break;
		case VK_RETURN:
			break;
		case VK_F5:
	//		ScreenCapture(m_pd3dRenderTargetView);
			break;
		case VK_F8:
			m_bIsShowDebugInfo != m_bIsShowDebugInfo;
			cout << "Show DebugInfo On/Off" << endl;
			break;
		case VK_F9:
		{
			BOOL bFullScreenState = FALSE;
			m_pDXGISwapChain->GetFullscreenState(&bFullScreenState, NULL);
			if (!bFullScreenState)
			{
				DXGI_MODE_DESC dxgiTargetParameters;
				dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				dxgiTargetParameters.Width = m_nWndClientSize.x;
				dxgiTargetParameters.Height = m_nWndClientSize.y;
			//	dxgiTargetParameters.RefreshRate.Numerator = 60;
			//	dxgiTargetParameters.RefreshRate.Denominator = 1;
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
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (false == m_bTweakInit) {
		// 최초 한 번 호출 -> 호출하지 않는다면 TwDraw 오류 발생
		TwEventWin(m_hWnd, nMessageID, wParam, lParam);
		m_bTweakInit = true;
	}

	if (SCENE_MGR->g_bMouseBindFlag)
		TwEventWin(m_hWnd, nMessageID, wParam, lParam);
	else 
		TwEventWin(m_hWnd, 0, wParam, lParam);

	switch (nMessageID) {
	case WM_SIZE:
	{
		m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
		SCENE_MGR->g_nowScene->OnChangedWindowsSize(hWnd, nMessageID, wParam, lParam);
		SCENE_MGR->g_lParam = lParam;
		m_nWndClientSize.x = LOWORD(lParam);
		m_nWndClientSize.y = HIWORD(lParam);
	
		m_fResizeRatioX = m_nWndClientSize.x / (float)FRAME_BUFFER_WIDTH;
		m_fResizeRatioY = m_nWndClientSize.y / (float)FRAME_BUFFER_HEIGHT;
		TEXT_MGR->SetResizeRatio(m_fResizeRatioX, m_fResizeRatioY);
		SCENE_MGR->g_nowScene->SetResizeRatio(m_fResizeRatioX, m_fResizeRatioY);

//		m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();		// 왜 최소화 하면 안만들어지는가 ?
		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();			// 해제할 경우 디퍼드 렌더링 시 문제됨
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

//		m_pDXGISwapChain->ResizeBuffers(1, m_nWndClientSize.x, m_nWndClientSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		m_pDXGISwapChain->ResizeBuffers(1, m_nWndClientSize.x, m_nWndClientSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		CreateRenderTargetDepthStencilView();

		if (m_pCamera) m_pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, m_nWndClientSize.x, m_nWndClientSize.y, 0.0f, 1.0f);
		break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
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

void CGameFramework::BuildObjects()
{
	CreateConstantBuffers(); 

#ifdef DEVELOP_MODE
	SceneTag startTag = SceneTag::eMainScene;		// Main Scene 시작
#else
	SceneTag startTag = SceneTag::eTitleScene;		// Title Scene 시작
#endif 

	switch (startTag) {
		case SceneTag::eTitleScene:
			SCENE_MGR->ChangeScene(SceneTag::eTitleScene);
		break;
		case SceneTag::eMainScene:
			RESOURCE_MGR->LoadResourceAll();
			SCENE_MGR->ChangeScene(SceneTag::eMainScene);
		break;
	}
	m_pCamera = SCENE_MGR->g_nowScene->GetPlayer()->GetCamera();
	m_pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
//	m_pCamera->GenerateViewMatrix();

	SCENE_MGR->g_nowScene->SetCamera(m_pCamera);
	SCENE_MGR->g_pCamera = m_pCamera;
}

void CGameFramework::ReleaseObjects()
{
	ReleaseConstantBuffers();

	SCENE_MGR->g_nowScene->ReleaseObjects();
	delete SCENE_MGR->g_nowScene;
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
}

void CGameFramework::ReleaseConstantBuffers()
{
	CGameObject::ReleaseConstantBuffers();
	CCamera::ReleaseShaderVariables();
	CTexture::ReleaseShaderVariables();
}

void CGameFramework::ProcessInput()
{
	static UCHAR pKeysBuffer[256];

	float cxDelta = 0.0f, cyDelta = 0.0f;
	POINT ptCursorPos;

	if (GetCapture() == m_hWnd)	{
		if (SCENE_MGR->g_bMouseBindFlag == false) {
			SetCursor(NULL);
			GetCursorPos(&ptCursorPos);

			cxDelta = (float)(ptCursorPos.x - (FRAME_BUFFER_WIDTH / 2)) / 20;
			cyDelta = (float)(ptCursorPos.y - (FRAME_BUFFER_HEIGHT / 2)) / 20;
			SetCursorPos(FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2);
		}
	}
	
	if (SCENE_MGR->g_nowScene->GetSceneTag() == SceneTag::eMainScene) {
		if ((cxDelta != 0.0f) || (cyDelta != 0.0f))
			SCENE_MGR->g_pPlayer->Rotate(cyDelta, cxDelta);
	}
}

void CGameFramework::UpdateObjects()
{
	m_fDeltaTime = m_pGameTimer->GetTimeElapsed();
	m_nFrameRate = m_pGameTimer->GetFrameRate();

	SCENE_MGR->g_fDeltaTime = m_fDeltaTime;
	SCENE_MGR->g_nowScene->Update(m_fDeltaTime);
	SOUND_MGR->UpdateManager(m_fDeltaTime);
}

//#define _WITH_PLAYER_TOP

void CGameFramework::FrameAdvance()
{
	m_pGameTimer->Tick(TWBAR_MGR->g_xmf3Quaternion.y);
//	m_pGameTimer->Tick();

	ProcessInput();
	UpdateObjects();

	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	if (m_pd3dRenderTargetView) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	if (m_pd3dDepthStencilView) m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pCamera = SCENE_MGR->g_nowScene->GetPlayer()->GetCamera();

	SCENE_MGR->g_nowScene->Render(m_pd3dDeviceContext, m_pCamera);
#ifdef _WITH_PLAYER_TOP
	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
#endif

	if(m_bIsShowDebugInfo)
		RenderDebugText();

	// Draw tweak bars
	if (SCENE_MGR->g_nowScene->GetSceneTag() == SceneTag::eMainScene) {
		if (true == SCENE_MGR->g_bMouseBindFlag) {
			if (0 == TwDraw()) {
				MessageBoxA(m_hWnd, TwGetLastError(), "TwDraw Error!", MB_OK | MB_ICONERROR);
				exit(0);
			}
		}
	}
	m_pDXGISwapChain->Present(0, 0);

	SetTitleName();
	m_pd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);
	m_pd3dDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void CGameFramework::SetTitleName()
{
	wstring wstrTitleName = PROJECT_NAME;

	wstrTitleName += to_wstring(m_pGameTimer->GetFrameRate());
	wstrTitleName += L" FPS, ";

	wstrTitleName += to_wstring(m_pGameTimer->GetRealFrameRate());
	wstrTitleName += L" FPS)";

	::SetWindowText(m_hWnd, wstrTitleName.c_str());
}

void CGameFramework::RenderDebugText()
{
	string str;

	// Draw FrameRate
	/*
	UINT fps = m_nFrameRate;
	str = to_string(fps) + " FPS";

	if (60 <= fps)
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 40, 1500, 20, 0xFFFFFFFF, FW1_LEFT);
	else if (30 <= fps && fps < 60)
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 40, 1500, 20, 0xFF1270FF, FW1_LEFT);
	else if (fps < 30)										   
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 40, 1500, 20, 0xFF0000FF, FW1_LEFT);
		*/
	UINT fps = m_pGameTimer->GetRealFrameRate();
	str = to_string(fps) + " FPS";

	if (300 <= fps)
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 40, 1500, 20, 0xFFFFFFFF, FW1_LEFT);
	else if (100 <= fps && fps < 200)
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 40, 1500, 20, 0xFF1270FF, FW1_LEFT);
	else if (fps < 100)
		TEXT_MGR->RenderText(m_pd3dDeviceContext, str, 40, 1500, 20, 0xFF0000FF, FW1_LEFT);

	if (SCENE_MGR->g_nowScene->GetSceneTag() == SceneTag::eMainScene) {
		// Graphic Crad Info
		TEXT_MGR->RenderText(m_pd3dDeviceContext, m_wsGraphicBrandName, 30, 1330, 830, 0xFF41FF3A, FW1_RIGHT);
		TEXT_MGR->RenderText(m_pd3dDeviceContext, "Video Memory : " + to_string(m_ui64VideoMemory / 1048576) + "MB", 30, 1330, 860, 0xFF0000FF, FW1_RIGHT);
	}
}