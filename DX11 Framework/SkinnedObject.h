#pragma once
#include "Object.h"
#include "AnimationController.h"
#include "FbxModelSkinnedMesh.h"


class CSkinnedObject : public CGameObject
{
protected:
	CFbxModelSkinnedMesh*	m_pSkinnedMesh = nullptr;

	CAnimationController*	m_pUpperController = nullptr;
	CAnimationController*	m_pLowerController = nullptr;

public:
	CSkinnedObject();
	virtual ~CSkinnedObject();

protected:
	virtual void CreateAnimation() = 0;
public:
	virtual void CreateObjectData(ID3D11Device *pd3dDevice);

	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	void AddAnimation(tuple<AnimationTag, AnimationTrack, AnimationType> anim) { 
		m_pUpperController->AddAnimation(anim); 
		m_pLowerController->AddAnimation(anim);
	};
	// ------ Get, Setter ------ //
	CFbxModelSkinnedMesh* GetSkinnedMesh() const { return m_pSkinnedMesh; }
	AnimationTag GetAnimationEnum(CharacterParts parts) const {
		if (parts == CharacterParts::UpperBody)
			return m_pUpperController->GetAnimEnum();
		else
			return m_pLowerController->GetAnimEnum();
	}
	bool GetControllerActive(CharacterParts parts) const {
		if (parts == CharacterParts::UpperBody)
			return m_pUpperController->GetActive();
		else
			return m_pLowerController->GetActive();
	}
	void SetControllerActive(CharacterParts parts, bool set) const {
		if (parts == CharacterParts::UpperBody)
			m_pUpperController->SetActive(set);
		else
			m_pLowerController->SetActive(set);
	}
	void SetMesh(CFbxModelSkinnedMesh* mesh, int index = 0);
	void SetAnimation(AnimationTag anim, float speed = 1.0f);
	void SetAnimation(CharacterParts parts, AnimationTag anim, float speed = 1.0f);
};