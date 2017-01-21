#pragma once
//#include "Mesh.h"
#include "ModelMesh_FBX.h"

struct VS_CB_SKINNED
{
	D3DXMATRIX	m_d3dxmtxBone[30];
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
	vector<KeyframeData> m_keyframeDataVector;
};

struct AnimationData
{
	string m_strAnimationName;
	int m_nAnimaitionKeys;
	vector<BoneData> m_boneDataVector;

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
	vector<XMFLOAT4X4>	SQTTransformVector;
	vector<XMFLOAT4X4>	finalBoneVector;
		

	XMFLOAT4			*m_pboneIndices = nullptr;
	XMFLOAT4			*m_pboneWeights = nullptr;
	
	map<string, AnimationData> m_animationMap;
	




//	static BoneAnimationData **s_ppBoneAnimationData;//�̰� static���� ������ �� �ִ� ������
	static int s_nBoneCount;		//18
	static int s_nAnimationClip;	//2
	static D3DXMATRIX *s_pd3dxmtxBoneOffsets;

	D3DXMATRIX *m_pd3dxmtxSQTTransform;//�̰͵� �ʿ��ϴ�.
	D3DXMATRIX *m_pd3dxmtxFinalBone;//������� �迭(�̰� �ʿ��ϴ�.)

	D3DXMATRIX m_d3dxmtxLocalTransform;//���������� �ʿ��� ���� ��ȯ ���

	// i�� ������ �θ� ����(parentIndex)�� ��´�.
	// i�� ����� �ִϸ��̼� Ŭ���� i��° BoneAnimation �ν��Ͻ��� ����.
	UINT *m_pBoneHierarchy;


	ID3D11Buffer *m_pd3dWeightBuffer;
	ID3D11Buffer *m_pd3dBoneIndiceBuffer;

	// ���� �������
	ID3D11Buffer *m_pd3dcbBones;

	float m_fFBXAnimationTime;	// ���� AnimationTime (���������� 0.0333333f �� ����)	
	int m_nFBXAnimationNum;		// ���� ������ �ִϸ��̼��� Set�ѹ� 0 Ȥ�� 1�̴�.

	int m_nFBXStartFrameNum;	// ���� �ִϸ��̼��� �����ϴ� ������ �ѹ�
	int m_nFBXMaxFrameNum;		// ���� ������ �ִϸ��̼��� �ִ� ������ ��.

	int m_nFBXNowFrameNum;		// ���� �������� �ִϸ��̼��� ���� ������ ��.

	unsigned int m_uiAnimationState;
public:

	CSkinnedMesh(ID3D11Device *pd3dDevice, const string&, float size = 1.0f);
	virtual ~CSkinnedMesh();

	virtual void Initialize(ID3D11Device *pd3dDevice);
	virtual bool LoadFBXfromFile(const string& fileName);
	// �ش� �������� SR(Q)T ȸ���� �ݿ��� ����� ��ȯ
	void MakeBoneMatrix(int nNowframe, int nAnimationNum, int nBoneNum, D3DXMATRIX& BoneMatrix);
	// ��� ���۷� ������ ���� �� ����� ���Ѵ�.
	void UpdateBoneTransform(ID3D11DeviceContext *pd3dDeviceContext, int nAnimationNum, int nNowFrame);
	// ���� ��� ���� ����
	void CreateConstantBuffer(ID3D11Device *pd3dDevice);

	float GetFBXAnimationTime() { return m_fFBXAnimationTime; }
	int GetFBXAnimationNum() { return m_nFBXAnimationNum; }
	int GetFBXNowFrameNum() { return m_nFBXNowFrameNum; }
	int GetFBXMaxFrameNum() { return m_nFBXMaxFrameNum; }
	//void SetFBXAnimationTime(float fFBXAnimationTime) { m_fFBXAnimationTime = fFBXAnimationTime; }
	bool FBXFrameAdvance(float fTimeElapsed);
	void SetAnimation(int nFBXAnimationNum);

	void WalkAnimationSet();
	void RunAnimationSet();
	void ShootRifle();
	void ShootPistol();
	void IdleSet();
	void HelloSet();
	void ShootSniper();
	void ReloadRifle();
	void ZombieWalk();
	unsigned int GetAnimationState() { return m_uiAnimationState; }
	void SetAnimationState(unsigned int CurrentState)
	{
		m_uiAnimationState = CurrentState;
	}
	void GetHandMatrix(D3DXMATRIX& mtxTransform) { mtxTransform = m_pd3dxmtxFinalBone[14] * m_d3dxmtxLocalTransform; }

	static void LoadAnimationSet();
	static void AnimationDestroy();
};

