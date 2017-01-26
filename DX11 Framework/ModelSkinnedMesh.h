#pragma once
//#include "Mesh.h"
#include "ModelMesh_FBX.h"

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
	string m_strAnimationName;
	vector<BoneData> m_boneDataVector;

	void Interpolate(float timePos, vector<XMFLOAT4X4>& boneTransforms) const;
	float GetClipStartTime() const;
	float GetClipEndTime() const;
	// Ű�����ӿ� ��� �ִϸ��̼��� ��� ���� ��� ����ϴ� ������ ����
	// float m_nStartFrameKey;
	// float m_nEndFrameKey;
};


class CSkinnedMesh : public CModelMesh_FBX
{
	vector<XMFLOAT4>	boneIndicesVector;
	vector<XMFLOAT4>	boneWeightsVector;

	vector<int>			boneHierarchyVector;
	vector<XMFLOAT4X4>	boneOffsetsVector;
	vector<XMMATRIX>	finalBoneVector;
		
	XMFLOAT4			*m_pboneIndices = nullptr;
	XMFLOAT4			*m_pboneWeights = nullptr;
	
	int					m_nBoneCount = 0;
	int					m_nAnimationClip = 0;
	string				m_strClipName;
	map<string, AnimationData> m_animationMap;
	
	ID3D11Buffer *m_pd3dBoneWeightBuffer = nullptr;
	ID3D11Buffer *m_pd3dBoneIndiceBuffer = nullptr;
	ID3D11Buffer *m_pd3dcbBones = nullptr;

	float m_fAnimationTime	= 0.0f;	// ���� AnimationTime (���������� 0.0333333f �� ����)	
	int m_nAnimationNum		= 0;	//1 ���� ������ �ִϸ��̼��� Set�ѹ� 0 Ȥ�� 1�̴�.
	int m_nStartFrameNum	= 0;	// ���� �ִϸ��̼��� �����ϴ� ������ �ѹ�
	int m_nMaxFrameNum		= 0;	// ���� ������ �ִϸ��̼��� �ִ� ������ ��.
	int m_nNowFrameNum		= 0;	// ���� �������� �ִϸ��̼��� ���� ������ 

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


	void SetAnimation();
	/*
	float GetFBXAnimationTime() { return m_fFBXAnimationTime; }
	int GetFBXAnimationNum() { return m_nFBXAnimationNum; }
	int GetFBXNowFrameNum() { return m_nFBXNowFrameNum; }
	int GetFBXMaxFrameNum() { return m_nFBXMaxFrameNum; }
	//void SetFBXAnimationTime(float fFBXAnimationTime) { m_fFBXAnimationTime = fFBXAnimationTime; }
	bool FBXFrameAdvance(float fTimeElapsed);
	void SetAnimation(int nFBXAnimationNum);

	unsigned int GetAnimationState() { return m_uiAnimationState; }
	void SetAnimationState(unsigned int CurrentState)
	{
		m_uiAnimationState = CurrentState;
	}
	void GetHandMatrix(D3DXMATRIX& mtxTransform) { mtxTransform = m_pd3dxmtxFinalBone[14] * m_d3dxmtxLocalTransform; }

	static void LoadAnimationSet();
	static void AnimationDestroy();
	*/
};

