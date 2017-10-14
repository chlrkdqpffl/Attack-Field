#include "stdafx.h"
#include "Player.h"
#include "Camera.h"

ID3D11Buffer *CCamera::m_pd3dcbCamera = NULL;
ID3D11Buffer *CCamera::m_pd3dcbCameraPosition = NULL;

CCamera::CCamera(CCamera *pCamera)
{
	if (pCamera)
	{
		m_vPosition = pCamera->GetPosition();
		m_vRight = pCamera->GetRight();
		m_vUp = pCamera->GetUp();
		m_vLook = pCamera->GetLook();
		XMStoreFloat4x4(&m_mtxView, pCamera->GetViewMatrix());
		XMStoreFloat4x4(&m_mtxProjection, pCamera->GetProjectionMatrix());
		m_d3dViewport = pCamera->GetViewport();
		m_vOffset = pCamera->GetOffset();
		m_fTimeLag = pCamera->GetTimeLag();
		m_pPlayer = pCamera->GetPlayer();
	}
	else
	{
		XMStoreFloat3(&m_vPosition, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		XMStoreFloat3(&m_vRight, XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
		XMStoreFloat3(&m_vUp, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		XMStoreFloat3(&m_vLook, XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
		m_fTimeLag = 0.0f;
		XMStoreFloat3(&m_vOffset, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		m_pPlayer = NULL;
		XMStoreFloat4x4(&m_mtxView, XMMatrixIdentity());
		XMStoreFloat4x4(&m_mtxProjection, XMMatrixIdentity());
	}
}

CCamera::~CCamera()
{
}

void CCamera::SetViewport(ID3D11DeviceContext *pd3dDeviceContext, DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = float(xTopLeft);
	m_d3dViewport.TopLeftY = float(yTopLeft);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
	if (pd3dDeviceContext) pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

void CCamera::SetViewport(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (pd3dDeviceContext) pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

void CCamera::SetLookAt(XMVECTOR& d3dxvPosition, XMVECTOR& d3dxvLookAt, XMVECTOR& d3dxvUp)
{
	XMFLOAT4X4 mtxLookAt;
	XMStoreFloat4x4(&mtxLookAt, XMMatrixLookToLH(d3dxvPosition, d3dxvLookAt, d3dxvUp));

	XMStoreFloat3(&m_vRight, XMVectorSet(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31, 0.0f));
	XMStoreFloat3(&m_vUp, XMVectorSet(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32, 0.0f));
	XMStoreFloat3(&m_vLook, XMVectorSet(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33, 0.0f));
}

void CCamera::SetLookAt(XMVECTOR& d3dxvLookAt)
{
	XMFLOAT4X4 mtxLookAt;
	XMStoreFloat4x4(&mtxLookAt, XMMatrixLookAtLH(XMLoadFloat3(&m_vPosition), d3dxvLookAt, m_pPlayer->GetvUp()));

	XMStoreFloat3(&m_vRight, XMVectorSet(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31, 0.0f));
	XMStoreFloat3(&m_vUp, XMVectorSet(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32, 0.0f));
	XMStoreFloat3(&m_vLook, XMVectorSet(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33, 0.0f));
}

void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	m_fNearPlane = fNearPlaneDistance;
	m_fFarPlane = fFarPlaneDistance;
	m_fFOVAngle = fFOVAngle;

	XMStoreFloat4x4(&m_mtxProjection, 
		XMMatrixPerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance));
}

void CCamera::GenerateViewMatrix()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(XMLoadFloat3(&m_vPosition), m_pPlayer->GetvPosition(), XMLoadFloat3(&m_vUp)));
}

void CCamera::RegenerateViewMatrix()
{
	XMStoreFloat3(&m_vLook, XMVector3Normalize(XMLoadFloat3(&m_vLook)));
	XMStoreFloat3(&m_vRight, XMVector3Cross(XMLoadFloat3(&m_vUp), XMLoadFloat3(&m_vLook)));
	XMStoreFloat3(&m_vRight, XMVector3Normalize(XMLoadFloat3(&m_vRight)));
	XMStoreFloat3(&m_vUp, XMVector3Cross(XMLoadFloat3(&m_vLook), XMLoadFloat3(&m_vRight)));
	XMStoreFloat3(&m_vUp, XMVector3Normalize(XMLoadFloat3(&m_vUp)));
	m_mtxView._11 = m_vRight.x; m_mtxView._12 = m_vUp.x; m_mtxView._13 = m_vLook.x;
	m_mtxView._21 = m_vRight.y; m_mtxView._22 = m_vUp.y; m_mtxView._23 = m_vLook.y;
	m_mtxView._31 = m_vRight.z; m_mtxView._32 = m_vUp.z; m_mtxView._33 = m_vLook.z;
	m_mtxView._41 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_vPosition), XMLoadFloat3(&m_vRight)));
	m_mtxView._42 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_vPosition), XMLoadFloat3(&m_vUp)));
	m_mtxView._43 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_vPosition), XMLoadFloat3(&m_vLook)));

	CalculateFrustumPlanes();
}

