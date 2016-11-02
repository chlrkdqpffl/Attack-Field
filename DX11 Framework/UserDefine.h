#pragma once

//#define _WITH_MSAA4_MULTISAMPLING
#define FRAME_BUFFER_WIDTH				1280
#define FRAME_BUFFER_HEIGHT				768

#define CUBEMAP_RENDER_TARGET_WIDTH		1024
#define CUBEMAP_RENDER_TARGET_HEIGHT	1024

#define SHADOW_RENDER_TARGET_WIDTH		1024
#define SHADOW_RENDER_TARGET_HEIGHT		1024

#define RANDOM_COLOR	D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

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

// Returns random float in [a, b).
#define RAND_FLOAT(a, b) float(a + ((float)(rand()) / (float)RAND_MAX) * (b - a));

//---------------------------------------------------------------------------------------
// Convenience macro for releasing, deleteing COM objects.
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define SafeDelete(x) { delete x; x = 0; }
//---------------------------------------------------------------------------------------

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