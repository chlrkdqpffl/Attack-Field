#pragma once

//#define _WITH_MSAA4_MULTISAMPLING
//#define FRAME_BUFFER_WIDTH				1280
//#define FRAME_BUFFER_HEIGHT				768

#define PROJECT_NAME					L"DX11_Framework ("

#define FRAME_BUFFER_WIDTH				1600
#define FRAME_BUFFER_HEIGHT				900

#define CUBEMAP_RENDER_TARGET_WIDTH		1024
#define CUBEMAP_RENDER_TARGET_HEIGHT	1024

#define SHADOW_RENDER_TARGET_WIDTH		1024
#define SHADOW_RENDER_TARGET_HEIGHT		1024

#define RANDOM_COLOR	D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)
#define MAXBONECOUNT					80

#define _WITH_TERRAIN_PARTITION
#define _WITH_FRUSTUM_CULLING_BY_OBJECT
//#define _WITH_FRUSTUM_CULLING_BY_SUBMESH

//#define _WITH_SKYBOX_TEXTURE_ARRAY
#define _WITH_SKYBOX_TEXTURE_CUBE
//#define _WITH_TERRAIN_TEXTURE_ARRAY

#define USE_CONSOLE					// Open Console Windows

// --------------------- Manager -------------------- //
#define STATEOBJ_MGR CStateObjectManager::GetInstance()
#define RESOURCE_MGR CResourceManager::GetInstance()
#define SCENE_MGR CSceneManager::GetInstance()
#define TEXT_MGR CTextManager::GetInstance()
#define TWBAR_MGR CAntTweakBarManager::GetInstance()

// Check Timer
#define STARTTIMER(msg)			{ cout << msg; STATEOBJ_MGR->g_startTime = chrono::system_clock::now();}
#define ENDTIMER				{STATEOBJ_MGR->g_timeGap = chrono::duration_cast<chrono::milliseconds> (chrono::system_clock::now() - STATEOBJ_MGR->g_startTime); \
								 cout << STATEOBJ_MGR->g_timeGap.count() << " milliseconds" << endl;}

// Returns random float in [a, b).
#define RAND_FLOAT(a, b) float(a + ((float)(rand()) / (float)RAND_MAX) * (b - a))

//---------------------------------------------------------------------------------------
// Convenience macro for releasing, deleteing COM objects.
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define SafeDelete(x) { delete x; x = nullptr; }
//---------------------------------------------------------------------------------------

#define ANIMFRAMETIME			0.0333333f

// 사진 저장 매크로 D3DX11SaveTextureToFile(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get(), &texture2d, D3DX11_IFF_JPG, L"save.jpg");

#define VK_A 0x41 
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44	
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

namespace Colors
{
	XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}

#if defined(DEBUG) || defined(_DEBUG)
	#ifndef HR
		#define HR(x) {HRESULT hr = (x); if (FAILED(hr)) DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);}
	#endif
#else
	#ifndef HR
		#define HR(x) (x)
	#endif
#endif

template <typename T >
void print(const T& firstArg)
{
	cout << firstArg << ", ";
}

template <typename T, typename ...Types >
void print(const T& firstArg, const Types& ...argv)
{
	cout << firstArg << ", ";
	print(argv...);
}

inline void ShowXMMatrix(XMMATRIX mtx)
{
	XMFLOAT4X4 mtxOut;
	XMStoreFloat4x4(&mtxOut, mtx);
	cout << mtxOut._11 << ", " << mtxOut._12 << ", " << mtxOut._13 << ", " << mtxOut._14 << endl;
	cout << mtxOut._21 << ", " << mtxOut._22 << ", " << mtxOut._23 << ", " << mtxOut._24 << endl;
	cout << mtxOut._31 << ", " << mtxOut._32 << ", " << mtxOut._33 << ", " << mtxOut._34 << endl;
	cout << mtxOut._41 << ", " << mtxOut._42 << ", " << mtxOut._43 << ", " << mtxOut._44 << endl << endl;
}

inline void ShowXMFloat4x4(XMFLOAT4X4 mtx)
{
	cout << mtx._11 << ", " << mtx._12 << ", " << mtx._13 << ", " << mtx._14 << endl;
	cout << mtx._21 << ", " << mtx._22 << ", " << mtx._23 << ", " << mtx._24 << endl;
	cout << mtx._31 << ", " << mtx._32 << ", " << mtx._33 << ", " << mtx._34 << endl;
	cout << mtx._41 << ", " << mtx._42 << ", " << mtx._43 << ", " << mtx._44 << endl << endl;
}

inline void ShowXMFloat4(XMFLOAT4 xmf4)
{
	cout << xmf4.x << ", " << xmf4.y << ", " << xmf4.z << ", " << xmf4.w << endl << endl;
}

inline void ShowXMFloat3(XMFLOAT3 xmf3)
{
	cout << xmf3.x << ", " << xmf3.y << ", " << xmf3.z << endl << endl;
}

inline void ShowXMVector(XMVECTOR xmVector)
{
	XMFLOAT4 out;
	XMStoreFloat4(&out, xmVector);
	cout << out.x << ", " << out.y << ", " << out.z << ", " << out.w << endl << endl;
}

inline void ShowTaskSuccess(string message)
{
#if defined(DEBUG) || defined(_DEBUG)
	cout << message << endl;
#endif
}

inline void ShowTaskFail(string message)
{
#if defined(DEBUG) || defined(_DEBUG)
	cout << message << endl;
	system("pause");
#endif
}