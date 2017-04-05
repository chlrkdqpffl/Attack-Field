//-----------------------------------------------------------------------------
// File: CGameObject.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Object.h"
#include "Shader.h"
#include "BoundingBoxMesh.h"
#include "BoundingBoxShader.h"
#include "AxisObjects.h"

///////////////////////////////////////////////////////////////////////////

ID3D11Buffer*	CGameObject::m_pd3dcbWorldMatrix = NULL;
ID3D11Buffer*	CGameObject::m_pd3dcbMaterialColors = NULL;
UINT			CGameObject::m_iObjectId = 0;

CGameObject::CGameObject(int nMeshes)
{
	m_nMeshes = nMeshes;
	if (m_nMeshes > 0) m_ppMeshes = new CMesh*[m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++)	m_ppMeshes[i] = NULL;

	m_bcMeshBoundingBox.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bcMeshBoundingBox.Extents = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_iObjectId++;
}

CGameObject::~CGameObject()
{
	if (m_pd3dDepthStencilState) m_pd3dDepthStencilState->Release();
	if (m_pd3dBlendState) m_pd3dBlendState->Release();

	if (m_pShader) m_pShader->Release();
	if (m_pMaterial) m_pMaterial->Release();

	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Release();
			m_ppMeshes[i] = NULL;
		}
		delete[] m_ppMeshes;
	}

	if (m_pSibling) delete m_pSibling;
	if (m_pChild) delete m_pChild;

	SafeDelete(m_pBoundingBoxMesh);
	SafeDelete(m_pBoundingBoxShader);
	SafeDelete(m_pAxisObject);
}

void CGameObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CreateMesh(pd3dDevice);
	CreateShader(pd3dDevice);
	CreateMaterial();
//	CreateBoundingBox(pd3dDevice);
}

void CGameObject::CreateBoundingBox(ID3D11Device *pd3dDevice)
{
	m_pBoundingBoxMesh = new CBoundingBoxMesh(pd3dDevice, m_bcMeshBoundingBox);

	m_pBoundingBoxShader = new CBoundingBoxShader();
	m_pBoundingBoxShader->CreateShader(pd3dDevice);
}

void CGameObject::CreateAxisObject(ID3D11Device *pd3dDevice)
{
	m_pAxisObject = new CAxisObjects(this);
	m_pAxisObject->CreateAxis(pd3dDevice);
}

void CGameObject::SetMesh(CMesh *pMesh, int nIndex)
{
	if (nIndex >= m_nMeshes)
	{
		CMesh **ppMeshes = new CMesh*[nIndex + 1];
		if (m_ppMeshes)
		{
			for (int i = 0; i < m_nMeshes; i++) ppMeshes[i] = m_ppMeshes[i];
			delete[] m_ppMeshes;
		}
		ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
		m_nMeshes = nIndex + 1;
		m_ppMeshes = ppMeshes;
	}
	else {
		if (m_ppMeshes)
		{
			if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
			m_ppMeshes[nIndex] = pMesh;
			if (pMesh) pMesh->AddRef();
		}
	}

	if (pMesh) {
		/*
		XMFLOAT3 xmMin, xmMax; // 기존값
		XMFLOAT3 input_xmMin, input_xmMax;		//새로 들어온거
		XMFLOAT3 fCenter, fExtern;

		fCenter = pMesh->GetBoundingCube().Center; 
		fExtern = pMesh->GetBoundingCube().Extents;

		XMStoreFloat3(&input_xmMin, XMVectorSubtract(XMLoadFloat3(&fCenter), XMLoadFloat3(&fExtern)));
		XMStoreFloat3(&input_xmMax, XMVectorAdd(XMLoadFloat3(&fCenter), XMLoadFloat3(&fExtern)));

		XMStoreFloat3(&xmMin, XMVectorSubtract(XMLoadFloat3(&m_bcMeshBoundingBox.Center), XMLoadFloat3(&m_bcMeshBoundingBox.Extents)));
		XMStoreFloat3(&xmMax, XMVectorAdd(XMLoadFloat3(&m_bcMeshBoundingBox.Center), XMLoadFloat3(&m_bcMeshBoundingBox.Extents)));
		
		if (input_xmMin.x < xmMin.x) xmMin.x = input_xmMin.x;
		if (input_xmMin.y < xmMin.y) xmMin.y = input_xmMin.y;
		if (input_xmMin.z < xmMin.z) xmMin.z = input_xmMin.z;
		if (input_xmMax.x > xmMax.x) xmMax.x = input_xmMax.x;
		if (input_xmMax.y > xmMax.y) xmMax.y = input_xmMax.y;
		if (input_xmMax.z > xmMax.z) xmMax.z = input_xmMax.z;
		
		XMStoreFloat3(&m_bcBoundingBox.Center, XMVectorSubtract(XMLoadFloat3(&input_xmMax), XMLoadFloat3(&input_xmMin)) * 0.5f);
		XMStoreFloat3(&m_bcBoundingBox.Extents, XMVectorAdd(XMLoadFloat3(&input_xmMax), XMLoadFloat3(&input_xmMin)) * 0.5f);
		*/
		
		
		BoundingBox::CreateMerged(m_bcMeshBoundingBox, m_bcMeshBoundingBox, pMesh->GetBoundingCube());
	}
}

