#include "stdafx.h"
#include "DirectX11_Project.h"
#include "GameFramework.h"

#define MAX_LOADSTRING 100

TCHAR				szTitle[MAX_LOADSTRING];				// 제목 표시줄 텍스트입니다.
TCHAR				szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

CGameFramework		gGameFramework;
SOCKET g_socket;

BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LABPROJECT14, szWindowClass, MAX_LOADSTRING);
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABPROJECT14));

	if (!InitInstance(hInstance, nCmdShow)) return(FALSE);

	MSG msg = {0};
	while (1) 
    {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        {
			if (msg.message == WM_QUIT) break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} 
        else 
        {
			gGameFramework.FrameAdvance();
		}
	}

	gGameFramework.OnDestroy();

	return((int)msg.wParam);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABPROJECT14));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL/*MAKEINTRESOURCE(IDC_LABPROJECT14)*/;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassEx(&wcex)) return(FALSE);

	RECT rc = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
	// 윈도우 창 크기 조절		
	//DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	AdjustWindowRect(&rc, dwStyle, FALSE);
	
	HWND hMainWnd = CreateWindow(szWindowClass, szTitle, dwStyle, 150, 30, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);	// 윈도우 창 시작 위치 조절 가능
	if (!hMainWnd) return(FALSE);

	SERVER_MGR->sethandle(hMainWnd);

#ifdef	USE_SERVER
	//SERVER_MGR->Server_init();
#endif
	if (!gGameFramework.OnCreate(hInstance, hMainWnd)) return(FALSE);

	ShowWindow(hMainWnd, nCmdShow);

	return(TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
        case WM_SIZE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
        case WM_KEYDOWN:
        case WM_KEYUP:
		case WM_CHAR:
			gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch (wmId)
			{
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
#ifdef	USE_SERVER
		case WM_SOCKET:
		{
			if (WSAGETSELECTERROR(lParam)) {
				closesocket((SOCKET)wParam);
				exit(-1);
				break;
			}
			switch (WSAGETSELECTEVENT(lParam)) {
			case FD_READ:
				SERVER_MGR->ReadPacket((SOCKET)wParam);
				break;
			case FD_CLOSE:
				closesocket((SOCKET)wParam);
				exit(-1);
				break;
			}
		}
		break;
#endif
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return(0);
}
