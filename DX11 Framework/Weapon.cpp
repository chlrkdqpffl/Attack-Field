#include "stdafx.h"
#include "Weapon.h"
#include "CharacterObject.h"

CWeapon::CWeapon(CCharacterObject* pOwner) 
	: m_pOwner(pOwner)
{
//	TWBAR_MGR->g_xmf3Offset = XMFLOAT3(-8.5f, 13.f, 0.5f);
//	TWBAR_MGR->g_xmf3Rotate = XMFLOAT3(-35.f, -90.f, 100.f);	
}

CWeapon::~CWeapon()
{
	for (auto& bullet : m_vecBulletContainer)
		SafeDelete(bullet);
}

void CWeapon::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CGameObject::CreateObjectData(pd3dDevice);
	CreateBulletPool(pd3dDevice);
}

void CWeapon::Fire()
{
	if (GetTickCount() - m_dwLastAttackTime >= m_uiFireSpeed) {
		for (auto bullet : m_vecBulletContainer) {
			if (false == bullet->GetActive()) {
				bullet->SetActive(true);
				bullet->SetPosition(GetPosition());

				XMFLOAT3 look; XMStoreFloat3(&look, -1 * m_pOwner->GetLook());		// ÀÓ½Ã·Î ¸ÂÃã
				bullet->SetLook(look);
				m_dwLastAttackTime = GetTickCount();
				break;
			}
		}
	}
}

void CWeapon::Update(float fTimeElapsed)
{
	CGameObject::Update(fTimeElapsed);

	for (auto bullet : m_vecBulletContainer) {
		if (bullet->GetActive())
			bullet->Update(fTimeElapsed);
	}

	m_mtxParent = m_pOwner->GetSkinnedMesh()->GetFinalBoneMtx(m_nBoneIndex);
	XMStoreFloat4x4(&m_d3dxmtxWorld, XMLoadFloat4x4(&m_d3dxmtxLocal) * m_mtxParent * XMLoadFloat4x4(&m_pOwner->m_d3dxmtxWorld));

//	SetRotate(TWBAR_MGR->g_xmf3Rotate, true);
//	SetPosition(TWBAR_MGR->g_xmf3Offset, true);


	XMMATRIX test = XMMatrixIdentity();
	test.r[0].m128_f32[3] = 1;
	ShowXMMatrix(test);
}

void CWeapon::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CGameObject::Render(pd3dDeviceContext, pCamera);

	for (auto bullet : m_vecBulletContainer)
		if (bullet->GetActive()) 
			bullet->Render(pd3dDeviceContext, pCamera);
}