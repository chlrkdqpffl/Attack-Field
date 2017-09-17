#include "stdafx.h"
#include "SpaceShipCamera.h"

CSpaceShipCamera::CSpaceShipCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_tagCamera = CameraTag::eFreeCam;
}

CSpaceShipCamera::~CSpaceShipCamera()
{
}

void CSpaceShipCamera::Rotate(float x, float y, float z)
{
	XMMATRIX mtxRotate;
	XMFLOAT3 xmPosition;
	if (m_pPlayer && (x != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetvRight(), XMConvertToRadians(x));
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

void CSpaceShipCamera::Update(float fDeltaTime)
{
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_vLook = m_pPlayer->GetLook();
	XMStoreFloat3(&m_vRight, XMVector3Cross(XMLoadFloat3(&m_vUp), XMLoadFloat3(&m_vLook)));
}