void CGameObject::SetChild(CGameObject *pChild)
{
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}
	if (pChild) pChild->m_pParent = this;
}

void CGameObject::SetShader(CShader *pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

void CGameObject::SetMaterial(CMaterial *pMaterial)
{
	if (m_pMaterial) m_pMaterial->Release();
	m_pMaterial = pMaterial;
	if (m_pMaterial) m_pMaterial->AddRef();
}

void CGameObject::SetShadowMatrix(XMVECTOR d3dxvLight, XMVECTOR d3dxPlane)
{
	m_mtxShadow = XMMatrixShadow(d3dxPlane, d3dxvLight);
}

void CGameObject::GenerateRayForPicking(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxWorld, XMMATRIX *pd3dxmtxView, XMVECTOR *pd3dxvPickRayPosition, XMVECTOR *pd3dxvPickRayDirection)
{
	XMMATRIX d3dxmtxInverse;
	XMMATRIX d3dxmtxWorldView = *pd3dxmtxView;
	if (pd3dxmtxWorld) d3dxmtxWorldView = XMMatrixMultiply(*pd3dxmtxWorld, *pd3dxmtxView);
	d3dxmtxInverse = XMMatrixInverse(NULL, d3dxmtxWorldView);
	/*
	pd3dxvPickRayDirection->x = pd3dxvPickPosition->x * d3dxmtxInverse._11 + pd3dxvPickPosition->y * d3dxmtxInverse._21 + pd3dxvPickPosition->z * d3dxmtxInverse._31;
	pd3dxvPickRayDirection->y = pd3dxvPickPosition->x * d3dxmtxInverse._12 + pd3dxvPickPosition->y * d3dxmtxInverse._22 + pd3dxvPickPosition->z * d3dxmtxInverse._32;
	pd3dxvPickRayDirection->z = pd3dxvPickPosition->x * d3dxmtxInverse._13 + pd3dxvPickPosition->y * d3dxmtxInverse._23 + pd3dxvPickPosition->z * d3dxmtxInverse._33;
	pd3dxvPickRayPosition->x = d3dxmtxInverse._41;
	pd3dxvPickRayPosition->y = d3dxmtxInverse._42;
	pd3dxvPickRayPosition->z = d3dxmtxInverse._43;
	*/
	XMVECTOR d3dxvCameraOrigin = XMVectorZero();
	*pd3dxvPickRayPosition = XMVector3TransformCoord(d3dxvCameraOrigin, d3dxmtxInverse);
	*pd3dxvPickRayDirection = XMVector3TransformCoord(*pd3dxvPickPosition, d3dxmtxInverse);
	*pd3dxvPickRayDirection = *pd3dxvPickRayDirection - *pd3dxvPickRayPosition;
}

int CGameObject::PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo)
{
	XMVECTOR d3dxvPickRayPosition, d3dxvPickRayDirection;
	int nIntersected = 0;
	if (m_bActive && m_bIsVisible && m_ppMeshes)
	{
		GenerateRayForPicking(pd3dxvPickPosition, &m_mtxWorld, pd3dxmtxView, &d3dxvPickRayPosition, &d3dxvPickRayDirection);
		for (int i = 0; i < m_nMeshes; i++)
		{
			nIntersected = m_ppMeshes[i]->CheckRayIntersection(&d3dxvPickRayPosition, &d3dxvPickRayDirection, pd3dxIntersectInfo);
			if (nIntersected > 0) break;
		}
	}
	return(nIntersected);
}

