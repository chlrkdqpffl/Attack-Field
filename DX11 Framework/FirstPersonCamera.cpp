#include "stdafx.h"
#include "FirstPersonCamera.h"

CFirstPersonCamera::CFirstPersonCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_tagCamera = CameraTag::eFirstPerson;
	if (pCamera) {
		if (pCamera->GetCameraTag() == CameraTag::eSpaceShip) {
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

		XMStoreFloat3(&xmPosition, m_pPlayer->GetvPosition());
		m_d3dxvPosition.x -= xmPosition.x;
		m_d3dxvPosition.y -= xmPosition.y;
		m_d3dxvPosition.z -= xmPosition.z;
		XMStoreFloat3(&m_d3dxvPosition, XMVector3TransformCoord(XMLoadFloat3(&m_d3dxvPosition), mtxRotate));
		m_d3dxvPosition.x += xmPosition.x;
		m_d3dxvPosition.y += xmPosition.y;
		m_d3dxvPosition.z += xmPosition.z;
	}
	if (m_pPlayer && (y != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetvUp(), XMConvertToRadians(y));
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
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetvLook(), XMConvertToRadians(z));
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

void CFirstPersonCamera::Update(float fDeltaTime)
{
	m_d3dxvUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = m_pPlayer->GetLook();

	XMVECTOR pos = m_pPlayer->GetvPosition();

	XMStoreFloat3(&m_d3dxvPosition, pos +
		m_pPlayer->GetvRight() * m_vOffset.x +
		m_pPlayer->GetvUp() * m_vOffset.y +
		m_pPlayer->GetvLook() * m_vOffset.z);

//	cout << m_pPlayer->GetPitch() << endl;
	Rotate(m_pPlayer->GetPitch(), 0, 0);
}