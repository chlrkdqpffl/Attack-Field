#include "stdafx.h"
#include "TerroristCharacterObject.h"


CTerroristCharacterObject::CTerroristCharacterObject(TeamType team)
	:CCharacterObject(team)
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

	CFbxModelSkinnedMesh* pCharacterArmMesh = new CFbxModelSkinnedMesh(pd3dDevice, MeshTag::eTerrorist_Arm);
	pCharacterArmMesh->Initialize(pd3dDevice);
	SetMesh(pCharacterArmMesh, 1);

	m_pSkinnedMesh->SetBodyBoundaryIndex(32);
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

	if (m_tagTeam == TeamType::eRedTeam)
		pCharacterTexture->SetTexture(0, TextureTag::eTerroristD_Red);
	else if (m_tagTeam == TeamType::eBlueTeam)
		pCharacterTexture->SetTexture(0, TextureTag::eTerroristD_Blue);
	else 
		cout << "Terrorist Team 미설정 - 텍스쳐 오류 발생!" << endl;

	pCharacterTexture->SetTexture(1, TextureTag::eTerroristND);
	pCharacterTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	pMaterial->SetTexture(pCharacterTexture);

	SetMaterial(pMaterial);
}

void CTerroristCharacterObject::CreateAnimation()
{
	// ----- 텍스트 파일과 애니메이션 순서가 일치해야함 ----- //

	AddAnimation(make_tuple(AnimationData::CharacterAnim::eIdle,				AnimationTrack("Idle"),				AnimationData::Type::eLoop));

	// Walk
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk_Left,			AnimationTrack("Walk_Left"),		AnimationData::Type::eLoop));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk_Right,			AnimationTrack("Walk_Left"),		AnimationData::Type::eInverseLoop));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk_ForwardLeft,	AnimationTrack("Walk_ForwardLeft"),	AnimationData::Type::eLoop));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk_BackwardRight,	AnimationTrack("Walk_ForwardLeft"), AnimationData::Type::eInverseLoop));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk_Forward,		AnimationTrack("Walk_Forward"),		AnimationData::Type::eLoop));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk_Backward,		AnimationTrack("Walk_Forward"),		AnimationData::Type::eInverseLoop));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk_ForwardRight,	AnimationTrack("Walk_ForwardRight"), AnimationData::Type::eLoop));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eWalk_BackwardLeft,	AnimationTrack("Walk_ForwardRight"), AnimationData::Type::eInverseLoop));

	AddAnimation(make_tuple(AnimationData::CharacterAnim::eRun,					AnimationTrack("Run"),				AnimationData::Type::eLoop));

	AddAnimation(make_tuple(AnimationData::CharacterAnim::eFire,				AnimationTrack("Fire"),				AnimationData::Type::eLoop));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eReload,				AnimationTrack("Reload"),			AnimationData::Type::eOnce));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eDeath_Head,			AnimationTrack("Death_Head"),		AnimationData::Type::eOnce));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eDeath,				AnimationTrack("Death"),			AnimationData::Type::eOnce));
	AddAnimation(make_tuple(AnimationData::CharacterAnim::eHeadHit,				AnimationTrack("HeadHit"),			AnimationData::Type::eOnce));

	AddAnimation(make_tuple(AnimationData::CharacterAnim::eCrouch,				AnimationTrack("Crouch"),			AnimationData::Type::eLoop));

}

void CTerroristCharacterObject::CreateWeapon(ID3D11Device *pd3dDevice)
{
	m_pWeapon[0] = new CRifleGunWeapon(this);
	m_pWeapon[1] = new CSniperRifle(this);

	for (int i = 0; i < static_cast<UINT>(WeaponTag::eMaxWeaponCount); ++i) {
		m_pWeapon[i]->CreateObjectData(pd3dDevice);
		m_pWeapon[i]->CreateAxisObject(pd3dDevice);
	}
}

void CTerroristCharacterObject::CreateBoundingBox(ID3D11Device *pd3dDevice)
{
	// ----- Bounding Box Size ----- //
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eBody)].Center = XMFLOAT3(0.00f, 1.95f, 0.0f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eBody)].Extents = XMFLOAT3(0.3f, 0.55f, 0.22f);

	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eHead)].Center = XMFLOAT3(0, 2.75f, 0);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eHead)].Extents = XMFLOAT3(0.14f, 0.17f, 0.12f);

	// --- Arm --- //
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftUpArm)].Center = XMFLOAT3(-0.4f, 2.1f, -0.1f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftUpArm)].Extents = XMFLOAT3(0.08f, 0.3f, 0.08f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftDownArm)].Center = XMFLOAT3(-0.8f, 1.6f, -0.1f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftDownArm)].Extents = XMFLOAT3(0.08f, 0.3f, 0.1f);

	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightUpArm)].Center = XMFLOAT3(0.45f, 2.15f, -0.1f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightUpArm)].Extents = XMFLOAT3(0.08f, 0.22f, 0.1f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightDownArm)].Center = XMFLOAT3(0.8f, 1.65f, -0.15f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightDownArm)].Extents = XMFLOAT3(0.08f, 0.3f, 0.1f);

	// --- Leg --- //
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftUpLeg)].Center = XMFLOAT3(-0.2f, 1.1f, 0.0f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftUpLeg)].Extents = XMFLOAT3(0.13f, 0.3f, 0.15f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftDownLeg)].Center = XMFLOAT3(-0.175f, 0.475f, 0.0f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftDownLeg)].Extents = XMFLOAT3(0.12f, 0.35f, 0.17f);

	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightUpLeg)].Center = XMFLOAT3(0.175f, 1.1f, 0.0f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightUpLeg)].Extents = XMFLOAT3(0.13, 0.3f, 0.15f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightDownLeg)].Center = XMFLOAT3(0.175f, 0.45f, 0.0f);
	m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightDownLeg)].Extents = XMFLOAT3(0.12f, 0.35f, 0.17f);


	// ----- Bounding Box Mesh ----- //
	m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::eBody)] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eBody)]);
	m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::eHead)] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eHead)]);

	// --- Arm --- //
	m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::eLeftUpArm)] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftUpArm)]);
	m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::eLeftDownArm)] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftDownArm)]);

	m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::eRightUpArm)] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightUpArm)]);
	m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::eRightDownArm)] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightDownArm)]);

	// --- Leg --- //
	m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::eLeftUpLeg)] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftUpLeg)]);
	m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::eLeftDownLeg)] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eLeftDownLeg)]);
	m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::eRightUpLeg)] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightUpLeg)]);
	m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::eRightDownLeg)] = new CBoundingBoxMesh(pd3dDevice, m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::eRightDownLeg)]);
	
	m_pBoundingBoxMesh = new CBoundingBoxMesh(pd3dDevice, m_bcMeshBoundingBox);
}