#pragma once
#include "FbxModelMesh.h"
#include "AnimationController.h"

struct VS_CB_SKINNED
{
	XMFLOAT4X4	m_mtxBoneTransform[MAXBONECOUNT];
};

struct BlendingInfo
{
	AnimationTrack			m_prevAnimation;
	AnimationTrack			m_CurrAnimation;
	float					m_fTimePos = 0.0f;
	AnimationData::MultiAnimation	m_typeParts = AnimationData::MultiAnimation::Defalut;

	BlendingInfo(AnimationTrack prev, AnimationTrack curr, float timePos, AnimationData::MultiAnimation type)
		: m_prevAnimation(prev), m_CurrAnimation(curr), m_fTimePos(timePos), m_typeParts(type)
	{
	}
};

class CFbxModelSkinnedMesh : public CFbxModelMesh
{
protected:
	vector<XMMATRIX>			m_vecFinalBone;
	float						m_fPitch = 0;
	XMFLOAT4					*m_pboneIndices = nullptr;
	XMFLOAT4					*m_pboneWeights = nullptr;
	UINT						m_nBodyBoundaryIndex = 32;			// 상체와 하체를 구분하는 인덱스. 하체의 첫번째 뼈대 인덱스를 의미

	string						m_strClipName;

	ID3D11Buffer				*m_pd3dBoneWeightBuffer = nullptr;
	ID3D11Buffer				*m_pd3dBoneIndiceBuffer = nullptr;
	ID3D11Buffer				*m_pd3dcbBones = nullptr;

public:
	CFbxModelSkinnedMesh() {};
	CFbxModelSkinnedMesh(ID3D11Device *pd3dDevice, const MeshTag meshTag, const XMFLOAT3 size = XMFLOAT3(1.f, 1.f, 1.f));
	virtual ~CFbxModelSkinnedMesh();

	virtual void Initialize(ID3D11Device *pd3dDevice, bool isCalcTangent = false);
	
	void CreateConstantBuffer(ID3D11Device *pd3dDevice);
	void UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext);
	void ReleaseConstantBuffer();

	void Interpolate_Blending(const CAnimationClip& dataA, bool& enable, const CAnimationClip& dataB, float timePos, vector<XMFLOAT4X4>& boneTransforms) const;
	void Interpolate_Blending(const CAnimationClip& dataA, bool& enable, const CAnimationClip& dataB, float timePos, AnimationData::MultiAnimation type, vector<XMFLOAT4X4>& boneTransforms) const;

	// ----- Get, Setter ----- //
	void CalcFinalTransformsBlending(BlendingInfo& blendingInfo, bool& bIsBlending);
	void GetFinalTransformsBlending(AnimationTrack& prevAnim, const AnimationTrack& currAnim, const float& fTimePos);

	float GetClipStartTime(const string& clipName) const;
	float GetClipEndTime(const string& clipName) const;
	const string& GetClipName() const { return m_strClipName; }
	const map<string, CAnimationClip>& GetAnimMap() const { return m_meshData.m_mapAnimationClip; }
	XMMATRIX GetFinalBoneMtx(UINT index) const { return m_vecFinalBone[index]; }


	void SetClipName(string name) { m_strClipName = name; }
	void SetPitch(float pitch) { m_fPitch = pitch; }
};