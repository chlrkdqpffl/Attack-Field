// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <WinSock2.h>
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <Mmsystem.h>

//#include <D3D9Types.h>
#include <iostream>
#include <chrono>
// wstring to string
#include <codecvt>

// DirectX 11
#include <d3d11.h>
#include <d3dx11.h>
#include <D3DX10Math.h>
#include <d3dcompiler.h>
#include <DxErr.h>

// DirectX Math
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

// STL
#include <vector>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <fstream>
#include <stack>
#include <algorithm>
#include <memory>
#include <process.h>
#include <utility>

// AntTweakBar
#include <AntTweakBar.h>

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace std;

#if defined(DEBUG) || defined(_DEBUG)
// Memory Leak Check
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#pragma comment(lib, "legacy_stdio_definitions.lib")		// DXTrace
#pragma comment(lib, "ws2_32.lib")							// 서버

#include "ConstantBuffer_Slot.h"
#include "UserDefine.h"
#include "UserTag.h"

// ------- Manager Header -------- //
#include "ServerManager.h"
#include "GlobalVariableManager.h"
#include "StateObjectManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "TextManager.h"
#include "AntTweakBarManager.h"
#include "CollisionManager.h"
#include "MapDataManager.h"
#include "SoundManager.h"
#include "ParticleManager.h"
#include "SpriteImageManager.h"
// ------------------------------- //

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
	return max(lower, min(n, upper));
}

inline char* ConvertWCtoC(const WCHAR* str)
{
	char* pStr;
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	pStr = new char[strSize];

	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}

inline WCHAR* ConverCtoWC(const char* str)
{
	WCHAR* pStr;
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	pStr = new WCHAR[strSize];

	MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, pStr, strSize);
	return pStr;
}

inline std::wstring s_to_ws(const std::string& str, std::locale Loc = std::locale("ko"))
{
	using conv_Ty = std::codecvt_utf8_utf16<wchar_t>;
	return std::wstring_convert<conv_Ty, wchar_t> {&std::use_facet<conv_Ty>(Loc) }.from_bytes(str);
}

inline std::string ws_to_s(const std::wstring& wstr, std::locale Loc = std::locale("ko"))
{
	using conv_Ty = std::codecvt_utf8_utf16<wchar_t>;
	return std::wstring_convert<conv_Ty, wchar_t> {&std::use_facet<conv_Ty>(Loc) }.to_bytes(wstr);
}

#if defined(_DEBUG) || defined(DEBUG)

inline void DXUT_SetDebugName(ID3D11Resource* pObj, const CHAR* pstrName)
{
	if (pObj)
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
}

inline void DXUT_SetDebugName(IDXGIObject* pObj, const CHAR* pstrName)
{
	if (pObj)
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
}

inline void DXUT_SetDebugName(ID3D11Device* pObj, const CHAR* pstrName)
{
	if (pObj)
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
}

inline void DXUT_SetDebugName(ID3D11DeviceChild* pObj, const CHAR* pstrName)
{
	if (pObj)
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
}

inline void DXUT_SetDebugName(ID3D11Resource* pObj, const WCHAR* pstrName)
{
	if (pObj) {
		char* str = ConvertWCtoC(pstrName);
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(str), str);
	}
}

inline void DXUT_SetDebugName(IDXGIObject* pObj, const WCHAR* pstrName)
{
	if (pObj) {
		char* str = ConvertWCtoC(pstrName);
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(str), str);
	}
}

inline void DXUT_SetDebugName(ID3D11Device* pObj, const WCHAR* pstrName)
{
	if (pObj) {
		char* str = ConvertWCtoC(pstrName);
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(str), str);
	}
}

inline void DXUT_SetDebugName(ID3D11DeviceChild* pObj, const WCHAR* pstrName)
{
	if (pObj) {
		char* str = ConvertWCtoC(pstrName);
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(str), str);
	}
}

#else
	#define DXUT_SetDebugName( pObj, pstrName )
#endif


inline void ShowXMMatrix(XMMATRIX& mtx)
{
	XMFLOAT4X4 mtxOut;
	XMStoreFloat4x4(&mtxOut, mtx);
	cout << mtxOut._11 << ", " << mtxOut._12 << ", " << mtxOut._13 << ", " << mtxOut._14 << endl;
	cout << mtxOut._21 << ", " << mtxOut._22 << ", " << mtxOut._23 << ", " << mtxOut._24 << endl;
	cout << mtxOut._31 << ", " << mtxOut._32 << ", " << mtxOut._33 << ", " << mtxOut._34 << endl;
	cout << mtxOut._41 << ", " << mtxOut._42 << ", " << mtxOut._43 << ", " << mtxOut._44 << endl << endl;
}

