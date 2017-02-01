#pragma once

struct AnimationTrack
{
	string m_strClipName;
	bool m_bEnable = false;
	float m_fSpeed = 0.0f;
	float m_fPosition = 0.0f;
	float m_fWeight = 0.0f;
};

class CAnimationController
{
	float						m_fTimePos = 0.0f;
	int							m_nAnimationCount = 0;
	CSkinnedMesh*				m_pSkinnedMesh = nullptr;
	vector<AnimationTrack>		m_animTrackVector;


	Animation::Character		m_animState;
	AnimationType::Type			m_animType = AnimationType::Loop;
	vector<tuple<Animation::Character, string, AnimationType::Type>> m_animaitionTupleVector;
public:
	CAnimationController();
	virtual ~CAnimationController();

	void AddAnimation(tuple<Animation::Character, string, AnimationType::Type> anim);

	void Update(float fTimeElapsed);
	void UpdateTime(float fTimeElapsed);
	void UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	// ------ Get, Setter ------ //
	Animation::Character GetAnimState() const { return  m_animState; }

	void SetMesh(CSkinnedMesh* mesh);
	void SetAnimation(Animation::Character anim);
};

