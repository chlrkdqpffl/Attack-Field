#pragma once
#include "FbxModelMesh.h"

struct AnimationTrack;

struct VS_CB_SKINNED
{
	XMFLOAT4X4	m_mtxBoneTransform[MAXBONECOUNT];
};

class CFbxModelSkinnedMesh : public CFbxModelMesh
{
protected:
	vector<XMMATRIX>			m_vecFinalBone;

	XMFLOAT4					*m_pboneIndices = nullptr;
	XMFLOAT4					*m_pboneWeights = nullptr;

	string						m_strClipName;

	ID3D11Buffer				*m_pd3dBoneWeightBuffer = nullptr;
	ID3D11Buffer				*m_pd3dBoneIndiceBuffer = nullptr;
	ID3D11Buffer				*m_pd3dcbBones = nullptr;

public:

	CFbxModelSkinnedMesh(ID3D11Device *pd3dDevice, const MeshTag meshTag, const XMFLOAT3 size = XMFLOAT3(1.f, 1.f, 1.f));
	virtual ~CFbxModelSkinnedMesh();

	virtual void Initialize(ID3D11Device *pd3dDevice, bool isCalcTangent = false);
	
	void CreateConstantBuffer(ID3D11Device *pd3dDevice);
	void UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext);
	void ReleaseConstantBuffer();

	void Interpolate_Blending(const CAnimationClip& dataA, bool& enable, const CAnimationClip& dataB, float timePos, vector<XMFLOAT4X4>& boneTransforms) const;

	// ----- Get, Setter ----- //
	void GetFinalTransforms(const string& clipName, const float& fTimePos);
	void GetFinalTransformsBlending(AnimationTrack& prevAnim, const AnimationTrack& currAnim, const float& fTimePos);
	
	float GetClipStartTime(const string& clipName) const;
	float GetClipEndTime(const string& clipName) const;
	const string& GetClipName() const { return m_strClipName; }
	const map<string, CAnimationClip>& GetAnimMap() const { return m_meshData.m_mapAnimationClip; }
	XMFLOAT4X4 GetOffsetMtx(UINT index) const { return m_meshData.m_vecBoneOffsets[index]; }
	XMMATRIX GetFinalBoneMtx(UINT index) const { return m_vecFinalBone[index]; }

	void SetClipName(string name) { m_strClipName = name; }
};