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

	m_vecObjectsContainer.clear();
	m_vecInstancedObjectsShaderContainer.clear();

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
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
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
				cout << "RGB Axis Option" << endl;
				GLOBAL_MGR->g_bShowWorldAxis = !GLOBAL_MGR->g_bShowWorldAxis;
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
				GLOBAL_MGR->g_bShowWireFrame = !GLOBAL_MGR->g_bShowWireFrame;
				break;
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
			case '5':
			{
				cout << "G-Buffer Option" << endl;
				GLOBAL_MGR->g_bShowGBuffer= !GLOBAL_MGR->g_bShowGBuffer;
				break;
			}
		
		}
		break;
	case WM_KEYUP:
		switch (wParam) {
			
		}
		break;
	}
	return(false);
}

void CScene::Initialize()
{
	cout << "========================================================================================" << endl;
	cout << "==================================== Scene Loading =====================================" << endl;
}

void CScene::CreatePlayer()
{
	m_pPlayerCharacter = new CTerroristCharacterObject();
	m_pPlayerCharacter->CreateObjectData(m_pd3dDevice);
	m_pPlayerCharacter->CreateAxisObject(m_pd3dDevice);

	m_pPlayer = new CTerrainPlayer(m_pPlayerCharacter);
	m_pPlayer->ChangeCamera(m_pd3dDevice, CameraTag::eThirdPerson, 0.0f);
	m_pCamera = m_pPlayer->GetCamera();
	m_pPlayerCharacter->SetPlayer(m_pPlayer);

	SCENE_MGR->g_pPlayer = m_pPlayer;
	m_pPlayer->SetPosition(XMVectorSet(60.0f, 2.5f, 0.0f, 0.0f));
}

void CScene::ReleaseObjects()
{
	SafeDelete(m_pSkyBox);
	SafeDelete(m_pTerrain);
	
	SafeDelete(m_pWorldCenterAxis);

	for (auto& object : m_vecObjectsContainer)
		SafeDelete(object);

	m_vecObjectsContainer.clear();

	for (auto& instancedShaderObject : m_vecInstancedObjectsShaderContainer) {
		instancedShaderObject->ReleaseObjects();
		SafeDelete(instancedShaderObject);
	}
	m_vecInstancedObjectsShaderContainer.clear();

	CScene::ReleaseConstantBuffers();

	SafeDelete(m_pPlayer);
	SafeDelete(m_pUIManager);

	m_vecShaderObjectContainer.ReleaseObjects();
}

XMFLOAT2 CScene::ObjectPositionConvertToScreen(XMFLOAT3 d3dxvObjPos)
{
	// 임시 보류 - 제대로 실행되지 않는다. 다른 우선순위에서 밀려나서 다음에 다시 구현
	if (!m_pCamera) return(XMFLOAT2(0.0f, 0.0f));

	XMFLOAT2 d3dxvScreenPos;

	float w = 1.0f;

	XMMATRIX mtxView = m_pCamera->GetViewMatrix();
	XMMATRIX mtxProjection = m_pCamera->GetProjectionMatrix();
	XMMATRIX mtxIdentity = XMMatrixIdentity();

	/*
	D3D10_VIEWPORT d3d10ViewPort;

	d3d10ViewPort.Height = d3dViewport.Height;
	d3d10ViewPort.MaxDepth = d3dViewport.MaxDepth;
	d3d10ViewPort.MinDepth = d3dViewport.MinDepth;
	d3d10ViewPort.TopLeftX = d3dViewport.TopLeftX;
	d3d10ViewPort.TopLeftY = d3dViewport.TopLeftY;
	d3d10ViewPort.Width = d3dViewport.Width;

	//프로젝션 행렬, 뷰포트 행렬, 뷰 행렬, 월드변환 행렬(화면 좌표계로 변환이므로 단위행렬 넣어주면 됨)을 모두 곱한다.
	//왜 D3DXVec3Project함수는 D3D10 뷰포트만 지원할까...
	D3DXVec3Project(&d3dxvOut, &d3dxvObjPos, &d3d10ViewPort, &d3dxmtxProjection, &d3dxmtxView, &d3dxmIdentityWorld);
	*/
	XMVECTOR pOutVector;
	D3D11_VIEWPORT d3d11ViewPort = m_pCamera->GetViewport();

	pOutVector = XMVector3Project(XMLoadFloat3(&d3dxvObjPos), d3d11ViewPort.TopLeftX, d3d11ViewPort.TopLeftY, d3d11ViewPort.Width, d3d11ViewPort.Height,
		d3d11ViewPort.MinDepth, d3d11ViewPort.MaxDepth,	mtxProjection, mtxView, mtxIdentity);

	XMStoreFloat2(&d3dxvScreenPos, pOutVector);

	return d3dxvScreenPos;
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
	CollisionInfo d3dxIntersectInfo;
	CGameObject *pIntersectedObject = NULL, *pNearestObject = NULL;


	for (auto& object : m_vecObjectsContainer) {
		nIntersected = object->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if ((nIntersected > 0) && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = object;
		}
	}

	pIntersectedObject = m_vecShaderObjectContainer.PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
	if (pIntersectedObject && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
	{
		fNearHitDistance = d3dxIntersectInfo.m_fDistance;
		pNearestObject = pIntersectedObject;
	}

	for (auto& instancedShaderObject : m_vecInstancedObjectsShaderContainer) {
		pIntersectedObject = instancedShaderObject->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if (pIntersectedObject && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = pIntersectedObject;
		}
	}

	return(pNearestObject);
}

void CScene::UpdateObjects(float fTimeElapsed)
{
	m_fTimeElapsed = fTimeElapsed;

	if(m_pSkyBox) m_pSkyBox->Update(fTimeElapsed);
	if(m_pTerrain) m_pTerrain->Update(fTimeElapsed);
	if(GLOBAL_MGR->g_bShowWorldAxis) m_pWorldCenterAxis->Update(fTimeElapsed);

	for (auto& object : m_vecObjectsContainer) {
		if(object->GetActive())
			object->Update(fTimeElapsed);
	}

	m_vecShaderObjectContainer.UpdateObjects(fTimeElapsed);

	for (auto& instancedShaderObject : m_vecInstancedObjectsShaderContainer)
		instancedShaderObject->UpdateObjects(fTimeElapsed);


	/*
	// Collision
	for (auto& staticMeshObject : m_vecStaticMeshContainer)
		staticMeshObject->Update(fTimeElapsed);
	
	for (auto& dynamicMeshObject : m_vecDynamicMeshContainer)
		dynamicMeshObject->Update(fTimeElapsed);
	*/
}

void CScene::OnPreRender(ID3D11DeviceContext *pd3dDeviceContext)
{
}

void CScene::Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pSkyBox)
		m_pSkyBox->Render(pd3dDeviceContext, pCamera);
	
	// WireFrame Mode
	if (GLOBAL_MGR->g_bShowWireFrame)
		m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pWireframeRS);

	for (auto& object : m_vecObjectsContainer)
		if(object->IsVisible(pCamera))
			object->Render(pd3dDeviceContext, pCamera);

	m_vecShaderObjectContainer.Render(pd3dDeviceContext, pCamera);
	
	for (auto& instancedShaderObject : m_vecInstancedObjectsShaderContainer)
		instancedShaderObject->Render(pd3dDeviceContext, pCamera);	

	if (m_pSphereObject)
		m_pSphereObject->Render(pd3dDeviceContext, pCamera);
}