inline void ShowXMFloat4x4(XMFLOAT4X4& mtx)
{
	cout << mtx._11 << ", " << mtx._12 << ", " << mtx._13 << ", " << mtx._14 << endl;
	cout << mtx._21 << ", " << mtx._22 << ", " << mtx._23 << ", " << mtx._24 << endl;
	cout << mtx._31 << ", " << mtx._32 << ", " << mtx._33 << ", " << mtx._34 << endl;
	cout << mtx._41 << ", " << mtx._42 << ", " << mtx._43 << ", " << mtx._44 << endl << endl;
}

inline void ShowXMFloat4(XMFLOAT4& xmf4)
{
	cout << xmf4.x << ", " << xmf4.y << ", " << xmf4.z << ", " << xmf4.w << endl;
}

inline void ShowXMFloat3(XMFLOAT3& xmf3)
{
	cout << xmf3.x << ", " << xmf3.y << ", " << xmf3.z << endl;
}

inline void ShowXMVector(XMVECTOR& xmVector)
{
	XMFLOAT4 out;
	XMStoreFloat4(&out, xmVector);
	cout << out.x << ", " << out.y << ", " << out.z << ", " << out.w << endl;
}

inline void ShowTaskSuccess(string message)
{
	cout << message << endl;
}

inline void ShowTaskFail(string message)
{
	cout << message << endl;
	system("pause");
}

inline void ShowTag(TextureTag tag)
{
	cout << static_cast<int> (tag) << endl;
}