void CGameObject::SetPosition(float x, float y, float z, bool isLocal)
{
	XMFLOAT4X4 mtx; 
	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._41 = x;  mtx._42 = y; 	mtx._43 = z;
		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._41 = x;  mtx._42 = y; 	mtx._43 = z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::SetPosition(XMVECTOR d3dxvPosition, bool isLocal)
{
	XMFLOAT4 f4vPosition;
	XMStoreFloat4(&f4vPosition, d3dxvPosition);
	SetPosition(f4vPosition.x, f4vPosition.y, f4vPosition.z, isLocal);
}

void CGameObject::SetPosition(XMFLOAT3 pos, bool isLocal)
{
	XMFLOAT4X4 mtx;
	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._41 = pos.x; mtx._42 = pos.y; mtx._43 = pos.z;
		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._41 = pos.x; mtx._42 = pos.y; mtx._43 = pos.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMVECTOR d3dxvPosition = GetvPosition();
	XMVECTOR d3dxvRight = GetRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveUp(float fDistance)
{
	XMVECTOR d3dxvPosition = GetvPosition();
	XMVECTOR d3dxvUp = GetUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveForward(float fDistance)
{
	XMVECTOR d3dxvPosition = GetvPosition();
	XMVECTOR d3dxvLookAt = GetLook();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::Move(XMFLOAT3 vPos, bool isLocal)
{
	if (isLocal) {
		XMVECTOR d3dxvPosition = GetvPosition(true) + XMLoadFloat3(&vPos);
		CGameObject::SetPosition(d3dxvPosition, true);
	}
	else {
		XMVECTOR d3dxvPosition = GetvPosition() + XMLoadFloat3(&vPos);
		CGameObject::SetPosition(d3dxvPosition);
	}
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll, bool isLocal)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), 
		XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));

	if (isLocal)
		m_mtxLocal = mtxRotate * m_mtxLocal;
	else
		m_mtxWorld = mtxRotate * m_mtxWorld;
}

void CGameObject::Rotate(XMFLOAT3 fAngle, bool isLocal)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fAngle.x),
		XMConvertToRadians(fAngle.y), XMConvertToRadians(fAngle.z));

	if (isLocal)
		m_mtxLocal = mtxRotate * m_mtxLocal;
	else
		m_mtxWorld = mtxRotate * m_mtxWorld;
}

void CGameObject::Rotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationAxis(*pd3dxvAxis, XMConvertToRadians(fAngle));

	if (isLocal)
		m_mtxLocal = mtxRotate * m_mtxLocal;
	else
		m_mtxWorld = mtxRotate * m_mtxWorld;
}

void CGameObject::SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch),
		XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));

	if (isLocal)
		m_mtxLocal = mtxRotate;
	else
		m_mtxWorld = mtxRotate;
}

void CGameObject::SetRotate(XMFLOAT3 fAngle, bool isLocal)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fAngle.x),
		XMConvertToRadians(fAngle.y), XMConvertToRadians(fAngle.z));

	if (isLocal)
		m_mtxLocal = mtxRotate;
	else
		m_mtxWorld = mtxRotate;
}

void CGameObject::SetRotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationAxis(*pd3dxvAxis, XMConvertToRadians(fAngle));

	if (isLocal)
		m_mtxLocal = mtxRotate;
	else
		m_mtxWorld = mtxRotate;
}

