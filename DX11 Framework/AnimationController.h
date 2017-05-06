#pragma once

class CFbxModelSkinnedMesh;

struct AnimationTrack
{
	string m_strClipName;
	bool m_bEnable = false;
	float m_fSpeed = 1.0f;
	float m_fTimePos = 0.0f;
//	float m_fWeight = 0.0f;

	AnimationTrack() {}
	AnimationTrack(string name) : m_strClipName(name) {}
};

class CAnimationController
{
	float						m_fTimePos = 0.0f;
	CFbxModelSkinnedMesh*		m_pSkinnedMesh = nullptr;
	AnimationData::Parts		m_typeParts = AnimationData::Parts::Defalut;
	bool						m_bIsBlending = false;

	tuple<AnimationData::CharacterAnim, AnimationTrack, AnimationData::Type> m_prevAnimState;
	tuple<AnimationData::CharacterAnim, AnimationTrack, AnimationData::Type> m_currAnimState;
	vector<tuple<AnimationData::CharacterAnim, AnimationTrack, AnimationData::Type>> m_animaitionTupleVector;

public:
	CAnimationController() {};
	CAnimationController(AnimationData::Parts type);
	virtual ~CAnimationController();

	void AddAnimation(tuple<AnimationData::CharacterAnim, AnimationTrack, AnimationData::Type> anim);

	void Update(float fDeltaTime);
	void UpdateTime(float fDeltaTime);
	void UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	// ------ Get, Setter ------ //
	AnimationData::CharacterAnim GetAnimEnum() const {return get<0>(m_currAnimState); }
	AnimationData::Type GetAnimType() const { return get<2>(m_currAnimState); }

	void SetMesh(CFbxModelSkinnedMesh* mesh) { m_pSkinnedMesh = mesh; }
	void SetAnimation(AnimationData::CharacterAnim anim, float speed = 1.0f);
	float GetTimePos() const { return m_fTimePos; }
	void SetTimePos(float timePos) { m_fTimePos = timePos; }
};