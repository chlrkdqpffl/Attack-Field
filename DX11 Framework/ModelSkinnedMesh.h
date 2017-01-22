#pragma once
//#include "Mesh.h"
#include "ModelMesh_FBX.h"

struct VS_CB_SKINNED
{
	XMFLOAT4X4	m_xmMtxBoneTransform[MAXBONECOUNT];
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

	// 키프레임에 모든 애니메이션을 담고 있을 경우 사용하는 것으로 추정
	// float m_nStartFrameKey;
	// float m_nEndFrameKey;
};


class CSkinnedMesh : public CModelMesh_FBX
{
	vector<XMFLOAT4>	boneIndicesVector;
	vector<XMFLOAT4>	boneWeightsVector;

	vector<int>			boneHierarchyVector;
	vector<XMFLOAT4X4>	boneOffsetsVector;
	vector<XMMATRIX>	SQTTransformVector;
	vector<XMMATRIX>	finalBoneVector;
		

	XMFLOAT4			*m_pboneIndices = nullptr;
	XMFLOAT4			*m_pboneWeights = nullptr;
	
	int					m_nBoneCount = 0;
	int					m_nAnimationClip = 0;
	map<string, AnimationData> m_animationMap;
	
	ID3D11Buffer *m_pd3dBoneWeightBuffer;
	ID3D11Buffer *m_pd3dBoneIndiceBuffer;
	ID3D11Buffer *m_pd3dcbBones;


	float m_fAnimationTime;	// 모델의 AnimationTime (공통적으로 0.0333333f 씩 증가)	
	int m_nAnimationNum;	// 모델이 실행할 애니메이션의 Set넘버 0 혹은 1이다.
	int m_nStartFrameNum;	// 모델의 애니메이션이 시작하는 프레임 넘버
	int m_nMaxFrameNum;		// 모델이 실행할 애니메이션의 최대 프레임 수.
	int m_nNowFrameNum;		// 모델이 진행중인 애니메이션의 현재 프레임 값.
	/*
	D3DXMATRIX *m_pd3dxmtxSQTTransform;//이것도 필요하다.
	D3DXMATRIX *m_pd3dxmtxFinalBone;//최종행렬 배열(이건 필요하다.)

	D3DXMATRIX m_d3dxmtxLocalTransform;//개별적으로 필요한 로컬 변환 행렬

	float m_fFBXAnimationTime;	// 모델의 AnimationTime (공통적으로 0.0333333f 씩 증가)	
	int m_nFBXAnimationNum;		// 모델이 실행할 애니메이션의 Set넘버 0 혹은 1이다.

	int m_nFBXStartFrameNum;	// 모델의 애니메이션이 시작하는 프레임 넘버
	int m_nFBXMaxFrameNum;		// 모델이 실행할 애니메이션의 최대 프레임 수.

	int m_nFBXNowFrameNum;		// 모델이 진행중인 애니메이션의 현재 프레임 값.

	unsigned int m_uiAnimationState;
	*/
public:

	CSkinnedMesh(ID3D11Device *pd3dDevice, const string&, float size = 1.0f);
	virtual ~CSkinnedMesh();

	virtual void Initialize(ID3D11Device *pd3dDevice);
	virtual bool LoadFBXfromFile(const string& fileName);

	void CreateConstantBuffer(ID3D11Device *pd3dDevice);
//	void UpdateConstantBuffers();
	void ReleaseConstantBuffer();


	// 해당 프레임의 SR(Q)T 회전이 반영된 행렬을 반환
	void MakeBoneMatrix(int nNowframe, int nAnimationNum, int nBoneNum);
	// 상수 버퍼로 전달할 최종 본 행렬을 구한다.
	void UpdateBoneTransform(ID3D11DeviceContext *pd3dDeviceContext, int nAnimationNum, int nNowFrame);
	void Update(float fTimeElapsed);
	/*
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
	*/
};