void CCamera::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_CAMERA);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbCamera);
	DXUT_SetDebugName(m_pd3dcbCamera, "ViewProjection");

	bd.ByteWidth = sizeof(XMFLOAT4);
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbCameraPosition);
	DXUT_SetDebugName(m_pd3dcbCameraPosition, "CameraPosition");
}

void CCamera::ReleaseShaderVariables()
{
	ReleaseCOM(m_pd3dcbCamera);
	ReleaseCOM(m_pd3dcbCameraPosition);
}

void CCamera::UpdateConstantBuffer_ViewProjection(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxView, XMMATRIX *pd3dxmtxProjection)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_CAMERA *pcbViewProjection = (VS_CB_CAMERA *)d3dMappedResource.pData;
	XMStoreFloat4x4(&pcbViewProjection->m_mtxView, XMMatrixTranspose(*pd3dxmtxView));
	XMStoreFloat4x4(&pcbViewProjection->m_mtxProjection, XMMatrixTranspose(*pd3dxmtxProjection));
	pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);

	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_CAMERA_VIEW_PROJECTION, 1, &m_pd3dcbCamera);
	pd3dDeviceContext->GSSetConstantBuffers(GS_CB_SLOT_CAMERA_VIEW_PROJECTION, 1, &m_pd3dcbCamera);
	pd3dDeviceContext->DSSetConstantBuffers(DS_CB_SLOT_CAMERA_VIEW_PROJECTION, 1, &m_pd3dcbCamera);
#ifdef _WITH_GEOMETRY_SHADER_SHADOW
	pd3dDeviceContext->GSSetConstantBuffers(GS_CB_SLOT_CAMERA_VIEW_PROJECTION, 1, &m_pd3dcbCamera);
#endif
}

void CCamera::UpdateConstantBuffer_CameraPos(ID3D11DeviceContext *pd3dDeviceContext, XMVECTOR* pf3CameraPosition)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbCameraPosition, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMFLOAT4 *pcbCameraPosition = (XMFLOAT4 *)d3dMappedResource.pData;
	XMStoreFloat4(pcbCameraPosition, *pf3CameraPosition);
	pd3dDeviceContext->Unmap(m_pd3dcbCameraPosition, 0);

//	pd3dDeviceContext->VSSetConstantBuffers(CB_SLOT_CAMERA_POSITION, 1, &m_pd3dcbCameraPosition);
	pd3dDeviceContext->HSSetConstantBuffers(CB_SLOT_CAMERA_POSITION, 1, &m_pd3dcbCameraPosition);
	pd3dDeviceContext->DSSetConstantBuffers(CB_SLOT_CAMERA_POSITION, 1, &m_pd3dcbCameraPosition);
	pd3dDeviceContext->GSSetConstantBuffers(CB_SLOT_CAMERA_POSITION, 1, &m_pd3dcbCameraPosition);
	pd3dDeviceContext->PSSetConstantBuffers(CB_SLOT_CAMERA_POSITION, 1, &m_pd3dcbCameraPosition);
}

void CCamera::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	UpdateConstantBuffer_ViewProjection(pd3dDeviceContext, &XMLoadFloat4x4(&m_mtxView), &XMLoadFloat4x4(&m_mtxProjection));

	if (!XMVector3Equal(XMLoadFloat3(&m_vPrevPosition), XMLoadFloat3(&m_vPosition)))
		UpdateConstantBuffer_CameraPos(pd3dDeviceContext, &XMLoadFloat3(&m_vPosition));
}

