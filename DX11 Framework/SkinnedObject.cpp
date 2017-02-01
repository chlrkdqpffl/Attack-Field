#include "stdafx.h"
#include "SkinnedObject.h"


CSkinnedObject::CSkinnedObject()
{
	m_pAnimController = new CAnimationController();
}

CSkinnedObject::~CSkinnedObject()
{
	SafeDelete(m_pAnimController);
}

void CSkinnedObject::SetMesh(CSkinnedMesh* mesh) 
{
	CGameObject::SetMesh(mesh);
	m_pAnimController->SetMesh(mesh);
	m_pSkinnedMesh = mesh;
}

void CSkinnedObject::SetAnimation(Animation::Character anim)
{
	m_pAnimController->SetAnimation(anim);
}

//#define KEYFRAME_ANIMATION

#ifdef KEYFRAME_ANIMATION
void CSkinnedObject::Update(float fTimeElapsed)
{
	m_pSkinnedMesh->Update(fTimeElapsed);
}

void CSkinnedObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_pSkinnedMesh->UpdateBoneTransform(pd3dDeviceContext);
	m_pSkinnedMesh->UpdateConstantBuffer(pd3dDeviceContext);

	CGameObject::Render(pd3dDeviceContext, pCamera);
}
#else

void CSkinnedObject::Update(float fTimeElapsed)
{
	m_pAnimController->Update(fTimeElapsed);
}

void CSkinnedObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_pAnimController->UpdateConstantBuffer(pd3dDeviceContext);
	CGameObject::Render(pd3dDeviceContext, pCamera);
}
#endif