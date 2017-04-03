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
	CreateBoundingBox(pd3dDevice);
}

void CCharacterObject::Fire()
{
	SetAnimation(AnimationData::CharacterAnim::eStandingFire);
	m_pWeapon->Fire();
}

void CCharacterObject::Update(float fTimeElapsed)
{
	CGameObject::Update(fTimeElapsed);
	CSkinnedObject::Update(fTimeElapsed);
	m_pWeapon->Update(fTimeElapsed);
}

void CCharacterObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CSkinnedObject::Render(pd3dDeviceContext, pCamera);
	m_pWeapon->Render(pd3dDeviceContext, pCamera);
}