//-----------------------------------------------------------------------------
// File: Scene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"
#include "resource.h"

CScene::CScene()
{
	m_ppObjects = NULL;
	m_nObjects = 0;

	m_ppObjectShaders = NULL;
	m_nObjectShaders = 0;

	m_ppInstancingShaders = NULL;
	m_nInstancingShaders = 0;

	m_pLights = NULL;
	m_pd3dcbLights = NULL;

	m_pCamera = NULL;
	m_pSelectedObject = NULL;

	m_pSkyBox = nullptr;
	m_pTerrain = nullptr;
}

CScene::~CScene()
{
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		m_pSelectedObject = PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONUP:
		break;
	default:
		break;
	}
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

void CScene::OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex)
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
}

void CScene::ReleaseObjects()
{
	for (int j = 0; j < m_nObjects; j++)
		if (m_ppObjects[j]) 
			m_ppObjects[j]->Release();

	if (m_ppObjects) delete[] m_ppObjects;

	for (int j = 0; j < m_nObjectShaders; j++)
	{
		if (m_ppObjectShaders[j]) m_ppObjectShaders[j]->ReleaseObjects();
		if (m_ppObjectShaders[j]) m_ppObjectShaders[j]->Release();
	}
	if (m_ppObjectShaders) delete[] m_ppObjectShaders;

	for (int j = 0; j < m_nInstancingShaders; j++)
	{
		if (m_ppInstancingShaders[j]) m_ppInstancingShaders[j]->ReleaseObjects();
		if (m_ppInstancingShaders[j]) m_ppInstancingShaders[j]->Release();
	}
	if (m_ppInstancingShaders) delete[] m_ppInstancingShaders;
}

void CScene::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
}

void CScene::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS *pLights)
{
}

void CScene::ReleaseShaderVariables()
{
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

CGameObject *CScene::PickObjectPointedByCursor(int xClient, int yClient)
{
	if (!m_pCamera) return(NULL);

	XMMATRIX d3dxmtxView = m_pCamera->GetViewMatrix();
	XMMATRIX d3dxmtxProjection = m_pCamera->GetProjectionMatrix();
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	XMVECTOR d3dxvPickPosition;

	XMFLOAT4 f4vPickPosition;
	XMFLOAT4X4 f4x4mtxProjection;
	
	XMStoreFloat4x4(&f4x4mtxProjection, d3dxmtxProjection);
	f4vPickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / f4x4mtxProjection._11;
	f4vPickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / f4x4mtxProjection._22;
	f4vPickPosition.z = 1.0f;

	d3dxvPickPosition = XMLoadFloat4(&f4vPickPosition);

	int nIntersected = 0;
	float fNearHitDistance = FLT_MAX;
	MESHINTERSECTINFO d3dxIntersectInfo;
	CGameObject *pIntersectedObject = NULL, *pNearestObject = NULL;
	for (int i = 0; i < m_nObjectShaders; i++)
	{
		pIntersectedObject = m_ppObjectShaders[i]->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if (pIntersectedObject && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = pIntersectedObject;
		}
	}
	for (int i = 1; i < m_nObjects; i++)
	{
		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if ((nIntersected > 0) && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = m_ppObjects[i];
		}
	}

	return(pNearestObject);
}

void CScene::UpdateObjects(float fTimeElapsed)
{
	m_pSkyBox->Animate(fTimeElapsed, NULL);
	m_pTerrain->Animate(fTimeElapsed, NULL);

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fTimeElapsed, NULL);
	for (int i = 0; i < m_nInstancingShaders; i++) m_ppInstancingShaders[i]->UpdateObjects(fTimeElapsed);
}

void CScene::OnPreRender(ID3D11DeviceContext *pd3dDeviceContext)
{
}

void CScene::Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pLights && m_pd3dcbLights) UpdateShaderVariable(pd3dDeviceContext, m_pLights);

	if (m_pSkyBox)
		m_pSkyBox->Render(pd3dDeviceContext, pCamera);

	if (m_pTerrain->IsVisible(pCamera)) 
		m_pTerrain->Render(pd3dDeviceContext, pCamera);  //Terrain

	if (m_ppObjects && m_ppObjects[0])
		m_ppObjects[0]->Render(pd3dDeviceContext, pCamera); //Water
	
	for (int i = 0; i < m_nObjectShaders; i++) m_ppObjectShaders[i]->Render(pd3dDeviceContext, pCamera);
	for (int i = 0; i < m_nInstancingShaders; i++) m_ppInstancingShaders[i]->Render(pd3dDeviceContext, pCamera);
}

void CScene::RenderAllText(ID3D11DeviceContext *pd3dDeviceContext)
{
}