#include "stdafx.h"
#include "FirstPersonCamera.h"

CFirstPersonCamera::CFirstPersonCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_tagCamera = CameraTag::eFirstPerson;
	if (pCamera)
	{
		if (pCamera->GetCameraTag() == CameraTag::eSpaceShip)
		{
			XMStoreFloat3(&m_d3dxvUp, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			m_d3dxvRight.y = 0.0f;
			m_d3dxvLook.y = 0.0f;
			XMStoreFloat3(&m_d3dxvRight, XMVector3Normalize(XMLoadFloat3(&m_d3dxvRight)));
			XMStoreFloat3(&m_d3dxvLook, XMVector3Normalize(XMLoadFloat3(&m_d3dxvLook)));
		}
	}
}

void CFirstPersonCamera::Rotate(float x, float y, float z)
{
	XMMATRIX mtxRotate;
	XMFLOAT3 xmPosition;
	if (m_pPlayer && (x != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_d3dxvRight), XMConvertToRadians(x));

		XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));
	}
	if (m_pPlayer && (y != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetUpVector(), XMConvertToRadians(y));
		XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetvPosition());
		m_d3dxvPosition.x -= xmPosition.x;
		m_d3dxvPosition.y -= xmPosition.y;
		m_d3dxvPosition.z -= xmPosition.z;
		XMStoreFloat3(&m_d3dxvPosition, XMVector3TransformCoord(XMLoadFloat3(&m_d3dxvPosition), mtxRotate));
		m_d3dxvPosition.x += xmPosition.x;
		m_d3dxvPosition.y += xmPosition.y;
		m_d3dxvPosition.z += xmPosition.z;
	}
	if (m_pPlayer && (z != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetLookVector(), XMConvertToRadians(z));
		XMStoreFloat3(&m_d3dxvRight, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvRight), mtxRotate));
		XMStoreFloat3(&m_d3dxvUp, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvUp), mtxRotate));
		XMStoreFloat3(&m_d3dxvLook, XMVector3TransformNormal(XMLoadFloat3(&m_d3dxvLook), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetvPosition());
		m_d3dxvPosition.x -= xmPosition.x;
		m_d3dxvPosition.y -= xmPosition.y;
		m_d3dxvPosition.z -= xmPosition.z;
		XMStoreFloat3(&m_d3dxvPosition, XMVector3TransformCoord(XMLoadFloat3(&m_d3dxvPosition), mtxRotate));
		m_d3dxvPosition.x += xmPosition.x;
		m_d3dxvPosition.y += xmPosition.y;
		m_d3dxvPosition.z += xmPosition.z;
	}
}

void CFirstPersonCamera::Update(float fTimeElapsed)
{
	XMFLOAT4X4 mtxRotate;
	XMStoreFloat4x4(&mtxRotate, XMMatrixIdentity());
	XMFLOAT3 d3dxvRight; XMStoreFloat3(&d3dxvRight, m_pPlayer->GetRightVector());
	XMFLOAT3 d3dxvUp; XMStoreFloat3(&d3dxvUp, m_pPlayer->GetUpVector());
	XMFLOAT3 d3dxvLook; XMStoreFloat3(&d3dxvLook, m_pPlayer->GetLookVector());
	mtxRotate._11 = d3dxvRight.x; mtxRotate._21 = d3dxvUp.x; mtxRotate._31 = d3dxvLook.x;
	mtxRotate._12 = d3dxvRight.y; mtxRotate._22 = d3dxvUp.y; mtxRotate._32 = d3dxvLook.y;
	mtxRotate._13 = d3dxvRight.z; mtxRotate._23 = d3dxvUp.z; mtxRotate._33 = d3dxvLook.z;

	XMVECTOR d3dxvOffset;
	d3dxvOffset = XMVector3TransformCoord(XMLoadFloat3(&m_d3dxvOffset), XMLoadFloat4x4(&mtxRotate));
	XMVECTOR d3dxvPosition = m_pPlayer->GetvPosition() + d3dxvOffset;
	XMVECTOR d3dxvDirection = d3dxvPosition - XMLoadFloat3(&m_d3dxvPosition);

	XMStoreFloat3(&m_d3dxvPosition, XMLoadFloat3(&m_d3dxvPosition) += d3dxvDirection);

	XMVECTOR lookPos = d3dxvPosition + XMLoadFloat3(&d3dxvLook);

	SetLookAt(lookPos);
}