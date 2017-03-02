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

protected:
	virtual void CreateAnimation() {};

public:
	virtual void CreateObjectData(ID3D11Device *pd3dDevice);

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	// ------ Get, Setter ------ //
	void SetMesh(CSkinnedMesh* mesh);
	void SetAnimation(AnimationData::CharacterAnim anim, float speed = 1.0f);
};

