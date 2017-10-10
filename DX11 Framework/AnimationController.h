#pragma once

class CFbxModelSkinnedMesh;

struct AnimationTrack
{
	string m_strClipName;
	bool m_bEnable = false;
	float m_fSpeed = 1.0f;
	float m_fTimePos = 0.0f;
//	float m_fWeight = 0.0f;		// 현재 미사용

	AnimationTrack() {}
	AnimationTrack(string name) : m_strClipName(name) {}
};

class CAnimationController
{
	bool						m_isActive = true;
	bool						m_bIsBlending = false;
	float						m_fTimePos = 0.0f;
	CharacterParts				m_typeParts = CharacterParts::Defalut;
	CFbxModelSkinnedMesh*		m_pSkinnedMesh = nullptr;

	tuple<AnimationTag, AnimationTrack, AnimationType> m_prevAnimState;
	tuple<AnimationTag, AnimationTrack, AnimationType> m_currAnimState;
	vector<tuple<AnimationTag, AnimationTrack, AnimationType>> m_animaitionTupleVector;

public:
	CAnimationController() {};
	CAnimationController(CharacterParts type);
	virtual ~CAnimationController();

	void AddAnimation(tuple<AnimationTag, AnimationTrack, AnimationType> anim);

	void Update(float fDeltaTime);
	void UpdateTime(float fDeltaTime);
	void UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext);

	// ------ Get, Setter ------ //
	AnimationTag GetAnimEnum() const {return get<0>(m_currAnimState); }
	AnimationType GetAnimType() const { return get<2>(m_currAnimState); }

	void SetMesh(CFbxModelSkinnedMesh* mesh) { m_pSkinnedMesh = mesh; }
	void SetAnimation(AnimationTag anim, float speed = 1.0f);
	float GetTimePos() const { return m_fTimePos; }
	void SetTimePos(float timePos) { m_fTimePos = timePos; }
	bool GetActive() const { return m_isActive; }
	void SetActive(bool set) { m_isActive = set; }
};