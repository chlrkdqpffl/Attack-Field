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
}

CScene::~CScene()
{
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
//		m_pSelectedObject = PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam));
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
	switch (nMessageID) {
	case WM_KEYDOWN:
		switch (wParam) {
		case '1':
		{
			cout << "RGB Axis Option" << endl;
			GLOBAL_MGR->g_bShowWorldAxis = !GLOBAL_MGR->g_bShowWorldAxis;
		}
		break;
		case '2':
		{
			cout << "Fog Option" << endl;
			GLOBAL_MGR->g_vRenderOption.x = !GLOBAL_MGR->g_vRenderOption.x;
		
			D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
			STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Map(GLOBAL_MGR->g_pd3dcbRenderOption, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
			XMFLOAT4 *pcbRenderOption = (XMFLOAT4 *)d3dMappedResource.pData;
			*pcbRenderOption = GLOBAL_MGR->g_vRenderOption;
			STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Unmap(GLOBAL_MGR->g_pd3dcbRenderOption, 0);
			break;
		}
		case '3':
		{
			//	cout << "ºó ¼Ó¼º" << endl;

			break;
		}
		case '4':
		{	
			cout << "BoundingBox Rendering Option" << endl;
			GLOBAL_MGR->g_vRenderOption.y = !GLOBAL_MGR->g_vRenderOption.y;

			D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
			STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Map(GLOBAL_MGR->g_pd3dcbRenderOption, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
			XMFLOAT4 *pcbRenderOption = (XMFLOAT4 *)d3dMappedResource.pData;
			*pcbRenderOption = GLOBAL_MGR->g_vRenderOption;
			STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Unmap(GLOBAL_MGR->g_pd3dcbRenderOption, 0);

			break;
		}
		}
		break;
	case WM_KEYUP:
		switch (wParam) {
		case VK_A:

			break;
		}
		break;
	}
	return(false);
}

void CScene::OnChangeSkyBoxTextures(ID3D11Device *pd3dDevice, CMaterial *pMaterial, int nIndex)
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	cout << "========================================================================================" << endl;
	cout << "==================================== Scene Loading =====================================" << endl;

	CScene::CreateConstantBuffers(pd3dDevice);
	CScene::CreateTweakBars();

	m_pWorldCenterAxis = new CAxisObjects();
	m_pWorldCenterAxis->CreateAxis(pd3dDevice);
}

void CScene::CreateTweakBars()
{
}

void CScene::ReleaseObjects()
{
	m_pSkyBox->Release();
	m_pTerrain->Release();

	SafeDelete(m_pWorldCenterAxis);

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

	CScene::ReleaseConstantBuffers();
}

void CScene::CreateConstantBuffers(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT4);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
	CScene::UpdateConstantBuffers(STATEOBJ_MGR->g_pd3dImmediateDeviceContext);
	
	if(m_pSkyBox) m_pSkyBox->Update(fTimeElapsed, NULL);
	if(m_pTerrain) m_pTerrain->Update(fTimeElapsed, NULL);
	if(GLOBAL_MGR->g_bShowWorldAxis) m_pWorldCenterAxis->Update(fTimeElapsed);

	for (auto object : m_vObjectsVector)
		object->Update(fTimeElapsed);

	/*
	for (auto shaderObject : m_vObjectsShaderVector)
		shaderObject->UpdateObjects(fTimeElapsed);

	for (auto instancedShaderObject : m_vInstancedObjectsShaderVector)
		instancedShaderObject->UpdateObjects(fTimeElapsed);
		*/
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

	if (GLOBAL_MGR->g_bShowWorldAxis) m_pWorldCenterAxis->Render(pd3dDeviceContext, pCamera);
	
	for (auto object : m_vObjectsVector) {
	//	if (object->IsVisible(pCamera))
			object->Render(pd3dDeviceContext, pCamera);
	}

	for (auto shaderObject : m_vObjectsShaderVector)
		shaderObject->Render(pd3dDeviceContext, pCamera);
	
	for (auto instancedShaderObject : m_vInstancedObjectsShaderVector)
		instancedShaderObject->Render(pd3dDeviceContext, pCamera);	
		
}

void CScene::RenderAllText(ID3D11DeviceContext *pd3dDeviceContext)
{
	string str;
	
	// Draw FrameRate
	int fps = (int)SCENE_MGR->fFrameRate;
	str = to_string(fps) + " FPS";

	if (60 <= fps)
		TEXT_MGR->RenderText(pd3dDeviceContext, str, 40, 1500, 20, 0xFFFFFFFF, FW1_LEFT);
	else if(30 <= fps && fps < 60)
		TEXT_MGR->RenderText(pd3dDeviceContext, str, 40, 1500, 20, 0xFF1270FF, FW1_LEFT);
	else if (fps < 30)
		TEXT_MGR->RenderText(pd3dDeviceContext, str, 40, 1500, 20, 0xFF0000FF, FW1_LEFT);
}