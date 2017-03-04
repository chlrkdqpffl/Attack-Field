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
	CSkinnedMesh* pCharacterMesh = new CSkinnedMesh(pd3dDevice, RESOURCE_MGR->FindResourcePath(MeshData::eMesh_Terrorist));
	pCharacterMesh->Initialize(pd3dDevice);

	SetMesh(pCharacterMesh);
}

void CPoliceCharacterObject::CreateShader(ID3D11Device *pd3dDevice)
{
//	m_pShader = new CCharacterShader();
	m_pShader = new CCharacterNormalShader();
	m_pShader->CreateShader(pd3dDevice);
}

void CPoliceCharacterObject::CreateMaterial(ID3D11Device *pd3dDevice)
{
	CTexture *pCharacterTexture = new CTexture(2, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);

	m_pMaterial = new CMaterial();
	pCharacterTexture->SetTexture(0, eTexture_TerroristDiffuse);
	pCharacterTexture->SetTexture(1, eTexture_TerroristNormalDisplace);
	pCharacterTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	m_pMaterial->SetTexture(pCharacterTexture);
}

void CPoliceCharacterObject::CreateAnimation()
{
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eIdle,			AnimationTrack("Idle"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk,			AnimationTrack("Walk"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eRun,			AnimationTrack("Run"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eStanding_Fire, AnimationTrack("StandingFire"), AnimationData::Type::eOnce));
}

void CPoliceCharacterObject::CreateWeapon(ID3D11Device *pd3dDevice)
{
	m_pWeapon = new CRifleGunWeapon();
	
	m_pWeapon->CreateObjectData(pd3dDevice);

	XMFLOAT4X4 rightHandOffsetMtx = m_pSkinnedMesh->GetOffsetMtx(10);

	m_pWeapon->SetPosition(rightHandOffsetMtx._41, rightHandOffsetMtx._42, rightHandOffsetMtx._43);
	m_pWeapon->Rotate(0, -90, 0);
}