void CCamera::CalculateFrustumPlanes()
{
	XMMATRIX mtxViewProjection = XMLoadFloat4x4(&m_mtxView) * XMLoadFloat4x4(&m_mtxProjection);

	XMFLOAT4X4 mtx;
	XMStoreFloat4x4(&mtx, mtxViewProjection);

	m_pd3dxFrustumPlanes[0].x = -(mtx._14 + mtx._11);
	m_pd3dxFrustumPlanes[0].y = -(mtx._24 + mtx._21);
	m_pd3dxFrustumPlanes[0].z = -(mtx._34 + mtx._31);
	m_pd3dxFrustumPlanes[0].w = -(mtx._44 + mtx._41);

	m_pd3dxFrustumPlanes[1].x = -(mtx._14 - mtx._11);
	m_pd3dxFrustumPlanes[1].y = -(mtx._24 - mtx._21);
	m_pd3dxFrustumPlanes[1].z = -(mtx._34 - mtx._31);
	m_pd3dxFrustumPlanes[1].w = -(mtx._44 - mtx._41);

	m_pd3dxFrustumPlanes[2].x = -(mtx._14 - mtx._12);
	m_pd3dxFrustumPlanes[2].y = -(mtx._24 - mtx._22);
	m_pd3dxFrustumPlanes[2].z = -(mtx._34 - mtx._32);
	m_pd3dxFrustumPlanes[2].w = -(mtx._44 - mtx._42);

	m_pd3dxFrustumPlanes[3].x = -(mtx._14 + mtx._12);
	m_pd3dxFrustumPlanes[3].y = -(mtx._24 + mtx._22);
	m_pd3dxFrustumPlanes[3].z = -(mtx._34 + mtx._32);
	m_pd3dxFrustumPlanes[3].w = -(mtx._44 + mtx._42);

	m_pd3dxFrustumPlanes[4].x = -(mtx._13);
	m_pd3dxFrustumPlanes[4].y = -(mtx._23);
	m_pd3dxFrustumPlanes[4].z = -(mtx._33);
	m_pd3dxFrustumPlanes[4].w = -(mtx._43);

	m_pd3dxFrustumPlanes[5].x = -(mtx._14 - mtx._13);
	m_pd3dxFrustumPlanes[5].y = -(mtx._24 - mtx._23);
	m_pd3dxFrustumPlanes[5].z = -(mtx._34 - mtx._33);
	m_pd3dxFrustumPlanes[5].w = -(mtx._44 - mtx._43);

	for (int i = 0; i < 6; i++) XMPlaneNormalize(XMLoadFloat4(&m_pd3dxFrustumPlanes[i]));
}

bool CCamera::IsInFrustum(XMVECTOR& xCenter, XMVECTOR& xExtern)
{
	XMFLOAT3 vNearPoint, vFarPoint, vPlaneNormal;
	XMFLOAT3 vTempMax, vTempMin;

	XMStoreFloat3(&vTempMin, XMVectorSubtract(xCenter, xExtern));
	XMStoreFloat3(&vTempMax, XMVectorAdd(xCenter, xExtern));

	for (int i = 0; i < 6; i++) {
		XMStoreFloat3(&vPlaneNormal, XMVectorSet(m_pd3dxFrustumPlanes[i].x, m_pd3dxFrustumPlanes[i].y, m_pd3dxFrustumPlanes[i].z, 0.0f));

		if (vPlaneNormal.x >= 0.0f) {
			if (vPlaneNormal.y >= 0.0f) {
				if (vPlaneNormal.z >= 0.0f) {
					vNearPoint.x = vTempMin.x; vNearPoint.y = vTempMin.y; vNearPoint.z = vTempMin.z;
				}
				else {
					vNearPoint.x = vTempMin.x; vNearPoint.y = vTempMin.y; vNearPoint.z = vTempMax.z;
				}
			}
			else {
				if (vPlaneNormal.z >= 0.0f) {
					vNearPoint.x = vTempMin.x; vNearPoint.y = vTempMax.y; vNearPoint.z = vTempMin.z;
				}
				else {
					vNearPoint.x = vTempMin.x; vNearPoint.y = vTempMax.y; vNearPoint.z = vTempMax.z;
				}
			}
		}
		else {
			if (vPlaneNormal.y >= 0.0f) {
				if (vPlaneNormal.z >= 0.0f) {
					vNearPoint.x = vTempMax.x; vNearPoint.y = vTempMin.y; vNearPoint.z = vTempMin.z;
				}
				else {
					vNearPoint.x = vTempMax.x; vNearPoint.y = vTempMin.y; vNearPoint.z = vTempMax.z;
				}
			}
			else {
				if (vPlaneNormal.z >= 0.0f) {
					vNearPoint.x = vTempMax.x; vNearPoint.y = vTempMax.y; vNearPoint.z = vTempMin.z;
				}
				else {
					vNearPoint.x = vTempMax.x; vNearPoint.y = vTempMax.y; vNearPoint.z = vTempMax.z;
				}
			}
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_pd3dxFrustumPlanes[i]), XMLoadFloat3(&vNearPoint))) > 0.0f)
			return false;
	}
	return true;
}

