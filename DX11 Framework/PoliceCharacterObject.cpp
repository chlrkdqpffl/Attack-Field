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
	//CSkinnedMesh* pCharacterMesh = new CSkinnedMesh(pd3dDevice, RESOURCE_MGR->FindResourcePath(MeshData::eMesh_Siegetank), 0.013f);
	CSkinnedMesh* pCharacterMesh = new CSkinnedMesh(pd3dDevice, RESOURCE_MGR->FindResourcePath(MeshData::eMesh_Police));
	pCharacterMesh->Initialize(pd3dDevice);

	SetMesh(pCharacterMesh);
}

void CPoliceCharacterObject::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CCharacterShader();
	m_pShader->CreateShader(pd3dDevice);
}

void CPoliceCharacterObject::CreateMaterial(ID3D11Device *pd3dDevice)
{
	CTexture *pCharacterTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	m_pMaterial = new CMaterial();

	pCharacterTexture->SetTexture(0, eTexture_DrayerDiffuse);
	pCharacterTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	m_pMaterial->SetTexture(pCharacterTexture);
}

void CPoliceCharacterObject::CreateAnimation()
{
	m_pAnimController->AddAnimation(make_tuple(Animation::eIdle, AnimationTrack("Idle"), AnimationType::Loop));
	m_pAnimController->AddAnimation(make_tuple(Animation::eWalk, AnimationTrack("Walk"), AnimationType::Loop));
	m_pAnimController->AddAnimation(make_tuple(Animation::eRun, AnimationTrack("Run"), AnimationType::Loop));
	m_pAnimController->AddAnimation(make_tuple(Animation::eStanding_Fire, AnimationTrack("StandingFire"), AnimationType::Once));
}