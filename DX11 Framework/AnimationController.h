#pragma once

struct AnimationTrack
{
	string m_strClipName;
	bool m_bEnable = false;
	float m_fSpeed = 0.0f;
	float m_fPosition = 0.0f;
	float m_fWeight = 0.0f;

	AnimationTrack() {}
	AnimationTrack(string name) : m_strClipName(name) {}
};

class CAnimationController
{
	float						m_fTimePos = 0.0f;
	CSkinnedMesh*				m_pSkinnedMesh = nullptr;

	tuple<Animation::Character, AnimationTrack, AnimationType::Type> m_prevAnimState;
	tuple<Animation::Character, AnimationTrack, AnimationType::Type> m_currAnimState;
	vector<tuple<Animation::Character, AnimationTrack, AnimationType::Type>> m_animaitionTupleVector;

public:
	CAnimationController();
	virtual ~CAnimationController();

	void AddAnimation(tuple<Animation::Character, AnimationTrack, AnimationType::Type> anim);

	void Update(float fTimeElapsed);
	void UpdateTime(float fTimeElapsed);
	void UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	// ------ Get, Setter ------ //
	Animation::Character GetAnimEnum() const {return get<0>(m_currAnimState); }
	AnimationType::Type GetAnimType() const { return get<2>(m_currAnimState); }

	void SetMesh(CSkinnedMesh* mesh);
	void SetAnimation(Animation::Character anim);
};

