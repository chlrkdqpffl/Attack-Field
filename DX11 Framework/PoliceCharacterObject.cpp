#include "stdafx.h"
#include "PoliceCharacterObject.h"


CPoliceCharacterObject::CPoliceCharacterObject()
{
}

CPoliceCharacterObject::~CPoliceCharacterObject()
{
}

void CPoliceCharacterObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	CFbxModelSkinnedMesh* pCharacterMesh = new CFbxModelSkinnedMesh(pd3dDevice, MeshTag::ePolice);
	pCharacterMesh->Initialize(pd3dDevice);

	SetMesh(pCharacterMesh);
}

void CPoliceCharacterObject::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CCharacterNormalShader();
	m_pShader->CreateShader(pd3dDevice);
}

void CPoliceCharacterObject::CreateMaterial()
{
	CTexture *pCharacterTexture = new CTexture(1, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);

	m_pMaterial = new CMaterial();
	pCharacterTexture->SetTexture(0, TextureTag::ePoliceD);
//	pCharacterTexture->SetTexture(1, TextureTag::eTerroristND);
	pCharacterTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	m_pMaterial->SetTexture(pCharacterTexture);
}

void CPoliceCharacterObject::CreateAnimation()
{
//	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eIdle,				AnimationTrack("Take_001"), AnimationData::Type::eLoop));
//	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eIdle,				AnimationTrack("Idle"), AnimationData::Type::eLoop));
//	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk,				AnimationTrack("Walk"), AnimationData::Type::eLoop));
//	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eRun,				AnimationTrack("Run"), AnimationData::Type::eLoop));
//	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eStanding_Fire,	AnimationTrack("StandingFire"), AnimationData::Type::eOnce));
}

void CPoliceCharacterObject::CreateWeapon(ID3D11Device *pd3dDevice)
{
	m_pWeapon = new CRifleGunWeapon(pd3dDevice, this);

	m_pWeapon->CreateObjectData(pd3dDevice);
	m_pWeapon->CreateAxisObject(pd3dDevice);
}