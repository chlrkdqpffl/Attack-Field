#pragma once
#include "Object.h"
#include "ModelSkinnedMesh.h"

class CSkinnedObject : public CGameObject
{
protected:
	CSkinnedMesh*		m_pSkinnedMesh = nullptr;
	float				fTimePos = 0.0f;

	AnimationType::Type	m_animType = AnimationType::Loop;
	vector<tuple<Animation::Character, string, AnimationType::Type>> m_animaitionTupleVector;
public:
	CSkinnedObject();
	virtual ~CSkinnedObject();

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	// ------ Get, Setter ------ //
	void SetMesh(CSkinnedMesh* mesh);
	void SetAnimation(Animation::Character anim);
};

