#pragma once
//#include "Mesh.h"
#include "ModelMesh_FBX.h"

namespace AnimationType
{
	enum Type {
		Loop, Once, PingPong
	};
}

struct VS_CB_SKINNED
{
	XMFLOAT4X4	m_mtxBoneTransform[MAXBONECOUNT];
};

struct KeyframeData
{
	float	m_fAnimationTime;

	XMFLOAT3 m_xmf3Scale;
	XMFLOAT3 m_xmf3Translate;
	XMFLOAT4 m_xmf4Quaternion;
};

struct BoneData
{
	int m_nAnimaitionKeys;
	vector<KeyframeData> m_keyframeDataVector;

	void Interpolate(float timePos, XMFLOAT4X4& boneTransforms) const;
	float GetStartTime() const { return m_keyframeDataVector.front().m_fAnimationTime; };
	float GetEndTime() const { return m_keyframeDataVector.back().m_fAnimationTime; };
};

struct AnimationData
{
	vector<BoneData> m_boneDataVector;

	void Interpolate(float timePos, vector<XMFLOAT4X4>& boneTransforms) const;
	float GetClipStartTime() const;
	float GetClipEndTime() const;

	// 키프레임에 모든 애니메이션을 담고 있을 경우 사용하는 것으로 추정
	// float m_nStartFrameKey;
	// float m_nEndFrameKey;
};


class CSkinnedMesh : public CModelMesh_FBX
{
protected:
	vector<XMFLOAT4>			boneIndicesVector;
	vector<XMFLOAT4>			boneWeightsVector;

	vector<int>					boneHierarchyVector;
	vector<XMFLOAT4X4>			boneOffsetsVector;
	vector<XMMATRIX>			finalBoneVector;

	XMFLOAT4					*m_pboneIndices = nullptr;
	XMFLOAT4					*m_pboneWeights = nullptr;

	int							m_nBoneCount = 0;
	int							m_nAnimationClip = 0;
	string						m_strClipName;
	map<string, AnimationData>	m_animationMap;

	ID3D11Buffer				 *m_pd3dBoneWeightBuffer = nullptr;
	ID3D11Buffer				 *m_pd3dBoneIndiceBuffer = nullptr;
	ID3D11Buffer				 *m_pd3dcbBones = nullptr;

	float						m_fAnimationTime = 0.0f;
	int							m_nAnimationNum = 0;
	int							m_nStartFrameNum = 0;
	int							m_nMaxFrameNum = 0;
	int							m_nNowFrameNum = 0;	

public:

	CSkinnedMesh(ID3D11Device *pd3dDevice, const string&, float size = 1.0f);
	virtual ~CSkinnedMesh();

	virtual void Initialize(ID3D11Device *pd3dDevice);
	virtual bool LoadFBXfromFile(const string& fileName);

	void CreateConstantBuffer(ID3D11Device *pd3dDevice);
	void UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext);
	void ReleaseConstantBuffer();

	void MakeBoneMatrix(int nNowframe, int nBoneNum, vector<XMMATRIX>&) const;

	void UpdateBoneTransform(ID3D11DeviceContext *pd3dDeviceContext);
	void Update(float fTimeElapsed);

	// ----- Get, Setter ----- //
	void GetFinalTransforms(const string& clipName, float fTimePos);
	float GetClipStartTime(const string& clipName) const;
	float GetClipEndTime(const string& clipName) const;
	const string& GetClipName() const { return m_strClipName; }

	void SetClipName(string name) { m_strClipName = name; }
};