#include "stdafx.h"
#include "CharacterObject.h"

CCharacterObject::CCharacterObject()
{
}

CCharacterObject::~CCharacterObject()
{
	SafeDelete(m_pWeapon);
}

void CCharacterObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y;
	m_d3dxmtxWorld._43 = z;

	m_pWeapon->SetPosition(x, y, z);
}

void CCharacterObject::SetPosition(XMVECTOR d3dxvPosition)
{
	XMFLOAT4 f4vPosition;
	XMStoreFloat4(&f4vPosition, d3dxvPosition);
	SetPosition(f4vPosition.x, f4vPosition.y, f4vPosition.z);
}

void CCharacterObject::SetPosition(XMFLOAT3 pos)
{
	m_d3dxmtxWorld._41 = pos.x;
	m_d3dxmtxWorld._42 = pos.y;
	m_d3dxmtxWorld._43 = pos.z;

	m_pWeapon->SetPosition(pos);
}

void CCharacterObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CSkinnedObject::CreateObjectData(pd3dDevice);
	CreateWeapon(pd3dDevice);
}

void CCharacterObject::Update(float fTimeElapsed)
{
	CSkinnedObject::Update(fTimeElapsed);
}

void CCharacterObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CSkinnedObject::Render(pd3dDeviceContext, pCamera);
	m_pWeapon->Render(pd3dDeviceContext, pCamera);
}