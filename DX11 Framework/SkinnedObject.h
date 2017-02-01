#pragma once
#include "Object.h"
#include "AnimationController.h"
#include "ModelSkinnedMesh.h"

class CSkinnedObject : public CGameObject
{
protected:
	CSkinnedMesh*			m_pSkinnedMesh = nullptr;
	CAnimationController*	m_pAnimController = nullptr;

public:
	CSkinnedObject();
	virtual ~CSkinnedObject();

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	// ------ Get, Setter ------ //
	void SetMesh(CSkinnedMesh* mesh);
	void SetAnimation(Animation::Character anim);
};

