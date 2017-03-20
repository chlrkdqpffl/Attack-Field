#include "stdafx.h"
#include "Weapon.h"
#include "CharacterObject.h"

CWeapon::CWeapon(CCharacterObject* pOwner) : m_pOwner(pOwner)
{
//	TWBAR_MGR->g_xmf3Offset = XMFLOAT3(-8.5f, 13.f, 0.5f);
//	TWBAR_MGR->g_xmf3Rotate = XMFLOAT3(-35.f, -90.f, 100.f);
}

CWeapon::~CWeapon()
{
}

void CWeapon::Fire()
{
}

void CWeapon::Update(float fTimeElapsed)
{
	m_mtxParent = m_pOwner->GetSkinnedMesh()->GetFinalBoneMtx(m_nBoneIndex);

	XMStoreFloat4x4(&m_d3dxmtxWorld, m_mtxParent * XMLoadFloat4x4(&m_pOwner->m_d3dxmtxWorld));

//	SetRotate(TWBAR_MGR->g_xmf3Rotate, true);
//	SetPosition(TWBAR_MGR->g_xmf3Offset, true);
}