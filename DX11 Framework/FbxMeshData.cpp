#include "stdafx.h"
#include "FbxMeshData.h"

void CBoneData::Interpolate(float fTimePos, XMFLOAT4X4& boneTransforms) const
{
	if (fTimePos <= m_vecKeyframeData.front().m_fAnimationTime)
	{
		XMVECTOR S = XMLoadFloat3(&m_vecKeyframeData.front().m_xmf3Scale);
		XMVECTOR P = XMLoadFloat3(&m_vecKeyframeData.front().m_xmf3Translate);
		XMVECTOR Q = XMLoadFloat4(&m_vecKeyframeData.front().m_xmf4Quaternion);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&boneTransforms, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else if (m_vecKeyframeData.back().m_fAnimationTime <= fTimePos)
	{
		XMVECTOR S = XMLoadFloat3(&m_vecKeyframeData.back().m_xmf3Scale);
		XMVECTOR P = XMLoadFloat3(&m_vecKeyframeData.back().m_xmf3Translate);
		XMVECTOR Q = XMLoadFloat4(&m_vecKeyframeData.back().m_xmf4Quaternion);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&boneTransforms, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else
	{
		for (UINT i = 0; i < m_vecKeyframeData.size() - 1; ++i)
		{
			if (m_vecKeyframeData[i].m_fAnimationTime <= fTimePos && fTimePos <= m_vecKeyframeData[i + 1].m_fAnimationTime)
			{
				float lerpPercent = (fTimePos - m_vecKeyframeData[i].m_fAnimationTime) / (m_vecKeyframeData[i + 1].m_fAnimationTime - m_vecKeyframeData[i].m_fAnimationTime);

				XMVECTOR s0 = XMLoadFloat3(&m_vecKeyframeData[i].m_xmf3Scale);
				XMVECTOR s1 = XMLoadFloat3(&m_vecKeyframeData[i + 1].m_xmf3Scale);

				XMVECTOR p0 = XMLoadFloat3(&m_vecKeyframeData[i].m_xmf3Translate);
				XMVECTOR p1 = XMLoadFloat3(&m_vecKeyframeData[i + 1].m_xmf3Translate);

				XMVECTOR q0 = XMLoadFloat4(&m_vecKeyframeData[i].m_xmf4Quaternion);
				XMVECTOR q1 = XMLoadFloat4(&m_vecKeyframeData[i + 1].m_xmf4Quaternion);

				XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
				XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
				XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);
				XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

				XMStoreFloat4x4(&boneTransforms, XMMatrixAffineTransformation(S, zero, Q, P));

				break;
			}
		}
	}
}

void CAnimationClip::Interpolate(float fTimePos, vector<XMFLOAT4X4>& boneTransforms) const
{
	for (UINT i = 0; i < m_vecBoneData.size(); ++i)
	{
		if (0 < m_vecBoneData[i].m_nAnimaitionKeys)
			m_vecBoneData[i].Interpolate(fTimePos, boneTransforms[i]);
		else
		{
			XMMATRIX identityMtx = XMMatrixIdentity();
			XMStoreFloat4x4(&boneTransforms[i], identityMtx);
		}
	}
}

float CAnimationClip::GetClipStartTime() const
{
	float t = FLT_MAX;

	for (UINT i = 0; i < m_vecBoneData.size(); ++i) {
		if (m_vecBoneData[i].m_vecKeyframeData.size() == 0) continue;
		t = fmin(t, m_vecBoneData[i].GetStartTime());
	}
	return t;
}

float CAnimationClip::GetClipEndTime() const
{
	float t = 0.0f;

	for (UINT i = 0; i < m_vecBoneData.size(); ++i) {
		if (m_vecBoneData[i].m_vecKeyframeData.size() == 0) continue;
		t = fmax(t, m_vecBoneData[i].GetEndTime());
	}

	return t;
}

// ===================================================================================================================== //
// =================================================== Fbx Mesh Data =================================================== //

CFbxMeshData::CFbxMeshData()
{
}

CFbxMeshData::~CFbxMeshData()
{
}

bool CFbxMeshData::LoadFbxModelDatafromFile(const string& fileName)
{
	m_fileName = fileName;
	ifstream fin(fileName);
	//ifstream fin(fileName, ios::binary);

	if (!fin.is_open())
		return false;

	UINT meshCount = 0;
	
	string buf;

	while (!fin.eof()) {
		fin >> buf; // [FBX_META_DATA]
		fin >> buf >> meshCount;

		// [MESH_DATA]
		LoadMeshData(fin);
		
		// [BBox_DATA]
		LoadBBoxData(fin);

		// [VERTEX_DATA]
		LoadBasicVertexData(fin);

		// [INDEX_DATA]
		LoadIndexData(fin);

		// End
		fin >> buf;
	}
	fin.close();

	return true;
}

bool CFbxMeshData::LoadFbxModelSkinnedDatafromFile(const string& fileName)
{
	m_fileName = fileName;
	ifstream fin(fileName);

	if (!fin.is_open())
		return false;

	UINT meshCount = 0;
	string buf;

	while (!fin.eof()) {
		fin >> buf; // [FBX_META_DATA]
		fin >> buf >> meshCount;

		// 다중 메쉬의 경우 애니메이션에 문제가 발생
		assert(meshCount == 1);

		// [MESH_DATA]
		LoadMeshData(fin);

		// [BBox_DATA]
		LoadBBoxData(fin);

		// [VERTEX_DATA]
		LoadSkinnedVertexData(fin);

		// [INDEX_DATA]
		LoadIndexData(fin);

		// [BONE_HIERARCHY]
		// [OFFSET_MATRIX]
		LoadBoneData(fin);

		// [ANIMATION_CLIPS]
		LoadAnimationData(fin);

		// End
		fin >> buf;
		fin >> buf;

	}
	fin.close();

	return true;
}

void CFbxMeshData::LoadMeshData(ifstream& fin)
{
	string buf;

	fin >> buf; // [MESH_DATA]
	fin >> m_nVertexCount;
	{
		m_vecPosition.reserve(m_nVertexCount);
		m_vecNormal.reserve(m_nVertexCount);
		if (m_bTangent)
			m_vecTangent.reserve(m_nVertexCount);
		m_vecUV.reserve(m_nVertexCount);
	}
	fin >> m_nIndexCount;
	{
		m_vecIndex.reserve(m_nIndexCount / 3);
	}
	fin >> m_nBoneCount;
	fin >> m_nAnimationCount;

	fin >> buf; // isTangent
	if (buf == "true")
		m_bTangent = true;
	else
		m_bTangent = false;

}

void CFbxMeshData::LoadBBoxData(ifstream& fin)
{
	string buf;
	XMFLOAT3 bBoxCenter, bBoxExtents;

	fin >> buf; // [BBox_DATA]
	fin >> buf; // Center
	fin >> bBoxCenter.x >> bBoxCenter.y >> bBoxCenter.z;
	fin >> buf; // Extents
	fin >> bBoxExtents.x >> bBoxExtents.y >> bBoxExtents.z;
	
	m_boundingBox.Center = bBoxCenter;
	m_boundingBox.Extents = bBoxExtents;
}

void CFbxMeshData::LoadBasicVertexData(ifstream& fin)
{
	string buf;
	XMFLOAT3 pos, normal, tangent;
	XMFLOAT2 uv;

	fin >> buf; // [VERTEX_DATA]
	if (m_bTangent) {
		for (UINT i = 0; i < m_nVertexCount; ++i) {
			fin >> pos.x >> pos.y >> pos.z;
			fin >> normal.x >> normal.y >> normal.z;
			fin >> tangent.x >> tangent.y >> tangent.z;
			fin >> uv.x >> uv.y;

			m_vecPosition.push_back(pos);
			m_vecNormal.push_back(normal);
			m_vecTangent.push_back(tangent);
			m_vecUV.push_back(uv);
		}
	}
	else
	{
		for (UINT i = 0; i < m_nVertexCount; ++i) {
			fin >> pos.x >> pos.y >> pos.z;
			fin >> normal.x >> normal.y >> normal.z;
			fin >> uv.x >> uv.y;

			m_vecPosition.push_back(pos);
			m_vecNormal.push_back(normal);
			m_vecUV.push_back(uv);
		}
	}
}

void CFbxMeshData::LoadSkinnedVertexData(ifstream& fin)
{
	string buf;
	XMFLOAT3 pos, normal, tangent;
	XMFLOAT2 uv;
	XMFLOAT4 boneIndex, boneWeight;

	fin >> buf; // [VERTEX_DATA]
	if (m_bTangent) {
		for (UINT i = 0; i < m_nVertexCount; ++i) {
			fin >> pos.x >> pos.y >> pos.z;
			fin >> normal.x >> normal.y >> normal.z;
			fin >> tangent.x >> tangent.y >> tangent.z;
			fin >> uv.x >> uv.y;
			fin >> boneIndex.x >> boneIndex.y >> boneIndex.z >> boneIndex.w;
			fin >> boneWeight.x >> boneWeight.y >> boneWeight.z >> boneWeight.w;

			m_vecPosition.push_back(pos);
			m_vecNormal.push_back(normal);
			m_vecTangent.push_back(tangent);
			m_vecUV.push_back(uv);
			m_vecBoneIndices.push_back(boneIndex);
			m_vecBoneWeights.push_back(boneWeight);
		}
	}
	else {
		for (UINT i = 0; i < m_nVertexCount; ++i) {
			fin >> pos.x >> pos.y >> pos.z;
			fin >> normal.x >> normal.y >> normal.z;
			fin >> uv.x >> uv.y;
			fin >> boneIndex.x >> boneIndex.y >> boneIndex.z >> boneIndex.w;
			fin >> boneWeight.x >> boneWeight.y >> boneWeight.z >> boneWeight.w;

			m_vecPosition.push_back(pos);
			m_vecNormal.push_back(normal);
			m_vecUV.push_back(uv);
			m_vecBoneIndices.push_back(boneIndex);
			m_vecBoneWeights.push_back(boneWeight);
		}
	}
}

void CFbxMeshData::LoadIndexData(ifstream& fin)
{
	string buf;
	XMFLOAT3 index;

	fin >> buf; // [INDEX_DATA]
	for (UINT i = 0; i < m_nIndexCount / 3; ++i) {
		fin >> index.x >> index.y >> index.z;

		m_vecIndex.push_back(index);
	}
}

void CFbxMeshData::LoadBoneData(ifstream& fin)
{
	string buf;
	int boneHierarchy;
	XMFLOAT4X4 boneOffset;

	fin >> buf; // [BONE_HIERARCHY]
	for (UINT i = 0; i < m_nBoneCount; ++i) {
		fin >> boneHierarchy;

		m_vecBoneHierarchy.push_back(boneHierarchy);
	}

	fin >> buf; // [OFFSET_MATRIX]
	for (UINT i = 0; i < m_nBoneCount; ++i) {
		fin >> boneOffset._11 >> boneOffset._12 >> boneOffset._13 >> boneOffset._14;
		fin >> boneOffset._21 >> boneOffset._22 >> boneOffset._23 >> boneOffset._24;
		fin >> boneOffset._31 >> boneOffset._32 >> boneOffset._33 >> boneOffset._34;
		fin >> boneOffset._41 >> boneOffset._42 >> boneOffset._43 >> boneOffset._44;

		m_vecBoneOffsets.push_back(boneOffset);
	}
}

void CFbxMeshData::LoadAnimationData(ifstream& fin)
{
	string buf;
	CAnimationClip animationClip;
	CKeyframeData keyframeData;
	CBoneData boneData;
	string animationName;

	animationClip.m_vecBoneData.reserve(m_nBoneCount);

	fin >> buf; // [ANIMATION_CLIPS]
	for (UINT clip = 0; clip < m_nAnimationCount; ++clip) {
		fin >> buf; // AnimationClip 
		fin >> animationName;
	
		for (UINT bone = 0; bone < m_nBoneCount; ++bone) {
			fin >> buf; // Bone			Bone0 : 351
			fin >> boneData.m_nAnimaitionKeys;
			
			boneData.m_vecKeyframeData.reserve(boneData.m_nAnimaitionKeys);
			for (UINT frame = 0; frame < boneData.m_nAnimaitionKeys; ++frame) {
				fin >> keyframeData.m_fAnimationTime;
				fin >> keyframeData.m_xmf3Translate.x >> keyframeData.m_xmf3Translate.y >> keyframeData.m_xmf3Translate.z;
				fin >> keyframeData.m_xmf3Scale.x >> keyframeData.m_xmf3Scale.y >> keyframeData.m_xmf3Scale.z;
				fin >> keyframeData.m_xmf4Quaternion.x >> keyframeData.m_xmf4Quaternion.y >> keyframeData.m_xmf4Quaternion.z >> keyframeData.m_xmf4Quaternion.w;
								    	
				boneData.m_vecKeyframeData.push_back(keyframeData);
			}

			animationClip.m_vecBoneData.push_back(boneData);
			boneData.m_vecKeyframeData.clear();
		}
		m_mapAnimationClip.insert(make_pair(animationName, animationClip));

		animationClip.m_vecBoneData.clear();
	}
}