#include "stdafx.h"
#include "CharacterObject.h"

CCharacterObject::CCharacterObject()
{
}

CCharacterObject::~CCharacterObject()
{
	SafeDelete(m_pWeapon);
}

void CCharacterObject::SetPosition(float x, float y, float z, bool isLocal)
{
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y;
	m_d3dxmtxWorld._43 = z;

	m_pWeapon->SetPosition(x, y, z);
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
	CreateBoundingBox(pd3dDevice);
}

void CCharacterObject::Update(float fTimeElapsed)
{
	CSkinnedObject::Update(fTimeElapsed);
	m_pWeapon->Update(fTimeElapsed);
}

void CCharacterObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CSkinnedObject::Render(pd3dDeviceContext, pCamera);
	m_pWeapon->Render(pd3dDeviceContext, pCamera);
}