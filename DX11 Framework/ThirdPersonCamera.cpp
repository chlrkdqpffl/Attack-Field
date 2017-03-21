#include "stdafx.h"
#include "ThirdPersonCamera.h"


CThirdPersonCamera::CThirdPersonCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_tagCamera = CameraTag::eThirdPerson;
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

void CThirdPersonCamera::Update(XMVECTOR& d3dxvLookAt, float fTimeElapsed)
{
	if (m_pPlayer)
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
		float fLength = XMVectorGetX(XMVector3Length(d3dxvDirection));
		d3dxvDirection = XMVector3Normalize(d3dxvDirection);
		float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
		float fDistance = fLength * fTimeLagScale;
		if (fDistance > fLength) fDistance = fLength;
		if (fLength < 0.01f) fDistance = fLength;
		if (fDistance > 0)
		{
			XMStoreFloat3(&m_d3dxvPosition, XMLoadFloat3(&m_d3dxvPosition) += d3dxvDirection * fDistance);
			SetLookAt(d3dxvLookAt);
		}
	}
}

void CThirdPersonCamera::SetLookAt(XMVECTOR& d3dxvLookAt)
{
	XMFLOAT4X4 mtxLookAt;
	XMStoreFloat4x4(&mtxLookAt, XMMatrixLookAtLH(XMLoadFloat3(&m_d3dxvPosition), d3dxvLookAt, m_pPlayer->GetUpVector()));
	XMStoreFloat3(&m_d3dxvRight, XMVectorSet(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31, 0.0f));
	XMStoreFloat3(&m_d3dxvUp, XMVectorSet(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32, 0.0f));
	XMStoreFloat3(&m_d3dxvLook, XMVectorSet(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33, 0.0f));
}
