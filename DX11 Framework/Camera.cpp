#include "stdafx.h"
#include "Player.h"
#include "Camera.h"

ID3D11Buffer *CCamera::m_pd3dcbCamera = NULL;
ID3D11Buffer *CCamera::m_pd3dcbCameraPosition = NULL;

CCamera::CCamera(CCamera *pCamera)
{
	if (pCamera)
	{
		m_d3dxvPosition = pCamera->GetPosition();
		XMStoreFloat3(&m_d3dxvRight, pCamera->GetRightVector());
		XMStoreFloat3(&m_d3dxvLook, pCamera->GetLookVector());
		XMStoreFloat3(&m_d3dxvUp, pCamera->GetUpVector());
		m_fPitch = pCamera->GetPitch();
		m_fRoll = pCamera->GetRoll();
		m_fYaw = pCamera->GetYaw();
		XMStoreFloat4x4(&m_d3dxmtxView, pCamera->GetViewMatrix());
		XMStoreFloat4x4(&m_d3dxmtxProjection, pCamera->GetProjectionMatrix());
		m_d3dViewport = pCamera->GetViewport();
		XMStoreFloat3(&m_d3dxvOffset, pCamera->GetOffset());
		m_fTimeLag = pCamera->GetTimeLag();
		m_pPlayer = pCamera->GetPlayer();
	}
	else
	{
		XMStoreFloat3(&m_d3dxvPosition, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		XMStoreFloat3(&m_d3dxvRight, XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
		XMStoreFloat3(&m_d3dxvUp, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		XMStoreFloat3(&m_d3dxvLook, XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = 0.0f;
		m_fTimeLag = 0.0f;
		XMStoreFloat3(&m_d3dxvOffset, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		m_pPlayer = NULL;
		XMStoreFloat4x4(&m_d3dxmtxView, XMMatrixIdentity());
		XMStoreFloat4x4(&m_d3dxmtxProjection, XMMatrixIdentity());
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

	XMStoreFloat3(&m_d3dxvRight, XMVectorSet(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31, 0.0f));
	XMStoreFloat3(&m_d3dxvUp, XMVectorSet(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32, 0.0f));
	XMStoreFloat3(&m_d3dxvLook, XMVectorSet(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33, 0.0f));
}

void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	XMStoreFloat4x4(&m_d3dxmtxProjection, 
		XMMatrixPerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance));
}

void CCamera::GenerateViewMatrix()
{
	XMStoreFloat4x4(&m_d3dxmtxView, XMMatrixLookAtLH(XMLoadFloat3(&m_d3dxvPosition), m_pPlayer->GetvPosition(), XMLoadFloat3(&m_d3dxvUp)));
}

void CCamera::RegenerateViewMatrix()
{
	XMStoreFloat3(&m_d3dxvLook, XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook)));
	XMStoreFloat3(&m_d3dxvRight, XMVector3Cross(XMLoadFloat3(&m_d3dxvUp), XMLoadFloat3(&m_d3dxvLook)));
	XMStoreFloat3(&m_d3dxvRight, XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight)));
	XMStoreFloat3(&m_d3dxvUp, XMVector3Cross(XMLoadFloat3(&m_d3dxvLook), XMLoadFloat3(&m_d3dxvRight)));
	XMStoreFloat3(&m_d3dxvUp, XMVector3Normalize(XMLoadFloat3(&m_d3dxvUp)));
	m_d3dxmtxView._11 = m_d3dxvRight.x; m_d3dxmtxView._12 = m_d3dxvUp.x; m_d3dxmtxView._13 = m_d3dxvLook.x;
	m_d3dxmtxView._21 = m_d3dxvRight.y; m_d3dxmtxView._22 = m_d3dxvUp.y; m_d3dxmtxView._23 = m_d3dxvLook.y;
	m_d3dxmtxView._31 = m_d3dxvRight.z; m_d3dxmtxView._32 = m_d3dxvUp.z; m_d3dxmtxView._33 = m_d3dxvLook.z;
	m_d3dxmtxView._41 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_d3dxvPosition), XMLoadFloat3(&m_d3dxvRight)));
	m_d3dxmtxView._42 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_d3dxvPosition), XMLoadFloat3(&m_d3dxvUp)));
	m_d3dxmtxView._43 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_d3dxvPosition), XMLoadFloat3(&m_d3dxvLook)));

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
	XMStoreFloat4x4(&pcbViewProjection->m_d3dxmtxView, XMMatrixTranspose(*pd3dxmtxView));
	XMStoreFloat4x4(&pcbViewProjection->m_d3dxmtxProjection, XMMatrixTranspose(*pd3dxmtxProjection));
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

