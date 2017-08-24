#include "stdafx.h"
#include "GlobalVariableManager.h"

XMFLOAT4		CGlobalVariableManager::g_vRenderOption = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
ID3D11Buffer*	CGlobalVariableManager::g_pd3dcbRenderOption = nullptr;
ID3D11Buffer*	CGlobalVariableManager::g_pd3dcbTessOption = nullptr;

bool			CGlobalVariableManager::g_bShowGBuffer = false;
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
	HR(STATEOBJ_MGR->g_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &g_pd3dcbRenderOption));
	DXUT_SetDebugName(g_pd3dcbRenderOption, "Render Option");

	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_RENDEROPTION, 1, &g_pd3dcbRenderOption);

	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(TessOption);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = &TWBAR_MGR->g_tessOption;
	HR(STATEOBJ_MGR->g_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &g_pd3dcbTessOption));
	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->HSSetConstantBuffers(HSDS_CB_SLOT_TESS_OPTION, 1, &g_pd3dcbTessOption);
	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->DSSetConstantBuffers(HSDS_CB_SLOT_TESS_OPTION, 1, &g_pd3dcbTessOption);
	DXUT_SetDebugName(g_pd3dcbTessOption, "Tess Option");
}

void CGlobalVariableManager::UpdateManager()
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Map(g_pd3dcbTessOption, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	TessOption *pcbTessOption = (TessOption*)d3dMappedResource.pData;
	*pcbTessOption = TWBAR_MGR->g_tessOption;
	STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Unmap(g_pd3dcbTessOption, 0);
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