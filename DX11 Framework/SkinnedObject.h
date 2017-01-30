#pragma once
#include "Object.h"
#include "ModelSkinnedMesh.h"

class CAnimationController
{
	float m_fTime;
	int m_nAnimationCount;

};



class CSkinnedObject : public CGameObject
{
protected:
	CSkinnedMesh*		m_pSkinnedMesh = nullptr;
	float				fTimePos = 0.0f;

	Animation::Character m_animState;
	AnimationType::Type	m_animType = AnimationType::Loop;
	vector<tuple<Animation::Character, string, AnimationType::Type>> m_animaitionTupleVector;
public:
	CSkinnedObject();
	virtual ~CSkinnedObject();

	void UpdateTime(float& fTimeElapsed);

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	// ------ Get, Setter ------ //
	Animation::Character GetAnimState() const { return m_animState;	}

	void SetMesh(CSkinnedMesh* mesh);
	void SetAnimation(Animation::Character anim);
};

