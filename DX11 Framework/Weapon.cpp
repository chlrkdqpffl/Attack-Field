#include "stdafx.h"
#include "Weapon.h"
#include "CharacterObject.h"

CWeapon::CWeapon(ID3D11Device* pd3dDevice, CCharacterObject* pOwner) 
	: m_pOwner(pOwner), m_pd3dDevice(pd3dDevice)
{
//	TWBAR_MGR->g_xmf3Offset = XMFLOAT3(-8.5f, 13.f, 0.5f);
//	TWBAR_MGR->g_xmf3Rotate = XMFLOAT3(-35.f, -90.f, 100.f);

	for (int i = 0; i < 2; ++i) {
		CBulletObject* pBulletObject = new CBulletObject();
		pBulletObject->CreateObjectData(pd3dDevice);
		pBulletObject->CreateAxisObject(pd3dDevice);
		pBulletObject->SetActive(false);
		m_vecBulletContainer.push_back(pBulletObject);
	}
}

CWeapon::~CWeapon()
{
	for (auto& bullet : m_vecBulletContainer)
		SafeDelete(bullet);
}

void CWeapon::Fire()
{
	for (auto bullet : m_vecBulletContainer) {
		if (false == bullet->GetActive()) {
			bullet->SetActive(true);
			bullet->SetPosition(GetPosition());
			break;
		}
	}
}

void CWeapon::Update(float fTimeElapsed)
{
	CGameObject::Update(fTimeElapsed);

	for (auto bullet : m_vecBulletContainer)
		bullet->Update(fTimeElapsed);
	
	m_mtxParent = m_pOwner->GetSkinnedMesh()->GetFinalBoneMtx(m_nBoneIndex);
	XMStoreFloat4x4(&m_d3dxmtxWorld, XMLoadFloat4x4(&m_d3dxmtxLocal) * m_mtxParent * XMLoadFloat4x4(&m_pOwner->m_d3dxmtxWorld));

//	SetRotate(TWBAR_MGR->g_xmf3Rotate, true);
//	SetPosition(TWBAR_MGR->g_xmf3Offset, true);
}

void CWeapon::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CGameObject::Render(pd3dDeviceContext, pCamera);

	for (auto bullet : m_vecBulletContainer)
		if (bullet->GetActive()) 
			bullet->Render(pd3dDeviceContext, pCamera);
}