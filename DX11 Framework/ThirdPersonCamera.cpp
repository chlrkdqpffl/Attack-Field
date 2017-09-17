#include "stdafx.h"
#include "ThirdPersonCamera.h"

CThirdPersonCamera::CThirdPersonCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_tagCamera = CameraTag::eThirdPerson;
	if (pCamera)
	{
		if (pCamera->GetCameraTag() == CameraTag::eFreeCam)
		{
			XMStoreFloat3(&m_vUp, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			m_vRight.y = 0.0f;
			m_vLook.y = 0.0f;
			XMStoreFloat3(&m_vRight, XMVector3Normalize(XMLoadFloat3(&m_vRight)));
			XMStoreFloat3(&m_vLook, XMVector3Normalize(XMLoadFloat3(&m_vLook)));
		}
	}
}

void CThirdPersonCamera::Update(float fDeltaTime)
{
	m_vLook = m_pPlayer->GetLook();

	/*
	XMFLOAT4X4 mtxRotate;
	XMStoreFloat4x4(&mtxRotate, XMMatrixIdentity());
	XMFLOAT3 vRight = m_pPlayer->GetRight();
	XMFLOAT3 vUp = m_pPlayer->GetUp();
	XMFLOAT3 vLook = m_pPlayer->GetLook();
	mtxRotate._11 = vRight.x; mtxRotate._21 = vUp.x; mtxRotate._31 = vLook.x;
	mtxRotate._12 = vRight.y; mtxRotate._22 = vUp.y; mtxRotate._32 = vLook.y;
	mtxRotate._13 = vRight.z; mtxRotate._23 = vUp.z; mtxRotate._33 = vLook.z;

	XMVECTOR vOffset;
	vOffset = XMVector3TransformCoord(XMLoadFloat3(&m_vOffset), XMLoadFloat4x4(&mtxRotate));
	XMVECTOR vPosition = m_pPlayer->GetvPosition() + vOffset;
	XMVECTOR vDirection = vPosition - XMLoadFloat3(&m_vPosition);
	float fLength = XMVectorGetX(XMVector3Length(vDirection));
	vDirection = XMVector3Normalize(vDirection);
	float fTimeLagScale = (m_fTimeLag) ? fDeltaTime * (1.0f / m_fTimeLag) : 1.0f;
	float fDistance = fLength * fTimeLagScale;
	if (fDistance > fLength) fDistance = fLength;
	if (fLength < 0.01f) fDistance = fLength;

	if (fDistance > 0)
	{
		XMStoreFloat3(&m_vPosition, XMLoadFloat3(&m_vPosition) += vDirection * fDistance);
		SetLookAt(m_pPlayer->GetvPosition());
	}
	*/
}