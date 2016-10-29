//-----------------------------------------------------------------------------
// File: Scene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
	m_pLights = NULL;

	m_pCamera = NULL;
	m_pSelectedObject = NULL;

	m_pSkyBox = nullptr;
	m_pTerrain = nullptr;

	m_vObjectsVector.clear();
	m_vObjectsShaderVector.clear();
	m_vInstancedObjectsShaderVector.clear();

	m_pParticleSystem = nullptr;
	m_fGametime = 0.0f;

	m_pd3dcbRenderOption = nullptr;
	m_bFogEnable = true;
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
	for (auto& object : m_vObjectsVector) {
		object->Release();
		object = nullptr;
	}
	m_vObjectsVector.clear();

	for (auto& shaderObject : m_vObjectsShaderVector) {
		shaderObject->ReleaseObjects();
		shaderObject->Release();
	}
	m_vObjectsShaderVector.clear();

	for (auto& instancedShaderObject : m_vInstancedObjectsShaderVector) {
		instancedShaderObject->ReleaseObjects();
		instancedShaderObject->Release();
	}
	m_vInstancedObjectsShaderVector.clear();

	ReleaseCOM(m_pd3dcbRenderOption);
}

void CScene::CreateConstantBuffers(ID3D11Device *pd3dDevice)
{
}

void CScene::UpdateConstantBuffers(ID3D11DeviceContext *pd3dDeviceContext)
{
}

void CScene::ReleaseConstantBuffers()
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


	for (auto shaderObject : m_vObjectsShaderVector) {
		pIntersectedObject = shaderObject->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if (pIntersectedObject && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = pIntersectedObject;
		}
	}

	for (auto instancedShaderObject : m_vInstancedObjectsShaderVector) {
		pIntersectedObject = instancedShaderObject->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if (pIntersectedObject && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = pIntersectedObject;
		}
	}

	for (auto& object : m_vObjectsVector) {
		nIntersected = object->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if ((nIntersected > 0) && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = object;
		}
	}
	



	/*
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
	*/
	return(pNearestObject);
}

void CScene::UpdateObjects(float fTimeElapsed)
{
	UpdateConstantBuffers(STATEOBJ_MGR->m_pd3dImmediateDeviceContext.Get());

	m_pSkyBox->Animate(fTimeElapsed, NULL);
	m_pTerrain->Animate(fTimeElapsed, NULL);

	for (auto object : m_vObjectsVector)
		object->Update(nullptr);		// 오류 발생 확인하기

	for (auto shaderObject : m_vObjectsShaderVector)
		shaderObject->UpdateObjects(fTimeElapsed);

	for (auto instancedShaderObject : m_vInstancedObjectsShaderVector)
		instancedShaderObject->UpdateObjects(fTimeElapsed);
}

void CScene::OnPreRender(ID3D11DeviceContext *pd3dDeviceContext)
{
}

void CScene::Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pSkyBox)
		m_pSkyBox->Render(pd3dDeviceContext, pCamera);

	if (m_pTerrain->IsVisible(pCamera)) 
		m_pTerrain->Render(pd3dDeviceContext, pCamera);

	for(auto object : m_vObjectsVector)
		object->Render(pd3dDeviceContext, pCamera);

	for (auto shaderObject : m_vObjectsShaderVector)
		shaderObject->Render(pd3dDeviceContext, pCamera);

	for (auto instancedShaderObject : m_vInstancedObjectsShaderVector)
		instancedShaderObject->Render(pd3dDeviceContext, pCamera);
}

void CScene::RenderAllText(ID3D11DeviceContext *pd3dDeviceContext)
{
}