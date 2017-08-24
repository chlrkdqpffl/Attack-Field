#include "stdafx.h"
#include "RotatingObject.h"


CRotatingObject::CRotatingObject(int nMeshes) : CGameObject(nMeshes)
{
	XMStoreFloat3(&m_d3dxvRotationAxis, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	m_fRotationSpeed = 15.0f;
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Update(float fDeltaTime)
{
	CGameObject::Rotate(&XMLoadFloat3(&m_d3dxvRotationAxis), m_fRotationSpeed * fDeltaTime);
}