#include "stdafx.h"
#include "CharacterObject.h"

CCharacterObject::CCharacterObject()
{
}

CCharacterObject::~CCharacterObject()
{
	SafeDelete(m_pWeapon);
}

void CCharacterObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CSkinnedObject::CreateObjectData(pd3dDevice);
	CreateWeapon(pd3dDevice);
}

void CCharacterObject::Fire()
{
	SetAnimation(AnimationData::CharacterAnim::eStandingFire);
	m_pWeapon->Fire();
}

void CCharacterObject::OnCollisionCheck()
{
	CollisionInfo info;

	if (COLLISION_MGR->OBBCollision(info, GetBoundingOBox())) {
	//	m_pPlayer->SetCollision(true);


		//XMVECTOR velocity = m_pPlayer->GetVelocity();
		//m_pPlayer->Move(-1 * velocity);
	//	m_pPlayer->Move(XMVectorSet(0, GetBoundingOBox().Extents.y, 0, 0));
	//	m_pPlayer->SetPosition(XMVectorSet(60, 100, 20, 0));

		return;
	}

} 
void CCharacterObject::Update(float fTimeElapsed)
{
	if (m_pPlayer) {
		m_pPlayer->OnPrepareRender();
		m_pPlayer->UpdateKeyInput(fTimeElapsed);
		m_pPlayer->Update(fTimeElapsed);
	}

	CGameObject::Update(fTimeElapsed);
	CSkinnedObject::Update(fTimeElapsed);
	m_pWeapon->Update(fTimeElapsed);

	OnCollisionCheck();
}

void CCharacterObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pPlayer->GetCamera()->GetCameraTag() == CameraTag::eThirdPerson) {
		CSkinnedObject::Render(pd3dDeviceContext, pCamera);
		m_pPlayer->UpdateShaderVariables(pd3dDeviceContext);
	}
	m_pWeapon->Render(pd3dDeviceContext, pCamera);
}