XMVECTOR CGameObject::GetvPosition(bool isLocal) const
{
	XMFLOAT4X4 mtx;
	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else 
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	return XMVectorSet(mtx._41, mtx._42, mtx._43, 0.f);
}

XMFLOAT3 CGameObject::GetPosition(bool isLocal) const
{
	XMFLOAT4X4 mtx;
	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	return XMFLOAT3(mtx._41, mtx._42, mtx._43);
}

XMVECTOR CGameObject::GetRight(bool isLocal) const
{
	XMFLOAT4X4 mtx;
	
	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	XMVECTOR d3dxvRight = XMVectorSet(mtx._11, mtx._12, mtx._13, 0.f);
	d3dxvRight = XMVector3NormalizeEst(d3dxvRight);

	return d3dxvRight;
}

XMVECTOR CGameObject::GetUp(bool isLocal) const
{
	XMFLOAT4X4 mtx;

	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	XMVECTOR d3dxvUp = XMVectorSet(mtx._21, mtx._22, mtx._23, 0.f);
	d3dxvUp = XMVector3Normalize(d3dxvUp);

	return d3dxvUp;
}

XMVECTOR CGameObject::GetLook(bool isLocal) const
{
	XMFLOAT4X4 mtx;

	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	XMVECTOR d3dxvLookAt = XMVectorSet(mtx._31, mtx._32, mtx._33, 0.f);
	d3dxvLookAt = XMVector3Normalize(d3dxvLookAt);

	return d3dxvLookAt;
}

BoundingBox CGameObject::GetBoundingBox(bool isLocal)
{
	if (isLocal) 
		return m_bcMeshBoundingBox;
	else {
		BoundingBox bbox = m_bcMeshBoundingBox;
		XMFLOAT3 world = GetPosition();

		bbox.Center.x += world.x;
		bbox.Center.y += world.y;
		bbox.Center.z += world.z;

		return bbox;
	}
}

void CGameObject::SetRight(XMFLOAT3 axis, bool isLocal)
{
	XMFLOAT4X4 mtx;

	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._11 = axis.x, mtx._12 = axis.y, mtx._13 = axis.z;

		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._11 = axis.x, mtx._12 = axis.y, mtx._13 = axis.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::SetUp(XMFLOAT3 axis, bool isLocal)
{
	XMFLOAT4X4 mtx;

	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._21 = axis.x, mtx._22 = axis.y, mtx._23 = axis.z;

		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._21 = axis.x, mtx._22 = axis.y, mtx._23 = axis.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::SetLook(XMFLOAT3 axis, bool isLocal)
{
	XMFLOAT4X4 mtx;
	
	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._31 = axis.x, mtx._32 = axis.y, mtx._33 = axis.z;

		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._31 = axis.x, mtx._32 = axis.y, mtx._33 = axis.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::CreateConstantBuffers(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbWorldMatrix);

	d3dBufferDesc.ByteWidth = sizeof(XMVECTOR) * 4;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterialColors);

	DXUT_SetDebugName(m_pd3dcbWorldMatrix, "WorldMatrix");
	DXUT_SetDebugName(m_pd3dcbMaterialColors, "MaterialColors");
}

void CGameObject::ReleaseConstantBuffers()
{
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
	if (m_pd3dcbMaterialColors) m_pd3dcbMaterialColors->Release();
}

void CGameObject::UpdateConstantBuffer_WorldMtx(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxWorld)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	pcbWorldMatrix->m_mtxWorld = XMMatrixTranspose(*pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}

void CGameObject::UpdateConstantBuffer_Material(ID3D11DeviceContext *pd3dDeviceContext, CMaterialColors *pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterialColors, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMFLOAT4 *pcbMaterial = (XMFLOAT4 *)d3dMappedResource.pData;
	memcpy(pcbMaterial, &pMaterial->m_d3dxcAmbient, sizeof(XMFLOAT4));
	memcpy(pcbMaterial + 1, &pMaterial->m_d3dxcDiffuse, sizeof(XMFLOAT4));
	memcpy(pcbMaterial + 2, &pMaterial->m_d3dxcSpecular, sizeof(XMFLOAT4));
	memcpy(pcbMaterial + 3, &pMaterial->m_d3dxcEmissive, sizeof(XMFLOAT4));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterialColors, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_MATERIAL, 1, &m_pd3dcbMaterialColors);
}

