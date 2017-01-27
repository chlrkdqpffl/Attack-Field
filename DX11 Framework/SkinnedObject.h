#pragma once
#include "Object.h"
#include "ModelSkinnedMesh.h"

class CSkinnedObject : public CGameObject
{
protected:
	CSkinnedMesh*		m_pSkinnedMesh = nullptr;
	vector<pair<Animation::Character, string>> m_animaitionListVector;
	float				fTimePos = 0.0f;

	AnimationType::Type	m_animType = AnimationType::Loop;
	vector<tuple<Animation::Character, string, AnimationType::Type>> m_animaitionTupleListVector;
public:
	CSkinnedObject();
	virtual ~CSkinnedObject();

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	void SetMesh(CSkinnedMesh* mesh) {
		CGameObject::SetMesh(mesh);
		m_pSkinnedMesh = mesh;
	}

	void SetAnimation(Animation::Character anim);
};

