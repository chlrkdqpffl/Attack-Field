// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//#include <D3D9Types.h>
#include <Mmsystem.h>
#include <iostream>

// DirectX 11
#include <d3d11.h>
#include <d3dx11.h>
#include <D3DX10Math.h>
#include <d3dcompiler.h>

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
#include <fstream>
#include <stack>
#include <algorithm>
#include <memory>
#include <process.h>
#include <utility>

// Comptr
#include <wrl.h>
#include <wrl\client.h>
using namespace Microsoft::WRL;


using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace std;

#include "ConstantBuffer_Slot.h"
#include "UserDefine.h"

extern void TRACE(_TCHAR *pString);
extern void TRACE(char *pString);
extern void TRACE(_TCHAR *pString, UINT uValue);
extern void TRACE(_TCHAR *pString, int nValue);
extern void TRACE(_TCHAR *pString, int nValue0, int nValue1);
extern void TRACE(_TCHAR *pString, float fValue);

/*
inline D3DXVECTOR3&& XMVecToD3DXVec3(const XMVECTOR& xmVec)
{
	XMFLOAT3 xmf3;
	XMStoreFloat3(&xmf3, xmVec);
	return std::move(D3DXVECTOR3(xmf3.x, xmf3.y, xmf3.z));
}
inline XMFLOAT3 XMVFLOAT3Converter(const XMVECTOR& xmVec)
{
	XMFLOAT3 xmf3;
	XMStoreFloat3(&xmf3, xmVec);
	return xmf3;
}

inline XMVECTOR&& XMVECTORConverter(const XMFLOAT3& d3dxv, float wParam = 0.f)
{
	return std::move(XMVectorSet(d3dxv.x, d3dxv.y, d3dxv.z, 0.0f));
}

inline XMVECTOR&& D3DXVec3ToXMVec(const D3DXVECTOR3& d3dxv, float wParam = 0.f)
{
	return std::move(XMVectorSet(d3dxv.x, d3dxv.y, d3dxv.z, 0.0f));
}

inline BoundingBox GetBoudingBox(const D3DXVECTOR3 &fMin, const D3DXVECTOR3 &fMax)
{
	BoundingBox boundingbox;
	boundingbox.Center = { (fMin.x + fMax.x) * 0.5f, (fMin.y + fMax.y) * 0.5f, (fMin.z + fMax.z) * 0.5f };
	boundingbox.Extents = { (fMin.x - fMax.x) * 0.5f, (fMin.y - fMax.y) * 0.5f, (fMin.z - fMax.z) * 0.5f };
	return boundingbox;
}
*/

#if defined(DEBUG) || defined(_DEBUG)
	#ifndef HR
		#define HR(x) {HRESULT hr = (x); if (FAILED(hr)) DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);}
	#endif
#else
	#ifndef HR
		#define HR(x) (x)
	#endif
#endif