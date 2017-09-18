//-----------------------------------------------------------------------------
// File: Scene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

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
			/*
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
				*/
			case VK_F5:
			{
				cout << "Show Wire Frame" << endl;
				GLOBAL_MGR->g_bShowWireFrame = !GLOBAL_MGR->g_bShowWireFrame;

				GLOBAL_MGR->g_vRenderOption.y = false;

				D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
				STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Map(GLOBAL_MGR->g_pd3dcbRenderOption, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
				XMFLOAT4 *pcbRenderOption = (XMFLOAT4 *)d3dMappedResource.pData;
				*pcbRenderOption = GLOBAL_MGR->g_vRenderOption;
				STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Unmap(GLOBAL_MGR->g_pd3dcbRenderOption, 0);
			}
				break;
			case VK_F6:
			{
				cout << "BoundingBox Rendering Option" << endl;
				GLOBAL_MGR->g_vRenderOption.y = !GLOBAL_MGR->g_vRenderOption.y;

				D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
				STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Map(GLOBAL_MGR->g_pd3dcbRenderOption, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
				XMFLOAT4 *pcbRenderOption = (XMFLOAT4 *)d3dMappedResource.pData;
				*pcbRenderOption = GLOBAL_MGR->g_vRenderOption;
				STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Unmap(GLOBAL_MGR->g_pd3dcbRenderOption, 0);

				GLOBAL_MGR->g_bShowWireFrame = false;
				break;
			}
			case VK_F7:
			{
				cout << "G-Buffer Option" << endl;
				GLOBAL_MGR->g_bShowGBuffer= !GLOBAL_MGR->g_bShowGBuffer;
				break;
			}
			case VK_F8:
			{
				cout << "Show LightVolume" << endl;
				GLOBAL_MGR->g_bShowLightVolume = !GLOBAL_MGR->g_bShowLightVolume;
				break;
			}
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

void CScene::InitializePhysX()
{
	m_pPxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PxDefaultAllocatorCallback, m_PxDefaultErrorCallback);
	PxTolerancesScale PxScale = PxTolerancesScale();

	m_pPxPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pPxFoundation, PxScale, false);

	//Cooking Init
	PxCookingParams params(PxScale);
	params.meshWeldTolerance = 0.001f;
	params.meshPreprocessParams = PxMeshPreprocessingFlags(PxMeshPreprocessingFlag::eWELD_VERTICES | PxMeshPreprocessingFlag::eREMOVE_UNREFERENCED_VERTICES | PxMeshPreprocessingFlag::eREMOVE_DUPLICATED_TRIANGLES);
	m_pPxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pPxFoundation, params);


	if (FAILED(m_pPxPhysicsSDK == NULL))
	{
		return;
		//MSG_BOX(L"PhysicsSDK Initialize Failed");
	}

	PxInitExtensions(*m_pPxPhysicsSDK);
	PxSceneDesc sceneDesc(m_pPxPhysicsSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);


	if (!sceneDesc.cpuDispatcher)
	{
		PxDefaultCpuDispatcher* pCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = pCpuDispatcher;
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	m_pPxScene = m_pPxPhysicsSDK->createScene(sceneDesc);
	m_pPxControllerManager = PxCreateControllerManager(*m_pPxScene);

	//PxMaterial : 표면 특성 집합을 나타내는 재질 클래스
	m_pPxMaterial = m_pPxPhysicsSDK->createMaterial(0.5f, 0.5f, 0.2f); //1.정지 마찰계수 운동마찰계수, 반발계수


#if defined(DEBUG) || defined(_DEBUG)
	// Physx Visual Debugger
	if (NULL == m_pPxPhysicsSDK->getPvdConnectionManager())
	{
		cout << "PVD Connect Failed" << endl;
		return;
	}
	
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
	m_pPxPhysicsSDK->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
	m_pPxPVDConnection = PxVisualDebuggerExt::createConnection(m_pPxPhysicsSDK->getPvdConnectionManager(), "127.0.0.1", 5425, 1000, connectionFlags);
#endif
}

void CScene::ReleasePhysX()
{
#if defined(DEBUG) || defined(_DEBUG)
	if (m_pPxPVDConnection) m_pPxPVDConnection->release();
#endif
	if (m_pPxControllerManager) m_pPxControllerManager->release();
	if (m_pPxScene) m_pPxScene->release();
	if (m_pPxFoundation) m_pPxFoundation->release();
	if (m_pPxPhysicsSDK) m_pPxPhysicsSDK->release();
	if (m_pPxCooking) m_pPxCooking->release();
}

void CScene::CreatePlayer()
{
	m_pPlayerCharacter = new CTerroristPlayer(TeamType::eRedTeam);
	m_pPlayerCharacter->CreateObjectData(m_pd3dDevice);
	m_pPlayerCharacter->CreateAxisObject(m_pd3dDevice);

	m_pPlayer = new CTerrainPlayer(m_pPlayerCharacter);
	m_pPlayer->ChangeCamera(m_pd3dDevice, CameraTag::eFirstPerson);
//	m_pPlayer->ChangeCamera(m_pd3dDevice, CameraTag::eThirdPerson);
	m_pPlayer->InitializePhysXData(m_pPxPhysicsSDK, m_pPxMaterial, m_pPxControllerManager);

	m_pCamera = m_pPlayer->GetCamera();
	m_pPlayerCharacter->SetPlayer(m_pPlayer);

	SCENE_MGR->g_pPlayerCharacter = m_pPlayerCharacter;
	SCENE_MGR->g_pPlayer = m_pPlayer;
	m_pPlayer->SetPosition(XMFLOAT3(60.0f, 5.0f, 20.0f));
}

void CScene::ReleaseObjects()
{
	SafeDelete(m_pSkyBox);
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

void CScene::Update(float fDeltaTime)
{
	m_fDeltaTime = fDeltaTime;

	if(m_pSkyBox) m_pSkyBox->Update(fDeltaTime);
	if(GLOBAL_MGR->g_bShowWorldAxis) m_pWorldCenterAxis->Update(fDeltaTime);

	for (auto& object : m_vecObjectsContainer) {
		if(object->GetActive())
			object->Update(fDeltaTime);
	}

	m_vecShaderObjectContainer.UpdateObjects(fDeltaTime);

	for (auto& instancedShaderObject : m_vecInstancedObjectsShaderContainer)
		instancedShaderObject->UpdateObjects(fDeltaTime);


	/*
	// Collision
	for (auto& staticMeshObject : m_vecStaticMeshContainer)
		staticMeshObject->Update(fDeltaTime);
	
	for (auto& dynamicMeshObject : m_vecDynamicMeshContainer)
		dynamicMeshObject->Update(fDeltaTime);
	*/
}

void CScene::Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pSkyBox)
		m_pSkyBox->Render(pd3dDeviceContext, pCamera);
	
	// WireFrame Mode
	if (GLOBAL_MGR->g_bShowWireFrame)
		m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pWireframeRS);
	else 
		m_pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);

	for (auto& object : m_vecObjectsContainer)
		if(object->IsVisible(pCamera))
			object->Render(pd3dDeviceContext, pCamera);

	m_vecShaderObjectContainer.Render(pd3dDeviceContext, pCamera);
	
	for (auto& instancedShaderObject : m_vecInstancedObjectsShaderContainer)
		instancedShaderObject->Render(pd3dDeviceContext, pCamera);	

//	if (m_pSphereObject)
//		m_pSphereObject->Render(pd3dDeviceContext, pCamera);
}