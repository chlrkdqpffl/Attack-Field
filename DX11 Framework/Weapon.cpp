#include "stdafx.h"
#include "Weapon.h"
#include "CharacterObject.h"

CWeapon::CWeapon()
{
}

CWeapon::~CWeapon()
{
}

void CWeapon::Fire()
{
}

void CWeapon::Update(float fTimeElapsed)
{
	SetParentMtx(m_pSkinnedMesh->GetFinalBoneMtx(TWBAR_MGR->g_nBoneIndex));

//	XMFLOAT3 pos = GetPosition();
	XMStoreFloat4x4(&m_d3dxmtxWorld,  m_mtxParent);
//	XMStoreFloat4x4(&m_d3dxmtxLocal, m_mtxParent);
	Move(XMFLOAT3(100, 250, 100));
//	Move(XMFLOAT3(0, 250, 0));

//	ShowXMFloat4x4(m_d3dxmtxLocal);
//	SetPosition(m_pCharacterObject->GetPosition());

	TWBAR_MGR->g_xmf3Rotate.y = 180;
	Rotate(TWBAR_MGR->g_xmf3Rotate);
//	Move(TWBAR_MGR->g_xmf3Offset, true);
	SetPosition(TWBAR_MGR->g_xmf3Offset.x, TWBAR_MGR->g_xmf3Offset.y, TWBAR_MGR->g_xmf3Offset.z, true);
}