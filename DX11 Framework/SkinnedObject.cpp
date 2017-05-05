#include "stdafx.h"
#include "SkinnedObject.h"


CSkinnedObject::CSkinnedObject()
{
	m_pUpperController = new CAnimationController(AnimationData::MultiAnimation::UpperBody);
	m_pLowerController = new CAnimationController(AnimationData::MultiAnimation::LowerBody);
}

CSkinnedObject::~CSkinnedObject()
{
	SafeDelete(m_pUpperController);
	SafeDelete(m_pLowerController);
}

void CSkinnedObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CGameObject::CreateObjectData(pd3dDevice);
	CreateAnimation();

	// Defalut Animation
	m_pUpperController->SetAnimation(AnimationData::CharacterAnim::eIdle);
	m_pLowerController->SetAnimation(AnimationData::CharacterAnim::eIdle);
}

void CSkinnedObject::SetMesh(CFbxModelSkinnedMesh* mesh)
{
	CGameObject::SetMesh(mesh);
	m_pSkinnedMesh = mesh;

	m_pUpperController->SetMesh(mesh);
	m_pLowerController->SetMesh(mesh);
}

void CSkinnedObject::SetAnimation(AnimationData::CharacterAnim anim, float speed)
{
	UINT nAnimType = static_cast<int>(anim);
	UINT nUpperBody = static_cast<int>(AnimationData::CharacterAnim::UpperBodyAnim);
	UINT nLowerBody = static_cast<int>(AnimationData::CharacterAnim::LowerBodyAnim);

	if (nAnimType < nUpperBody) {
		m_pUpperController->SetAnimation(anim, speed);
		
		float upperTimePos = m_pUpperController->GetTimePos();
		m_pLowerController->SetAnimation(anim, speed);
		m_pLowerController->SetTimePos(upperTimePos);
	}
	else if (nAnimType < nLowerBody)
		m_pUpperController->SetAnimation(anim, speed);
	else
		m_pLowerController->SetAnimation(anim, speed);
}

void CSkinnedObject::Update(float fDeltaTime)
{
	m_pUpperController->Update(fDeltaTime);
	m_pLowerController->Update(fDeltaTime);
}

void CSkinnedObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_pUpperController->UpdateConstantBuffer(pd3dDeviceContext);
	m_pLowerController->UpdateConstantBuffer(pd3dDeviceContext);
	CGameObject::Render(pd3dDeviceContext, pCamera);
}