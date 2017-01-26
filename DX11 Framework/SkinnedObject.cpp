#include "stdafx.h"
#include "SkinnedObject.h"


CSkinnedObject::CSkinnedObject()
{
}

CSkinnedObject::~CSkinnedObject()
{
}

void CSkinnedObject::Update(float fTimeElapsed)
{
	fTimePos += fTimeElapsed;

	// Loop animation
	if (fTimePos > m_pSkinnedMesh->GetClipEndTime(m_pSkinnedMesh->GetClipName()))
		fTimePos = 0.0f;

	m_pSkinnedMesh->GetFinalTransforms(m_pSkinnedMesh->GetClipName(), fTimePos);
//	m_pSkinnedMesh->Update(fTimeElapsed);

}

void CSkinnedObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
//	m_pSkinnedMesh->UpdateBoneTransform(pd3dDeviceContext);
	m_pSkinnedMesh->UpdateConstantBuffer(pd3dDeviceContext);

	CGameObject::Render(pd3dDeviceContext, pCamera);
}