//	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_CAMERA_POSITION, 1, &m_pd3dcbCameraPosition);
	pd3dDeviceContext->HSSetConstantBuffers(HS_CB_SLOT_CAMERA_POSITION, 1, &m_pd3dcbCameraPosition);
	pd3dDeviceContext->DSSetConstantBuffers(DS_CB_SLOT_CAMERA_POSITION, 1, &m_pd3dcbCameraPosition);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_CAMERA_POSITION, 1, &m_pd3dcbCameraPosition);
}

void CCamera::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	UpdateConstantBuffer_ViewProjection(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxView), &XMLoadFloat4x4(&m_d3dxmtxProjection));
	UpdateConstantBuffer_CameraPos(pd3dDeviceContext, &XMLoadFloat3(&m_d3dxvPosition));
}

void CCamera::CalculateFrustumPlanes()
{
	XMMATRIX mtxViewProjection = XMLoadFloat4x4(&m_d3dxmtxView) * XMLoadFloat4x4(&m_d3dxmtxProjection);

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
	XMFLOAT3 xmVectorNearPoint, xmVectorFarPoint, xmVectorNormal;
	XMFLOAT3 xmVectorTempMax, xmVectorTempMin;

	XMStoreFloat3(&xmVectorTempMin, XMVectorSubtract(xCenter, xExtern));
	XMStoreFloat3(&xmVectorTempMax, XMVectorAdd(xCenter, xExtern));

	for (int i = 0; i < 6; i++)
	{
		XMStoreFloat3(&xmVectorNormal, XMVectorSet(m_pd3dxFrustumPlanes[i].x, m_pd3dxFrustumPlanes[i].y, m_pd3dxFrustumPlanes[i].z, 0.0f));

		if (xmVectorNormal.x >= 0.0f)
		{
			if (xmVectorNormal.y >= 0.0f)
			{
				if (xmVectorNormal.z >= 0.0f)
				{
					xmVectorNearPoint.x = xmVectorTempMin.x; xmVectorNearPoint.y = xmVectorTempMin.y; xmVectorNearPoint.z = xmVectorTempMin.z;
				}
				else
				{
					xmVectorNearPoint.x = xmVectorTempMin.x; xmVectorNearPoint.y = xmVectorTempMin.y; xmVectorNearPoint.z = xmVectorTempMax.z;
				}
			}
			else
			{
				if (xmVectorNormal.z >= 0.0f)
				{
					xmVectorNearPoint.x = xmVectorTempMin.x; xmVectorNearPoint.y = xmVectorTempMax.y; xmVectorNearPoint.z = xmVectorTempMin.z;
				}
				else
				{
					xmVectorNearPoint.x = xmVectorTempMin.x; xmVectorNearPoint.y = xmVectorTempMax.y; xmVectorNearPoint.z = xmVectorTempMax.z;
				}
			}
		}
		else
		{
			if (xmVectorNormal.y >= 0.0f)
			{
				if (xmVectorNormal.z >= 0.0f)
				{
					xmVectorNearPoint.x = xmVectorTempMax.x; xmVectorNearPoint.y = xmVectorTempMin.y; xmVectorNearPoint.z = xmVectorTempMin.z;
				}
				else
				{
					xmVectorNearPoint.x = xmVectorTempMax.x; xmVectorNearPoint.y = xmVectorTempMin.y; xmVectorNearPoint.z = xmVectorTempMax.z;
				}
			}
			else
			{
				if (xmVectorNormal.z >= 0.0f)
				{
					xmVectorNearPoint.x = xmVectorTempMax.x; xmVectorNearPoint.y = xmVectorTempMax.y; xmVectorNearPoint.z = xmVectorTempMin.z;
				}
				else
				{
					xmVectorNearPoint.x = xmVectorTempMax.x; xmVectorNearPoint.y = xmVectorTempMax.y; xmVectorNearPoint.z = xmVectorTempMax.z;
				}
			}
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_pd3dxFrustumPlanes[i]), XMLoadFloat3(&xmVectorNearPoint))) > 0.0f) return(false);
	}
	return(true);
}

bool CCamera::IsInFrustum(BoundingBox *boundingbox)
{
	return(IsInFrustum(XMLoadFloat3(&boundingbox->Center), XMLoadFloat3(&boundingbox->Extents)));
}