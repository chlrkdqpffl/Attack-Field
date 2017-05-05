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
	m_pUpperController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eIdle,			AnimationTrack("Idle"), AnimationData::Type::eLoop));
	m_pLowerController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eIdle,			AnimationTrack("Idle"), AnimationData::Type::eLoop));

	m_pUpperController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eFire,			AnimationTrack("StandingFire"), AnimationData::Type::eLoop));

	m_pLowerController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eForwardWalk,		AnimationTrack("Walk"), AnimationData::Type::eLoop));
	m_pLowerController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eBackwardWalk,	AnimationTrack("Walk"), AnimationData::Type::eInverseLoop));
		
	/*
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eIdle,				AnimationTrack("Idle"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eForwardWalk,		AnimationTrack("Walk"), AnimationData::Type::eLoop));
	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eBackwardWalk,		AnimationTrack("Walk"), AnimationData::Type::eInverseLoop));
	*/
//	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eRun,				AnimationTrack("Run"), AnimationData::Type::eLoop));
//	m_pAnimController->AddAnimation(make_tuple(AnimationData::CharacterAnim::eStandingFire,		AnimationTrack("StandingFire"), AnimationData::Type::eLoop));
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
	m_bcPartsBoundingOBox[BoundingBoxParts::eBody].Center = XMFLOAT3(0.00f, 1.95f, 0.0f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eBody].Extents = XMFLOAT3(0.3f, 0.55f, 0.25f);

	m_bcPartsBoundingOBox[BoundingBoxParts::eHead].Center = XMFLOAT3(0, 2.75f, 0);
	m_bcPartsBoundingOBox[BoundingBoxParts::eHead].Extents = XMFLOAT3(0.15f, 0.18f, 0.15f);

	// --- Arm --- //
	m_bcPartsBoundingOBox[BoundingBoxParts::eLeftUpArm].Center = XMFLOAT3(-0.5f, 2.2f, -0.1f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eLeftUpArm].Extents = XMFLOAT3(0.08f, 0.3f, 0.08f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eLeftDownArm].Center = XMFLOAT3(-0.8f, 1.8f, -0.1f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eLeftDownArm].Extents = XMFLOAT3(0.1f, 0.3f, 0.1f);

	m_bcPartsBoundingOBox[BoundingBoxParts::eRightUpArm].Center = XMFLOAT3(0.5f, 2.15f, -0.2f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eRightUpArm].Extents = XMFLOAT3(0.1f, 0.3f, 0.1f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eRightDownArm].Center = XMFLOAT3(0.8f, 1.75f, -0.15f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eRightDownArm].Extents = XMFLOAT3(0.1f, 0.3f, 0.1f);

	// --- Leg --- //
	m_bcPartsBoundingOBox[BoundingBoxParts::eLeftUpLeg].Center = XMFLOAT3(-0.2f, 1.1f, 0.0f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eLeftUpLeg].Extents = XMFLOAT3(0.15f, 0.3f, 0.17f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eLeftDownLeg].Center = XMFLOAT3(-0.175f, 0.475f, 0.0f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eLeftDownLeg].Extents = XMFLOAT3(0.12f, 0.35f, 0.17f);


	m_bcPartsBoundingOBox[BoundingBoxParts::eRightUpLeg].Center = XMFLOAT3(0.175f, 1.1f, 0.0f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eRightUpLeg].Extents = XMFLOAT3(0.15f, 0.3f, 0.17f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eRightDownLeg].Center = XMFLOAT3(0.175f, 0.45f, 0.0f);
	m_bcPartsBoundingOBox[BoundingBoxParts::eRightDownLeg].Extents = XMFLOAT3(0.12f, 0.35f, 0.17f);


	// ----- Bounding Box Mesh ----- //
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eBody] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eBody]);
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eHead] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eHead]);

	// --- Arm --- //
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eLeftUpArm] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eLeftUpArm]);
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eLeftDownArm] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eLeftDownArm]);

	m_pPartsBoundingBoxMesh[BoundingBoxParts::eRightUpArm] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eRightUpArm]);
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eRightDownArm] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eRightDownArm]);

	// --- Leg --- //
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eLeftUpLeg] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eLeftUpLeg]);
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eLeftDownLeg] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eLeftDownLeg]);
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eRightUpLeg] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eRightUpLeg]);
	m_pPartsBoundingBoxMesh[BoundingBoxParts::eRightDownLeg] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[BoundingBoxParts::eRightDownLeg]);
	
	m_pBoundingBoxMesh = new CBoundingBoxMesh(pd3dDevice, m_bcMeshBoundingBox);
}
