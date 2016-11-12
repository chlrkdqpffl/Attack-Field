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

	m_pPlayer = NULL;
	m_pCamera = NULL;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	_tcscpy_s(m_pszBuffer, _T("DX11_Framework ("));

	srand(timeGetTime());

#if defined(DEBUG) || defined(_DEBUG)
	#ifdef USE_CONSOLE
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);
	#endif
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
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	if (!CreateDirect3DDisplay()) return(false);

	// Manager Init
	STATEOBJ_MGR->InitializeManager();
	TEXT_MGR->InitializeManager(STATEOBJ_MGR->g_pd3dDevice.Get(), L"Koverwatch");
//	TEXT_MGR->InitializeManager(STATEOBJ_MGR->g_pd3dDevice.Get(), L"a반달곰");			// 폰트 여러개 만들 수 있음
	SCENE_MGR->InitializeManager();
	RESOURCE_MGR->InitializeManager();

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
		if (SUCCEEDED(hResult = D3D11CreateDevice(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, pd3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &STATEOBJ_MGR->g_pd3dDevice, &nd3dFeatureLevel, &STATEOBJ_MGR->g_pd3dImmediateDeviceContext))) break;
	}
	if (!STATEOBJ_MGR->g_pd3dDevice.Get()) return(false);

	if (FAILED(hResult = STATEOBJ_MGR->g_pd3dDevice.Get()->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_n4xMSAAQualities))) return(false);
#ifdef _WITH_MSAA4_MULTISAMPLING
	dxgiSwapChainDesc.SampleDesc.Count = 4;
	dxgiSwapChainDesc.SampleDesc.Quality = m_n4xMSAAQualities - 1;
#else
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
#endif
	IDXGIFactory1 *pdxgiFactory = NULL;
	if (FAILED(hResult = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)&pdxgiFactory))) return(false);
	IDXGIDevice *pdxgiDevice = NULL;
	if (FAILED(hResult = STATEOBJ_MGR->g_pd3dDevice.Get()->QueryInterface(__uuidof(IDXGIDevice), (void **)&pdxgiDevice))) return(false);
	if (FAILED(hResult = pdxgiFactory->CreateSwapChain(pdxgiDevice, &dxgiSwapChainDesc, &m_pDXGISwapChain))) return(false);
	if (pdxgiDevice) pdxgiDevice->Release();
	if (pdxgiFactory) pdxgiFactory->Release();
#endif

	if (!CreateRenderTargetDepthStencilView()) return(false);

	return(true);
}

bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;

	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = STATEOBJ_MGR->g_pd3dDevice.Get()->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

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
	if (FAILED(hResult = STATEOBJ_MGR->g_pd3dDevice.Get()->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return(false);

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
	if (FAILED(hResult = STATEOBJ_MGR->g_pd3dDevice.Get()->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return(false);

	STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get()->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

	return(true);
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (STATEOBJ_MGR->g_pd3dImmediateDeviceContext) STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get()->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
	if (STATEOBJ_MGR->g_pd3dImmediateDeviceContext) STATEOBJ_MGR->g_pd3dDevice.Get()->Release();
	if (STATEOBJ_MGR->g_pd3dDevice) STATEOBJ_MGR->g_pd3dDevice.Get()->Release();

	// Manager Relese
	STATEOBJ_MGR->ReleseInstance();
	TEXT_MGR->ReleseInstance();
	SCENE_MGR->ReleseInstance();
	RESOURCE_MGR->ReleseInstance();
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	SCENE_MGR->m_nowScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	SCENE_MGR->m_nowScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		case VK_F1:
		case VK_F2:
		case VK_F3:
			if (m_pPlayer)
			{
				m_pPlayer->ChangeCamera(STATEOBJ_MGR->g_pd3dDevice.Get(), DWORD(wParam - VK_F1 + 1), m_GameTimer.GetTimeElapsed());
				m_pCamera = m_pPlayer->GetCamera();
				SCENE_MGR->m_nowScene->SetCamera(m_pCamera);
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
			break;
		}
		case VK_F10:
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
	switch (nMessageID)
	{
	case WM_SIZE:
	{
		m_nWndClientWidth = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);

		STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get()->OMSetRenderTargets(0, NULL, NULL);

		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();		// 왜 최소화 하면 안만들어지는가 ?
		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

		m_pDXGISwapChain->ResizeBuffers(1, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		CreateRenderTargetDepthStencilView();

		if (m_pCamera) m_pCamera->SetViewport(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get(), 0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);
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

void CGameFramework::BuildObjects()
{
	CreateShaderVariables();

	CCubeMeshDiffused *pCubeMesh = new CCubeMeshDiffused(STATEOBJ_MGR->g_pd3dDevice.Get(), 4.0f, 12.0f, 4.0f);
	m_pPlayer = new CTerrainPlayer(1);
	m_pPlayer->AddRef();
	m_pPlayer->SetMesh(pCubeMesh);
	m_pPlayer->ChangeCamera(STATEOBJ_MGR->g_pd3dDevice.Get(), THIRD_PERSON_CAMERA, 0.0f);
	CShader *pPlayerShader = new CShader();
	pPlayerShader->CreateShader(STATEOBJ_MGR->g_pd3dDevice.Get(), m_pPlayer->GetMeshType());
	m_pPlayer->SetShader(pPlayerShader);
	
	CScene* m_pScene = new CScene_Main();

	SCENE_MGR->m_nowScene = m_pScene;
	m_pScene->SetPlayer(m_pPlayer);
	m_pScene->BuildObjects(STATEOBJ_MGR->g_pd3dDevice.Get());

	CHeightMapTerrain *pTerrain = m_pScene->GetTerrain();
	float fHeight = pTerrain->GetHeight(pTerrain->GetWidth()*0.5f, pTerrain->GetLength()*0.5f, false) + 500.0f;
	m_pPlayer->SetPosition(XMVectorSet(pTerrain->GetWidth()*0.5f, fHeight, pTerrain->GetLength()*0.5f, 0.0f));
	m_pPlayer->SetPlayerUpdatedContext(pTerrain);
	m_pPlayer->SetCameraUpdatedContext(pTerrain);

	m_pCamera = m_pPlayer->GetCamera();
	m_pCamera->SetViewport(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get(), 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	m_pCamera->GenerateViewMatrix();

	m_pScene->SetCamera(m_pCamera);

	SCENE_MGR->m_pPlayer = m_pPlayer;
	SCENE_MGR->m_pCamera = m_pCamera;
}

void CGameFramework::ReleaseObjects()
{
	ReleaseShaderVariables();

	SCENE_MGR->m_nowScene->ReleaseObjects();
	delete SCENE_MGR->m_nowScene;

	if (m_pPlayer) delete m_pPlayer;
}

void CGameFramework::CreateShaderVariables()
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	STATEOBJ_MGR->g_pd3dDevice.Get()->CreateBuffer(&d3dBufferDesc, NULL, &CGameObject::m_pd3dcbWorldMatrix);

	d3dBufferDesc.ByteWidth = sizeof(XMVECTOR) * 4;
	STATEOBJ_MGR->g_pd3dDevice.Get()->CreateBuffer(&d3dBufferDesc, NULL, &CGameObject::m_pd3dcbMaterialColors);

	CCamera::CreateShaderVariables(STATEOBJ_MGR->g_pd3dDevice.Get());
	CTexture::CreateShaderVariables(STATEOBJ_MGR->g_pd3dDevice.Get());
}

void CGameFramework::ReleaseShaderVariables()
{
	CGameObject::ReleaseConstantBuffers();
	CCamera::ReleaseShaderVariables();
	CTexture::ReleaseShaderVariables();
}

void CGameFramework::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	float fPlayerSpeed = 150;

	if (GetKeyboardState(pKeysBuffer) && SCENE_MGR->m_nowScene) bProcessedByScene = SCENE_MGR->m_nowScene->ProcessInput(pKeysBuffer);
	if (!bProcessedByScene)
	{
		DWORD dwDirection = 0;
		if (pKeysBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
		if (pKeysBuffer[VK_W] & 0xF0) dwDirection |= DIR_FORWARD;

		if (pKeysBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeysBuffer[VK_S] & 0xF0) dwDirection |= DIR_BACKWARD;

		if (pKeysBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeysBuffer[VK_A] & 0xF0) dwDirection |= DIR_LEFT;

		if (pKeysBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
		if (pKeysBuffer[VK_D] & 0xF0) dwDirection |= DIR_RIGHT;

		if (pKeysBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
		if (pKeysBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;

		if (pKeysBuffer[VK_SHIFT] & 0xF0) { fPlayerSpeed *= 10; }

		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;
		if (GetCapture() == m_hWnd)
		{
			SetCursor(NULL);
			GetCursorPos(&ptCursorPos);
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}

		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				if (pKeysBuffer[VK_RBUTTON] & 0xF0)
					m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
				else
					m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
			}
			if (dwDirection) m_pPlayer->Move(dwDirection, fPlayerSpeed * m_GameTimer.GetTimeElapsed(), true);
		}
	}
	m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::UpdateObjects()
{
	float fTimeElapsed = m_GameTimer.GetTimeElapsed();

	if (m_pPlayer) 
		m_pPlayer->Animate(fTimeElapsed, NULL);

	SCENE_MGR->m_nowScene->UpdateObjects(fTimeElapsed);
}

//#define _WITH_PLAYER_TOP

void CGameFramework::FrameAdvance()
{
	m_GameTimer.Tick();

	ProcessInput();

	UpdateObjects();

	SCENE_MGR->m_nowScene->OnPreRender(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get());

	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	if (m_pd3dRenderTargetView) STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get()->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	if (m_pd3dDepthStencilView) STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get()->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get()->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

	if (m_pPlayer) m_pPlayer->UpdateShaderVariables(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get());
	m_pCamera->SetViewport(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get());

	// WireFrame Mode
	if (GetAsyncKeyState('3') & 0x8000)
		STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get()->RSSetState(STATEOBJ_MGR->g_pWireframeRS);
	else 
		STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get()->RSSetState(STATEOBJ_MGR->g_pDefaultRS);
	

	SCENE_MGR->m_nowScene->Render(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get(), m_pCamera);

#ifdef _WITH_PLAYER_TOP
	g_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
#endif
	if (m_pPlayer) 
		m_pPlayer->Render(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get(), m_pCamera);

	SCENE_MGR->m_nowScene->RenderAllText(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get());		// 텍스트는 항상 제일 마지막에 그려야 정상적으로 그려짐

	m_pDXGISwapChain->Present(0, 0);

	m_GameTimer.GetFrameRate(m_pszBuffer + 16, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}