bool CCamera::IsInFrustum(BoundingBox *boundingbox)
{
	return(IsInFrustum(XMLoadFloat3(&boundingbox->Center), XMLoadFloat3(&boundingbox->Extents)));
}

bool CCamera::IsInFrustum(BoundingSphere *boundingSphere)
{
	return(IsInFrustum(XMLoadFloat3(&boundingSphere->Center), XMVectorSet(boundingSphere->Radius, boundingSphere->Radius, boundingSphere->Radius, 0.0f)));
}

void CCamera::OnCollisionCheck()
{
	/*
	XMFLOAT3 m_vDir;
	XMStoreFloat3(&m_vDir, m_pPlayer->GetvPosition() - XMLoadFloat3(&m_vPosition));
//	XMStoreFloat3(&m_vDir, XMLoadFloat3(&m_vPosition) - m_pPlayer->GetvPosition());
	XMStoreFloat3(&m_vDir, XMVector3Normalize(XMLoadFloat3(&m_vDir)));

//	PxVec3 OriginPos = PxVec3(m_vPosition.x, m_vPosition.y, m_vPosition.z);
	PxVec3 OriginPos = PxVec3(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, m_pPlayer->GetPosition().z);
	PxVec3 m_vPxDir = PxVec3(m_vDir.x, m_vDir.y, m_vDir.z);

	COLLISION_MGR->CreateFireDirectionLine(XMLoadFloat3(&m_vPosition), XMLoadFloat3(&m_vDir), 100);


	PxReal maxDistance = 9999.f;
	PxRaycastBuffer hit;
	PxQueryFilterData fd;
	fd.flags |= PxQueryFlag::ePOSTFILTER;

	if (SCENE_MGR->g_pPxScene == nullptr)
		return;
	
	bool m_bstatus = false;
	m_bstatus = SCENE_MGR->g_pPxScene->raycast(OriginPos, m_vPxDir, maxDistance, hit, PxHitFlags(PxHitFlag::eDEFAULT), fd);
	if (m_bstatus) {
		cout << hit.block.distance << endl;
		
		if ((hit.block.distance < m_fGap + 1.f)) {

			XMFLOAT3 vPos = XMFLOAT3(hit.block.position.x + hit.block.normal.x,
				hit.block.position.y + hit.block.normal.y,
				hit.block.position.z + hit.block.normal.z);

			XMStoreFloat3(&m_vPosition, XMLoadFloat3(&vPos));
		}
		
	}
	*/
}

void CCamera::UpdateOffset()
{
	XMVECTOR pos = m_pPlayer->GetvPosition();

	XMStoreFloat3(&m_vPosition, pos +
		m_pPlayer->GetvRight() * m_vOffset.x +
		m_pPlayer->GetvUp() * m_vOffset.y +
		m_pPlayer->GetvLook() * m_vOffset.z);

	if(m_tagCamera == CameraTag::eFirstPerson)
		Rotate(m_pPlayer->GetPitch(), 0, 0);
}