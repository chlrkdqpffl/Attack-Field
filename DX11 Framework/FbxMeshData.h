#pragma once

struct CKeyframeData
{
	float	m_fAnimationTime;

	XMFLOAT3 m_xmf3Scale;
	XMFLOAT3 m_xmf3Translate;
	XMFLOAT4 m_xmf4Quaternion;
};

struct CBoneData
{
	UINT m_nAnimaitionKeys;
	vector<CKeyframeData> m_vecKeyframeData;

	void Interpolate(float timePos, XMFLOAT4X4& boneTransforms) const;
	float GetStartTime() const { return m_vecKeyframeData.front().m_fAnimationTime; };
	float GetEndTime() const { return m_vecKeyframeData.back().m_fAnimationTime; };
};

struct CAnimationClip
{
	vector<CBoneData> m_vecBoneData;

	void Interpolate(float timePos, vector<XMFLOAT4X4>& boneTransforms) const;
	float GetClipStartTime() const;
	float GetClipEndTime() const;
};

// ===================================================================================================================== //
// =================================================== Fbx Mesh Data =================================================== //

struct CFbxMeshData
{
	string						m_fileName;

	// ------------- Basic Data ------------- //
	UINT						m_nVertexCount = 0;
	UINT						m_nIndexCount = 0;
	bool						m_bTangent = false;
	BoundingBox					m_boundingBox;

	vector<XMFLOAT3>			m_vecPosition;
	vector<XMFLOAT3>			m_vecNormal;
	vector<XMFLOAT3>			m_vecTangent;
	vector<XMFLOAT3>			m_vecIndex;
	vector<XMFLOAT2>			m_vecUV;

	// ------------ Animaiton Data ------------ //
	UINT						m_nBoneCount = 0;
	UINT						m_nAnimationCount = 0;

	map<string, CAnimationClip>	m_mapAnimationClip;
	vector<XMFLOAT4>			m_vecBoneIndices;
	vector<XMFLOAT4>			m_vecBoneWeights;
	vector<int>					m_vecBoneHierarchy;
	vector<XMFLOAT4X4>			m_vecBoneOffsets;

public:
	CFbxMeshData();
	~CFbxMeshData();

	bool LoadFbxModelDatafromFile(const string& fileName);
	bool LoadFbxModelSkinnedDatafromFile(const string& fileName);

	void LoadMeshData(ifstream& fin);
	void LoadBBoxData(ifstream& fin);
	void LoadBasicVertexData(ifstream& fin);
	void LoadSkinnedVertexData(ifstream& fin);
	void LoadIndexData(ifstream& fin);
	void LoadBoneData(ifstream& fin);
	void LoadAnimationData(ifstream& fin);
};