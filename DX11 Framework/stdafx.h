// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
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
//
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace std;

#pragma comment(lib, "legacy_stdio_definitions.lib")		// DXTrace

extern void TRACE(_TCHAR *pString);
extern void TRACE(char *pString);
extern void TRACE(_TCHAR *pString, UINT uValue);
extern void TRACE(_TCHAR *pString, int nValue);
extern void TRACE(_TCHAR *pString, int nValue0, int nValue1);
extern void TRACE(_TCHAR *pString, float fValue);


#include "ConstantBuffer_Slot.h"
#include "UserDefine.h"
#include "UserTag.h"

// ------- Manager Header -------- //
#include "StateObjectManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "TextManager.h"
// ------------------------------- //