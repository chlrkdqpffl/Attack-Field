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

	void AddAnimation(tuple<AnimationData::CharacterAnim, AnimationTrack, AnimationData::Type> anim) { 
		m_pUpperController->AddAnimation(anim); 
		m_pLowerController->AddAnimation(anim);
	};
	// ------ Get, Setter ------ //
	CFbxModelSkinnedMesh* GetSkinnedMesh() const { return m_pSkinnedMesh; }
	AnimationData::CharacterAnim GetAnimationEnum(AnimationData::Parts parts) const {
		if (parts == AnimationData::Parts::UpperBody)
			return m_pUpperController->GetAnimEnum();
		else
			return m_pLowerController->GetAnimEnum();
	}
	bool GetControllerActive(AnimationData::Parts parts) const {
		if (parts == AnimationData::Parts::UpperBody)
			return m_pUpperController->GetActive();
		else
			return m_pLowerController->GetActive();
	}
	void SetControllerActive(AnimationData::Parts parts, bool set) const {
		if (parts == AnimationData::Parts::UpperBody)
			m_pUpperController->SetActive(set);
		else
			m_pLowerController->SetActive(set);
	}
	void SetMesh(CFbxModelSkinnedMesh* mesh);
	void SetAnimation(AnimationData::CharacterAnim anim, float speed = 1.0f);
	void SetAnimation(AnimationData::Parts parts, AnimationData::CharacterAnim anim, float speed = 1.0f);
};