/*
LPCWSTR WINAPI DXUTDXGIFormatToString(DXGI_FORMAT format, bool bWithPrefix)
{
	WCHAR* pstr = NULL;
	switch (format)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		pstr = L"DXGI_FORMAT_R32G32B32A32_TYPELESS"; break;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		pstr = L"DXGI_FORMAT_R32G32B32A32_FLOAT"; break;
	case DXGI_FORMAT_R32G32B32A32_UINT:
		pstr = L"DXGI_FORMAT_R32G32B32A32_UINT"; break;
	case DXGI_FORMAT_R32G32B32A32_SINT:
		pstr = L"DXGI_FORMAT_R32G32B32A32_SINT"; break;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
		pstr = L"DXGI_FORMAT_R32G32B32_TYPELESS"; break;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		pstr = L"DXGI_FORMAT_R32G32B32_FLOAT"; break;
	case DXGI_FORMAT_R32G32B32_UINT:
		pstr = L"DXGI_FORMAT_R32G32B32_UINT"; break;
	case DXGI_FORMAT_R32G32B32_SINT:
		pstr = L"DXGI_FORMAT_R32G32B32_SINT"; break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		pstr = L"DXGI_FORMAT_R16G16B16A16_TYPELESS"; break;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		pstr = L"DXGI_FORMAT_R16G16B16A16_FLOAT"; break;
	case DXGI_FORMAT_R16G16B16A16_UNORM:
		pstr = L"DXGI_FORMAT_R16G16B16A16_UNORM"; break;
	case DXGI_FORMAT_R16G16B16A16_UINT:
		pstr = L"DXGI_FORMAT_R16G16B16A16_UINT"; break;
	case DXGI_FORMAT_R16G16B16A16_SNORM:
		pstr = L"DXGI_FORMAT_R16G16B16A16_SNORM"; break;
	case DXGI_FORMAT_R16G16B16A16_SINT:
		pstr = L"DXGI_FORMAT_R16G16B16A16_SINT"; break;
	case DXGI_FORMAT_R32G32_TYPELESS:
		pstr = L"DXGI_FORMAT_R32G32_TYPELESS"; break;
	case DXGI_FORMAT_R32G32_FLOAT:
		pstr = L"DXGI_FORMAT_R32G32_FLOAT"; break;
	case DXGI_FORMAT_R32G32_UINT:
		pstr = L"DXGI_FORMAT_R32G32_UINT"; break;
	case DXGI_FORMAT_R32G32_SINT:
		pstr = L"DXGI_FORMAT_R32G32_SINT"; break;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
		pstr = L"DXGI_FORMAT_R32G8X24_TYPELESS"; break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		pstr = L"DXGI_FORMAT_D32_FLOAT_S8X24_UINT"; break;
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		pstr = L"DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS"; break;
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		pstr = L"DXGI_FORMAT_X32_TYPELESS_G8X24_UINT"; break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		pstr = L"DXGI_FORMAT_R10G10B10A2_TYPELESS"; break;
	case DXGI_FORMAT_R10G10B10A2_UNORM:
		pstr = L"DXGI_FORMAT_R10G10B10A2_UNORM"; break;
	case DXGI_FORMAT_R10G10B10A2_UINT:
		pstr = L"DXGI_FORMAT_R10G10B10A2_UINT"; break;
	case DXGI_FORMAT_R11G11B10_FLOAT:
		pstr = L"DXGI_FORMAT_R11G11B10_FLOAT"; break;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		pstr = L"DXGI_FORMAT_R8G8B8A8_TYPELESS"; break;
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		pstr = L"DXGI_FORMAT_R8G8B8A8_UNORM"; break;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		pstr = L"DXGI_FORMAT_R8G8B8A8_UNORM_SRGB"; break;
	case DXGI_FORMAT_R8G8B8A8_UINT:
		pstr = L"DXGI_FORMAT_R8G8B8A8_UINT"; break;
	case DXGI_FORMAT_R8G8B8A8_SNORM:
		pstr = L"DXGI_FORMAT_R8G8B8A8_SNORM"; break;
	case DXGI_FORMAT_R8G8B8A8_SINT:
		pstr = L"DXGI_FORMAT_R8G8B8A8_SINT"; break;
	case DXGI_FORMAT_R16G16_TYPELESS:
		pstr = L"DXGI_FORMAT_R16G16_TYPELESS"; break;
	case DXGI_FORMAT_R16G16_FLOAT:
		pstr = L"DXGI_FORMAT_R16G16_FLOAT"; break;
	case DXGI_FORMAT_R16G16_UNORM:
		pstr = L"DXGI_FORMAT_R16G16_UNORM"; break;
	case DXGI_FORMAT_R16G16_UINT:
		pstr = L"DXGI_FORMAT_R16G16_UINT"; break;
	case DXGI_FORMAT_R16G16_SNORM:
		pstr = L"DXGI_FORMAT_R16G16_SNORM"; break;
	case DXGI_FORMAT_R16G16_SINT:
		pstr = L"DXGI_FORMAT_R16G16_SINT"; break;
	case DXGI_FORMAT_R32_TYPELESS:
		pstr = L"DXGI_FORMAT_R32_TYPELESS"; break;
	case DXGI_FORMAT_D32_FLOAT:
		pstr = L"DXGI_FORMAT_D32_FLOAT"; break;
	case DXGI_FORMAT_R32_FLOAT:
		pstr = L"DXGI_FORMAT_R32_FLOAT"; break;
	case DXGI_FORMAT_R32_UINT:
		pstr = L"DXGI_FORMAT_R32_UINT"; break;
	case DXGI_FORMAT_R32_SINT:
		pstr = L"DXGI_FORMAT_R32_SINT"; break;
	case DXGI_FORMAT_R24G8_TYPELESS:
		pstr = L"DXGI_FORMAT_R24G8_TYPELESS"; break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		pstr = L"DXGI_FORMAT_D24_UNORM_S8_UINT"; break;
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		pstr = L"DXGI_FORMAT_R24_UNORM_X8_TYPELESS"; break;
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		pstr = L"DXGI_FORMAT_X24_TYPELESS_G8_UINT"; break;
	case DXGI_FORMAT_R8G8_TYPELESS:
		pstr = L"DXGI_FORMAT_R8G8_TYPELESS"; break;
	case DXGI_FORMAT_R8G8_UNORM:
		pstr = L"DXGI_FORMAT_R8G8_UNORM"; break;
	case DXGI_FORMAT_R8G8_UINT:
		pstr = L"DXGI_FORMAT_R8G8_UINT"; break;
	case DXGI_FORMAT_R8G8_SNORM:
		pstr = L"DXGI_FORMAT_R8G8_SNORM"; break;
	case DXGI_FORMAT_R8G8_SINT:
		pstr = L"DXGI_FORMAT_R8G8_SINT"; break;
	case DXGI_FORMAT_R16_TYPELESS:
		pstr = L"DXGI_FORMAT_R16_TYPELESS"; break;
	case DXGI_FORMAT_R16_FLOAT:
		pstr = L"DXGI_FORMAT_R16_FLOAT"; break;
	case DXGI_FORMAT_D16_UNORM:
		pstr = L"DXGI_FORMAT_D16_UNORM"; break;
	case DXGI_FORMAT_R16_UNORM:
		pstr = L"DXGI_FORMAT_R16_UNORM"; break;
	case DXGI_FORMAT_R16_UINT:
		pstr = L"DXGI_FORMAT_R16_UINT"; break;
	case DXGI_FORMAT_R16_SNORM:
		pstr = L"DXGI_FORMAT_R16_SNORM"; break;
	case DXGI_FORMAT_R16_SINT:
		pstr = L"DXGI_FORMAT_R16_SINT"; break;
	case DXGI_FORMAT_R8_TYPELESS:
		pstr = L"DXGI_FORMAT_R8_TYPELESS"; break;
	case DXGI_FORMAT_R8_UNORM:
		pstr = L"DXGI_FORMAT_R8_UNORM"; break;
	case DXGI_FORMAT_R8_UINT:
		pstr = L"DXGI_FORMAT_R8_UINT"; break;
	case DXGI_FORMAT_R8_SNORM:
		pstr = L"DXGI_FORMAT_R8_SNORM"; break;
	case DXGI_FORMAT_R8_SINT:
		pstr = L"DXGI_FORMAT_R8_SINT"; break;
	case DXGI_FORMAT_A8_UNORM:
		pstr = L"DXGI_FORMAT_A8_UNORM"; break;
	case DXGI_FORMAT_R1_UNORM:
		pstr = L"DXGI_FORMAT_R1_UNORM"; break;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		pstr = L"DXGI_FORMAT_R9G9B9E5_SHAREDEXP"; break;
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
		pstr = L"DXGI_FORMAT_R8G8_B8G8_UNORM"; break;
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		pstr = L"DXGI_FORMAT_G8R8_G8B8_UNORM"; break;
	case DXGI_FORMAT_BC1_TYPELESS:
		pstr = L"DXGI_FORMAT_BC1_TYPELESS"; break;
	case DXGI_FORMAT_BC1_UNORM:
		pstr = L"DXGI_FORMAT_BC1_UNORM"; break;
	case DXGI_FORMAT_BC1_UNORM_SRGB:
		pstr = L"DXGI_FORMAT_BC1_UNORM_SRGB"; break;
	case DXGI_FORMAT_BC2_TYPELESS:
		pstr = L"DXGI_FORMAT_BC2_TYPELESS"; break;
	case DXGI_FORMAT_BC2_UNORM:
		pstr = L"DXGI_FORMAT_BC2_UNORM"; break;
	case DXGI_FORMAT_BC2_UNORM_SRGB:
		pstr = L"DXGI_FORMAT_BC2_UNORM_SRGB"; break;
	case DXGI_FORMAT_BC3_TYPELESS:
		pstr = L"DXGI_FORMAT_BC3_TYPELESS"; break;
	case DXGI_FORMAT_BC3_UNORM:
		pstr = L"DXGI_FORMAT_BC3_UNORM"; break;
	case DXGI_FORMAT_BC3_UNORM_SRGB:
		pstr = L"DXGI_FORMAT_BC3_UNORM_SRGB"; break;
	case DXGI_FORMAT_BC4_TYPELESS:
		pstr = L"DXGI_FORMAT_BC4_TYPELESS"; break;
	case DXGI_FORMAT_BC4_UNORM:
		pstr = L"DXGI_FORMAT_BC4_UNORM"; break;
	case DXGI_FORMAT_BC4_SNORM:
		pstr = L"DXGI_FORMAT_BC4_SNORM"; break;
	case DXGI_FORMAT_BC5_TYPELESS:
		pstr = L"DXGI_FORMAT_BC5_TYPELESS"; break;
	case DXGI_FORMAT_BC5_UNORM:
		pstr = L"DXGI_FORMAT_BC5_UNORM"; break;
	case DXGI_FORMAT_BC5_SNORM:
		pstr = L"DXGI_FORMAT_BC5_SNORM"; break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		pstr = L"DXGI_FORMAT_B5G6R5_UNORM"; break;
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		pstr = L"DXGI_FORMAT_B5G5R5A1_UNORM"; break;
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		pstr = L"DXGI_FORMAT_B8G8R8A8_UNORM"; break;
	default:
		pstr = L"Unknown format"; break;
	}
	if (bWithPrefix || wcsstr(pstr, L"DXGI_FORMAT_") == NULL)
		return pstr;
	else
		return pstr + lstrlen(L"DXGI_FORMAT_");
}
*/

extern void TRACE(_TCHAR *pString);
extern void TRACE(char *pString);
extern void TRACE(_TCHAR *pString, UINT uValue);
extern void TRACE(_TCHAR *pString, int nValue);
extern void TRACE(_TCHAR *pString, int nValue0, int nValue1);
extern void TRACE(_TCHAR *pString, float fValue);
extern SOCKET g_socket;
