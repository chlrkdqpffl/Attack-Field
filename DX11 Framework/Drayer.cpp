#include "stdafx.h"
#include "Drayer.h"

CDrayer::CDrayer()
{
}

CDrayer::~CDrayer()
{
}

void CDrayer::CreateMesh(ID3D11Device *pd3dDevice)
{
	//CSkinnedMesh* pCharacterMesh = new CSkinnedMesh(pd3dDevice, RESOURCE_MGR->FindResourcePath(MeshData::eMesh_Siegetank), 0.013f);
	CSkinnedMesh* pCharacterMesh = new CSkinnedMesh(pd3dDevice, RESOURCE_MGR->FindResourcePath(MeshData::eMesh_Drayer));
	pCharacterMesh->Initialize(pd3dDevice);

	SetMesh(pCharacterMesh);
}

void CDrayer::CreateShader(ID3D11Device *pd3dDevice)
{
	m_pShader = new CCharacterShader();
	m_pShader->CreateShader(pd3dDevice);
}

void CDrayer::CreateMaterial(ID3D11Device *pd3dDevice)
{
	CTexture *pCharacterTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	m_pMaterial = new CMaterial();

	pCharacterTexture->SetTexture(0, eTexture_DrayerDiffuse);
	pCharacterTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	m_pMaterial->SetTexture(pCharacterTexture);
}

void CDrayer::CreateAnimation()
{
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eIdle,	AnimationTrack("idle"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eRun,		AnimationTrack("Run"), AnimationData::Type::eLoop));
//	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eAttack,	AnimationTrack("CrossPunch"), AnimationData::Type::eOnce));
}