#include "stdafx.h"
#include "SkinnedObject.h"


CSkinnedObject::CSkinnedObject()
{
}

CSkinnedObject::~CSkinnedObject()
{
}

void CSkinnedObject::SetMesh(CSkinnedMesh* mesh) 
{
	CGameObject::SetMesh(mesh);
	m_pSkinnedMesh = mesh;
}

void CSkinnedObject::SetAnimation(Animation::Character anim)
{
	for (auto list : m_animaitionTupleVector)
		if (get<0>(list) == anim) {
			m_pSkinnedMesh->SetClipName(get<1>(list));
			m_animType = get<2>(list);
			fTimePos = 0.0f;
			return;
		}
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
	fTimePos += fTimeElapsed;

	/*
	// Loop animation
	if (fTimePos > m_pSkinnedMesh->GetClipEndTime(m_pSkinnedMesh->GetClipName()))
		fTimePos = 0.0f;
		*/
	m_pSkinnedMesh->GetFinalTransforms(m_pSkinnedMesh->GetClipName(), fTimePos);

	switch (m_animType) {
	case AnimationType::Loop :
		if (fTimePos > m_pSkinnedMesh->GetClipEndTime(m_pSkinnedMesh->GetClipName()))
			fTimePos = 0.0f;
		break;
	case AnimationType::Once:
		if (fTimePos > m_pSkinnedMesh->GetClipEndTime(m_pSkinnedMesh->GetClipName())) {
			fTimePos = 0.0f;
			SetAnimation(Animation::eIdle);
		}

		break;
	}

	cout << fTimePos << endl;
}

void CSkinnedObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_pSkinnedMesh->UpdateConstantBuffer(pd3dDeviceContext);

	CGameObject::Render(pd3dDeviceContext, pCamera);
}
#endif