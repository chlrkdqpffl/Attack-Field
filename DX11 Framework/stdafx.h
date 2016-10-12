// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

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

#include <d3d11.h>
#include <d3dx11.h>
#include <D3DX10Math.h>
#include <d3dcompiler.h>

//#include <D3D9Types.h>

#include <Mmsystem.h>


/*--------------------------------*/
#define _AABB_
//#define _SPACE_PAT

/*--------------------------------*/




// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

//#include <xnamath.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

using namespace DirectX;
using namespace DirectX::PackedVector;


#include <vector>
#include <list>
#include <map>
#include <string>
#include <fstream>
#include <stack>
#include <algorithm>
#include <memory>
#include <process.h>


//#define _WITH_MSAA4_MULTISAMPLING
#define FRAME_BUFFER_WIDTH				640
#define FRAME_BUFFER_HEIGHT				480

#define CUBEMAP_RENDER_TARGET_WIDTH		1024
#define CUBEMAP_RENDER_TARGET_HEIGHT	1024

#define SHADOW_RENDER_TARGET_WIDTH		1024
#define SHADOW_RENDER_TARGET_HEIGHT		1024

#define VS_CB_SLOT_CAMERA				0x00
#define VS_CB_SLOT_WORLD_MATRIX			0x01
#define VS_CB_SLOT_TEXTURE_MATRIX		0x02
#define VS_CB_SLOT_TERRAIN				0x03
#define VS_CB_SLOT_SKYBOX				0x04
#define VS_CB_SLOT_PROJECTION			0x05
#define VS_CB_SLOT_SHADOW				0x06

#define PS_CB_SLOT_LIGHT				0x00
#define PS_CB_SLOT_MATERIAL				0x01
#define PS_CB_SLOT_TERRAIN				0x03
#define PS_CB_SLOT_SKYBOX				0x04

#define PS_SLOT_TEXTURE					0x00
#define PS_SLOT_TEXTURE_TERRAIN			0x02
#define PS_SLOT_TEXTURE_SKYBOX			0x0D
#define PS_SLOT_TEXTURE_CUBEMAPPED		0x0E
#define PS_SLOT_TEXTURE_PROJECTION		0x0F
#define PS_SLOT_TEXTURE_PROJECTED_DEPTH	0x10
#define PS_SLOT_TEXTURE_SHADOW			0x11

#define PS_SLOT_SAMPLER					0x00
#define PS_SLOT_SAMPLER_DETAIL			0x01
#define PS_SLOT_SAMPLER_TERRAIN			0x02
#define PS_SLOT_SAMPLER_SKYBOX			0x04
#define PS_SLOT_SAMPLER_CUBEMAPPED		0x05
#define PS_SLOT_SAMPLER_PROJECTION		0x06
#define PS_SLOT_SAMPLER_SHADOW			0x07

#define GS_CB_SLOT_CAMERA				0x00

#define RANDOM_COLOR	D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

#define _WITH_TERRAIN_PARTITION
#define _WITH_FRUSTUM_CULLING_BY_OBJECT
//#define _WITH_FRUSTUM_CULLING_BY_SUBMESH

//#define _WITH_SKYBOX_TEXTURE_ARRAY
#define _WITH_SKYBOX_TEXTURE_CUBE
//#define _WITH_TERRAIN_TEXTURE_ARRAY

extern void TRACE(_TCHAR *pString);
extern void TRACE(char *pString);
extern void TRACE(_TCHAR *pString, UINT uValue);
extern void TRACE(_TCHAR *pString, int nValue);
extern void TRACE(_TCHAR *pString, int nValue0, int nValue1);
extern void TRACE(_TCHAR *pString, float fValue);

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

