#include "stdafx.h"
#include "FirstPersonCamera.h"

CFirstPersonCamera::CFirstPersonCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_tagCamera = CameraTag::eFirstPerson;
	if (pCamera) {
		if (pCamera->GetCameraTag() == CameraTag::eSpaceShip) {
			XMStoreFloat3(&m_vUp, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			m_vRight.y = 0.0f;
			m_vLook.y = 0.0f;
			XMStoreFloat3(&m_vRight, XMVector3Normalize(XMLoadFloat3(&m_vRight)));
			XMStoreFloat3(&m_vLook, XMVector3Normalize(XMLoadFloat3(&m_vLook)));
		}
	}
}

void CFirstPersonCamera::Rotate(float x, float y, float z)
{
	XMMATRIX mtxRotate;
	XMFLOAT3 xmPosition;
	if (m_pPlayer && (x != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_vRight), XMConvertToRadians(x));
		XMStoreFloat3(&m_vRight, XMVector3TransformNormal(XMLoadFloat3(&m_vRight), mtxRotate));
		XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vUp), mtxRotate));
		XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetvPosition());
		m_vPosition.x -= xmPosition.x;
		m_vPosition.y -= xmPosition.y;
		m_vPosition.z -= xmPosition.z;
		XMStoreFloat3(&m_vPosition, XMVector3TransformCoord(XMLoadFloat3(&m_vPosition), mtxRotate));
		m_vPosition.x += xmPosition.x;
		m_vPosition.y += xmPosition.y;
		m_vPosition.z += xmPosition.z;
	}
	if (m_pPlayer && (y != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetvUp(), XMConvertToRadians(y));
		XMStoreFloat3(&m_vRight, XMVector3TransformNormal(XMLoadFloat3(&m_vRight), mtxRotate));
		XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vUp), mtxRotate));
		XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetvPosition());
		m_vPosition.x -= xmPosition.x;
		m_vPosition.y -= xmPosition.y;
		m_vPosition.z -= xmPosition.z;
		XMStoreFloat3(&m_vPosition, XMVector3TransformCoord(XMLoadFloat3(&m_vPosition), mtxRotate));
		m_vPosition.x += xmPosition.x;
		m_vPosition.y += xmPosition.y;
		m_vPosition.z += xmPosition.z;
	}
	if (m_pPlayer && (z != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetvLook(), XMConvertToRadians(z));
		XMStoreFloat3(&m_vRight, XMVector3TransformNormal(XMLoadFloat3(&m_vRight), mtxRotate));
		XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vUp), mtxRotate));
		XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetvPosition());
		m_vPosition.x -= xmPosition.x;
		m_vPosition.y -= xmPosition.y;
		m_vPosition.z -= xmPosition.z;
		XMStoreFloat3(&m_vPosition, XMVector3TransformCoord(XMLoadFloat3(&m_vPosition), mtxRotate));
		m_vPosition.x += xmPosition.x;
		m_vPosition.y += xmPosition.y;
		m_vPosition.z += xmPosition.z;
	}
}

void CFirstPersonCamera::Update(float fDeltaTime)
{
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_vLook = m_pPlayer->GetLook();

	XMVECTOR pos = m_pPlayer->GetvPosition();

	XMStoreFloat3(&m_vPosition, pos +
		m_pPlayer->GetvRight() * m_vOffset.x +
		m_pPlayer->GetvUp() * m_vOffset.y +
		m_pPlayer->GetvLook() * m_vOffset.z);

	Rotate(m_pPlayer->GetPitch(), 0, 0);
}