#include "stdafx.h"
#include "GlobalVariableManager.h"

XMFLOAT4		CGlobalVariableManager::g_vRenderOption = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
ID3D11Buffer*	CGlobalVariableManager::g_pd3dcbRenderOption = nullptr;

bool			CGlobalVariableManager::g_bShowGBuffer = true;
bool			CGlobalVariableManager::g_bShowWorldAxis = false;
bool			CGlobalVariableManager::g_bShowWireFrame = false;

CGlobalVariableManager::CGlobalVariableManager()
{
}

CGlobalVariableManager::~CGlobalVariableManager()
{
}

void CGlobalVariableManager::InitializeManager()
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	D3D11_SUBRESOURCE_DATA d3dBufferData;

	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT4);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = &g_vRenderOption;
	STATEOBJ_MGR->g_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &g_pd3dcbRenderOption);

	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_RENDEROPTION, 1, &g_pd3dcbRenderOption);
	DXUT_SetDebugName(g_pd3dcbRenderOption, "Render Option");
}

void CGlobalVariableManager::ReleseManager()
{
	ReleaseCOM(g_pd3dcbRenderOption);

	for (auto& object : g_vecLineContainer)
		SafeDelete(object);
}

bool CGlobalVariableManager::SetTimer(DWORD& timeVar, int time)
{
	if (GetTickCount() - timeVar >= time) {
		timeVar = GetTickCount();
		return true;
	}
	else
		return false;
}