bool CGameObject::IsVisible(CCamera *pCamera)
{
//	OnPrepareRender();
//	Update(NULL);

	m_bIsVisible = (m_bActive) ? true : false;
#ifdef _WITH_FRUSTUM_CULLING_BY_OBJECT
	if (m_bActive) {
		BoundingBox bcBoundingCube;
		m_bcMeshBoundingBox.Transform(bcBoundingCube, m_mtxWorld);
		if (pCamera) m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);

		if (m_pSibling)	m_bIsVisible |= m_pSibling->IsVisible(pCamera);
		if (m_pChild) m_bIsVisible |= m_pChild->IsVisible(pCamera);
	}
#endif

	return(m_bIsVisible);
}

bool CGameObject::IsCollision(CGameObject* pObject)
{
	BoundingBox pTargetBBox = pObject->GetBoundingBox();
	BoundingBox pSourceBBox = GetBoundingBox();

	if (pSourceBBox.Intersects(pTargetBBox))
		return true;
	else
		return false;
}

void CGameObject::Update(float fTimeElapsed)
{
	if (m_pSibling) 
		m_pSibling->Update(fTimeElapsed);
	if (m_pChild) 
		m_pChild->Update(fTimeElapsed);

	if (m_pAxisObject) {
		if (GLOBAL_MGR->g_bShowWorldAxis)
			m_pAxisObject->Update(fTimeElapsed);
	}

	if (!XMMatrixIsIdentity(m_mtxLocal))
		m_mtxWorld = XMMatrixMultiply(m_mtxLocal, m_mtxWorld);
}

void CGameObject::RenderMesh(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if (m_ppMeshes) {
		for (int i = 0; i < m_nMeshes; i++)	{
			if (m_ppMeshes[i]) {
				bool bIsVisible = true;
				#ifdef _WITH_FRUSTUM_CULLING_BY_SUBMESH
					if (pCamera) {
					#if _AABB_
						AABB bcBoundingCube = m_ppMeshes[i]->GetBoundingCube();
						bcBoundingCube.Update(&XMLoadFloat4x4(&m_d3dxmtxWorld));
						bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
					#else
						BoundingBox bcBoundingCube = m_ppMeshes[i]->GetBoundingCube();
						bcBoundingCube.Transform(bcBoundingCube, XMLoadFloat4x4(&m_d3dxmtxWorld));
						m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
					#endif
				}
				#endif
				if (bIsVisible) m_ppMeshes[i]->Render(pd3dDeviceContext);
			}
		}
	}
}

void CGameObject::OnPrepareRender()
{
}

void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender();
	//	Update(NULL);
	
	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);
	if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pd3dDeviceContext);

	CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &m_mtxWorld);

	//	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 0);
	if (m_pd3dBlendState) pd3dDeviceContext->OMSetBlendState(m_pd3dBlendState, NULL, 0xffffffff);

	RenderMesh(pd3dDeviceContext, pCamera);

	if (m_pSibling) m_pSibling->Render(pd3dDeviceContext, pCamera);
	if (m_pChild) m_pChild->Render(pd3dDeviceContext, pCamera);

	if (GLOBAL_MGR->g_vRenderOption.y)
		BoundingBoxRender(pd3dDeviceContext);

	if (m_pAxisObject) {
		if (GLOBAL_MGR->g_bShowWorldAxis)
			m_pAxisObject->Render(pd3dDeviceContext, pCamera);
	}

	if (m_pShader) m_pShader->OnPostRender(pd3dDeviceContext);
	if (m_pd3dDepthStencilState) pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
	if (m_pd3dBlendState) pd3dDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void CGameObject::BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pBoundingBoxMesh) {
		pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pWireframeRS);
	
		m_pBoundingBoxShader->OnPrepareSetting(pd3dDeviceContext, m_bIsCollision);
		m_pBoundingBoxMesh->Render(pd3dDeviceContext);

		pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);
	}
}