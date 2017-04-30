#include "stdafx.h"
#include "TerroristCharacterObject.h"


CTerroristCharacterObject::CTerroristCharacterObject()
{
	m_tagMesh = MeshTag::eTerrorist;
}

CTerroristCharacterObject::~CTerroristCharacterObject()
{
}

void CTerroristCharacterObject::CreateMesh(ID3D11Device *pd3dDevice)
{
	CFbxModelSkinnedMesh* pCharacterMesh = new CFbxModelSkinnedMesh(pd3dDevice, MeshTag::eTerrorist);
	pCharacterMesh->Initialize(pd3dDevice);
	SetMesh(pCharacterMesh);
}

void CTerroristCharacterObject::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader* pShader = new CCharacterNormalShader();
	pShader->CreateShader(pd3dDevice);
	
	SetShader(pShader);
}

void CTerroristCharacterObject::CreateMaterial()
{
	CTexture *pCharacterTexture = new CTexture(2, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);

	CMaterial* pMaterial = new CMaterial();

	pCharacterTexture->SetTexture(0, TextureTag::eTerroristD);
	pCharacterTexture->SetTexture(1, TextureTag::eTerroristND);
	pCharacterTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	pMaterial->SetTexture(pCharacterTexture);

	SetMaterial(pMaterial);
}

void CTerroristCharacterObject::CreateAnimation()
{
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eIdle,				AnimationTrack("Idle"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk,				AnimationTrack("Walk"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eRun,				AnimationTrack("Run"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eStandingFire,		AnimationTrack("StandingFire"), AnimationData::Type::eLoop));
//	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalkingFire,		AnimationTrack("WalkingFire"), AnimationData::Type::eLoop));
//	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eStandingReloading, AnimationTrack("StandingReloading"), AnimationData::Type::eOnce));
}

void CTerroristCharacterObject::CreateWeapon(ID3D11Device *pd3dDevice)
{
	m_pWeapon = new CRifleGunWeapon(this);

	m_pWeapon->CreateObjectData(pd3dDevice);
	m_pWeapon->CreateAxisObject(pd3dDevice);
}

void CTerroristCharacterObject::CreateBoundingBox(ID3D11Device *pd3dDevice)
{
	// ----- Bounding Box Size ----- //
	m_bcPartsBoundingOBox[BoundingBoxParts::eBody].Center = XMFLOAT3(0.05f, 1.35f, 0.1f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eBody].Extents = XMFLOAT3(0.3f, 1.2f, 0.3f);

	m_bcPartsBoundingOBox[BoundingBoxParts::eHead].Center = XMFLOAT3(0, 2.75f, 0);
	m_bcPartsBoundingOBox[BoundingBoxParts::eHead].Extents = XMFLOAT3(0.15f, 0.18f, 0.15f);

	// ----- Bounding Box Mesh ----- //
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eBody] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eBody]);
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eHead] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eHead]);

	m_pBoundingBoxMesh = new CBoundingBoxMesh(pd3dDevice, m_bcMeshBoundingBox);
}
