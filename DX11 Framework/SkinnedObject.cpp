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
	m_pSkinnedMesh->Update(fTimeElapsed);
}

void CSkinnedObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_pSkinnedMesh->UpdateBoneTransform(pd3dDeviceContext, )
}