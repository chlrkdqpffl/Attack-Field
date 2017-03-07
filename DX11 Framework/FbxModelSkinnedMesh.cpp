#include "stdafx.h"
#include "FbxModelSkinnedMesh.h"

void BoneData::Interpolate(float fTimePos, XMFLOAT4X4& boneTransforms) const
{
	if (fTimePos <= m_keyframeDataVector.front().m_fAnimationTime)
	{
		XMVECTOR S = XMLoadFloat3(&m_keyframeDataVector.front().m_xmf3Scale);
		XMVECTOR P = XMLoadFloat3(&m_keyframeDataVector.front().m_xmf3Translate);
		XMVECTOR Q = XMLoadFloat4(&m_keyframeDataVector.front().m_xmf4Quaternion);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&boneTransforms, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else if (m_keyframeDataVector.back().m_fAnimationTime <= fTimePos)
	{
		XMVECTOR S = XMLoadFloat3(&m_keyframeDataVector.back().m_xmf3Scale);
		XMVECTOR P = XMLoadFloat3(&m_keyframeDataVector.back().m_xmf3Translate);
		XMVECTOR Q = XMLoadFloat4(&m_keyframeDataVector.back().m_xmf4Quaternion);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&boneTransforms, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else
	{
		for (UINT i = 0; i < m_keyframeDataVector.size() - 1; ++i)
		{
			if (m_keyframeDataVector[i].m_fAnimationTime <= fTimePos && fTimePos <= m_keyframeDataVector[i + 1].m_fAnimationTime)
			{
				float lerpPercent = (fTimePos - m_keyframeDataVector[i].m_fAnimationTime) / (m_keyframeDataVector[i + 1].m_fAnimationTime - m_keyframeDataVector[i].m_fAnimationTime);

				XMVECTOR s0 = XMLoadFloat3(&m_keyframeDataVector[i].m_xmf3Scale);
				XMVECTOR s1 = XMLoadFloat3(&m_keyframeDataVector[i + 1].m_xmf3Scale);
											
				XMVECTOR p0 = XMLoadFloat3(&m_keyframeDataVector[i].m_xmf3Translate);
				XMVECTOR p1 = XMLoadFloat3(&m_keyframeDataVector[i + 1].m_xmf3Translate);
											
				XMVECTOR q0 = XMLoadFloat4(&m_keyframeDataVector[i].m_xmf4Quaternion);
				XMVECTOR q1 = XMLoadFloat4(&m_keyframeDataVector[i + 1].m_xmf4Quaternion);

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

void AnimationClip::Interpolate(float fTimePos, vector<XMFLOAT4X4>& boneTransforms) const
{
	for (UINT i = 0; i < m_boneDataVector.size(); ++i)
	{
		if (0 < m_boneDataVector[i].m_nAnimaitionKeys)
			m_boneDataVector[i].Interpolate(fTimePos, boneTransforms[i]);
		else
		{
			XMMATRIX identityMtx = XMMatrixIdentity();
			XMStoreFloat4x4(&boneTransforms[i], identityMtx);
		}
	}
}

float AnimationClip::GetClipStartTime() const
{
	float t = FLT_MAX;

	for (UINT i = 0; i < m_boneDataVector.size(); ++i) {
		if (m_boneDataVector[i].m_keyframeDataVector.size() == 0) continue;
		t = fmin(t, m_boneDataVector[i].GetStartTime());
	}
	return t;
}

float AnimationClip::GetClipEndTime() const
{
	float t = 0.0f;

	for (UINT i = 0; i < m_boneDataVector.size(); ++i) {
		if (m_boneDataVector[i].m_keyframeDataVector.size() == 0) continue;
		t = fmax(t, m_boneDataVector[i].GetEndTime());
	}

	return t;
}

CSkinnedMesh::CSkinnedMesh(ID3D11Device *pd3dDevice, const string& fileName, float size) : CFbxModelMesh(pd3dDevice, fileName, true, size)
{
}

CSkinnedMesh::~CSkinnedMesh()
{
	if (m_pd3dBoneWeightBuffer) m_pd3dBoneWeightBuffer->Release();
	if (m_pd3dBoneIndiceBuffer) m_pd3dBoneIndiceBuffer->Release();
	if (m_pd3dcbBones) m_pd3dcbBones->Release();

	if (m_pboneWeights) delete[] m_pboneWeights;
	if (m_pboneIndices) delete[] m_pboneIndices;
}

void CSkinnedMesh::Initialize(ID3D11Device *pd3dDevice)
{
	cout << "File Loading < " + m_fileName + " > ";
	bool isLoad = LoadFBXfromFile(m_fileName);

	if (isLoad)
		ShowTaskSuccess("\t Success!!");
	else {
		ShowTaskFail("\t Error!! \t 파일 또는 경로를 확인하세요.");
		return;
	}

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	if (m_fModelSize == 1.0f) {
		for (int i = 0; i < m_nVertices; ++i) {
			m_pPositions[i] = posVector[i];
			m_pNormals[i] = normalVector[i];
			m_pTexCoords[i] = uvVector[i];
			m_pboneWeights[i] = boneWeightsVector[i];
			m_pboneIndices[i] = boneIndicesVector[i];
		}
	}
	else {
		for (int i = 0; i < m_nVertices; ++i) {
			XMStoreFloat3(&m_pPositions[i], XMVectorScale(XMLoadFloat3(&posVector[i]), m_fModelSize));
			//		XMStoreFloat3(&m_pvNormals[i], XMVectorScale(XMLoadFloat3(&normalVector[i]), size));
			//		XMStoreFloat2(&m_pvTexCoords[i], XMVectorScale(XMLoadFloat2(&uvVector[i]), size));
			m_pNormals[i] = normalVector[i];
			m_pTexCoords[i] = uvVector[i];
			m_pboneWeights[i] = boneWeightsVector[i];
			m_pboneIndices[i] = boneIndicesVector[i];
		}
	}

	// Create Buffer
	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTangentBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pTangents, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dBoneWeightBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT4), m_nVertices, m_pboneWeights, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dBoneIndiceBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT4), m_nVertices, m_pboneIndices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	DXUT_SetDebugName(m_pd3dTangentBuffer, "Tangent");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
	DXUT_SetDebugName(m_pd3dBoneWeightBuffer, "BoneWeight");
	DXUT_SetDebugName(m_pd3dBoneIndiceBuffer, "BoneIndice");


/*
	ID3D11Buffer *pd3dBuffers[5] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer, m_pd3dBoneWeightBuffer, m_pd3dBoneIndiceBuffer };
	UINT pnBufferStrides[5] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT4), sizeof(XMFLOAT4) };
	UINT pnBufferOffsets[5] = { 0, 0, 0, 0, 0 };
	AssembleToVertexBuffer(5, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	*/

	// Normalmap
	ID3D11Buffer *pd3dBuffers[6] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTangentBuffer, m_pd3dTexCoordBuffer, m_pd3dBoneWeightBuffer, m_pd3dBoneIndiceBuffer };
	UINT pnBufferStrides[6] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT4), sizeof(XMFLOAT4) };
	UINT pnBufferOffsets[6] = { 0, 0, 0, 0, 0, 0 };
	AssembleToVertexBuffer(6, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	
	for (UINT i = 0, j = 0; i < m_nIndices / 3; ++i, j += 3) {
		m_pnIndices[j] = (UINT)indexVector[i].x;
		m_pnIndices[j + 1] = (UINT)indexVector[i].y;
		m_pnIndices[j + 2] = (UINT)indexVector[i].z;
	}

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");

	CreateConstantBuffer(pd3dDevice);

	m_strClipName = m_animationMap.rbegin()->first; // 제일 처음 들어간 애니메이션이 초기 애니메이션
}

bool CSkinnedMesh::LoadFBXfromFile(const string& fileName)
{
	ifstream fin(fileName);

	if (!fin.is_open())
		return false;

	UINT meshCount = 0;
	UINT vertexCount = 0, indexCount = 0, boneCount = 0, animationClips = 0;
	XMFLOAT3 pos, normal, tangent, index;
	XMFLOAT2 uv;
	XMFLOAT4 boneIndex, boneWeight;
	XMFLOAT3 bBoxCenter, bBoxExtents, bBoxMax;
	int boneHierarchy;
	XMFLOAT4X4 boneOffset, SQTTransform, finalBone;

	AnimationClip animaitionData;
	KeyframeData keyframeData;
	BoneData boneData;
	string animationName;

	string buf;

	while (!fin.eof()) {
		fin >> buf; // [FBX_META_DATA]
		fin >> buf >> meshCount;

		// 다중 메쉬의 경우 애니메이션에 문제가 발생
		assert(meshCount == 1);

		fin >> buf; // [MESH_DATA]
		fin >> vertexCount;
		{
			posVector.reserve(vertexCount);
			normalVector.reserve(vertexCount);
			tangentVector.reserve(vertexCount);
			uvVector.reserve(vertexCount);
		}
		fin >> indexCount;
		{
			indexVector.reserve(indexCount / 3);
		}
		fin >> boneCount;
		fin >> animationClips;

		fin >> buf; // [BBox_DATA]
		fin >> buf; // Center
		fin >> bBoxCenter.x >> bBoxCenter.y >> bBoxCenter.z;
		fin >> buf; // Min
		fin >> buf >> buf >> buf;
		fin >> buf; // Max
		fin >> bBoxMax.x >> bBoxMax.y >> bBoxMax.z;

		m_bcBoundingBox.Center = bBoxCenter;
		bBoxExtents.x = bBoxMax.x - bBoxCenter.x;
		bBoxExtents.y = bBoxMax.y - bBoxCenter.y;
		bBoxExtents.z = bBoxMax.z - bBoxCenter.z;
		m_bcBoundingBox.Extents = bBoxExtents;

		fin >> buf; // [VERTEX_DATA]
		for (UINT i = 0; i < vertexCount; ++i) {
//			fin >> buf;	// Position
			fin >> pos.x >> pos.y >> pos.z;
//			fin >> buf;	// Normal
			fin >> normal.x >> normal.y >> normal.z;
//			fin >> buf; // Tangent
			fin >> tangent.x >> tangent.y >> tangent.z;
//			fin >> buf;	// UV
			fin >> uv.x >> uv.y;
//			fin >> buf;	// BoneIndices 
			fin >> boneIndex.x >> boneIndex.y >> boneIndex.z >> boneIndex.w;
//			fin >> buf;	// BoneWeights 
			fin >> boneWeight.x >> boneWeight.y >> boneWeight.z >> boneWeight.w;

			posVector.push_back(pos);
			normalVector.push_back(normal);
			tangentVector.push_back(tangent);
			uvVector.push_back(uv);
			boneIndicesVector.push_back(boneIndex);
			boneWeightsVector.push_back(boneWeight);
		}

		fin >> buf; // [INDEX_DATA]
		for (UINT i = 0; i < indexCount / 3; ++i) {
			fin >> index.x >> index.y >> index.z;

			indexVector.push_back(index);
		}


		fin >> buf; // [BONE_HIERARCHY]
		for (UINT i = 0; i < boneCount; ++i) {
			fin >> boneHierarchy;

			boneHierarchyVector.push_back(boneHierarchy);
		}

		fin >> buf; // [OFFSET_MATRIX]
		for (UINT i = 0; i < boneCount; ++i) {
			//			fin >> buf; // BoneOffSet
			fin >> boneOffset._11 >> boneOffset._12 >> boneOffset._13 >> boneOffset._14;
			fin >> boneOffset._21 >> boneOffset._22 >> boneOffset._23 >> boneOffset._24;
			fin >> boneOffset._31 >> boneOffset._32 >> boneOffset._33 >> boneOffset._34;
			fin >> boneOffset._41 >> boneOffset._42 >> boneOffset._43 >> boneOffset._44;

			boneOffsetsVector.push_back(boneOffset);
		}

		animaitionData.m_boneDataVector.reserve(boneCount);
		fin >> buf; // [ANIMATION_CLIPS]
		for (UINT clip = 0; clip < animationClips; ++clip) {
			fin >> buf; // AnimationClip 
			fin >> animationName;
			//			fin >> buf;	// {

			for (UINT bone = 0; bone < boneCount; ++bone) {
				fin >> buf; // Bone			Bone0 : 351
				fin >> boneData.m_nAnimaitionKeys;
				//				fin >> buf;	// {

				boneData.m_keyframeDataVector.reserve(boneData.m_nAnimaitionKeys);
				for (UINT frame = 0; frame < boneData.m_nAnimaitionKeys; ++frame) {
					fin >> keyframeData.m_fAnimationTime;
					fin >> keyframeData.m_xmf3Translate.x >> keyframeData.m_xmf3Translate.y >> keyframeData.m_xmf3Translate.z;
					fin >> keyframeData.m_xmf3Scale.x >> keyframeData.m_xmf3Scale.y >> keyframeData.m_xmf3Scale.z;
					fin >> keyframeData.m_xmf4Quaternion.x >> keyframeData.m_xmf4Quaternion.y >> keyframeData.m_xmf4Quaternion.z >> keyframeData.m_xmf4Quaternion.w;

					boneData.m_keyframeDataVector.push_back(keyframeData);
				}

				animaitionData.m_boneDataVector.push_back(boneData);
				boneData.m_keyframeDataVector.clear();
				//				fin >> buf; // }
			}
			m_animationMap.insert(make_pair(animationName, animaitionData));

			animaitionData.m_boneDataVector.clear();
		}

		// End
		fin >> buf;
		fin >> buf;

	}
	fin.close();

	m_nVertices = vertexCount;
	m_nIndices = indexCount;
	m_nBoneCount = boneCount;
	m_nAnimationClip = animationClips;

	m_pPositions = new XMFLOAT3[m_nVertices];
	m_pNormals = new XMFLOAT3[m_nVertices];
	m_pTangents = new XMFLOAT3[m_nVertices];
	m_pTexCoords = new XMFLOAT2[m_nVertices];
	m_pnIndices = new UINT[m_nIndices];
	m_pboneIndices = new XMFLOAT4[m_nVertices];
	m_pboneWeights = new XMFLOAT4[m_nVertices];

	return true;
}

float CSkinnedMesh::GetClipStartTime(const string& clipName) const
{
	auto clip = m_animationMap.find(clipName);
	return clip->second.GetClipStartTime();
}

float CSkinnedMesh::GetClipEndTime(const string& clipName) const
{
	auto clip = m_animationMap.find(clipName);
	return clip->second.GetClipEndTime();
}

void CSkinnedMesh::CreateConstantBuffer(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VS_CB_SKINNED);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bufferDesc, NULL, &m_pd3dcbBones);

	DXUT_SetDebugName(m_pd3dcbBones, "BoneTransform");
}

void CSkinnedMesh::UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbBones, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_SKINNED *pcbBones = (VS_CB_SKINNED*)d3dMappedResource.pData;
	for (UINT i = 0; i < m_nBoneCount; i++)
	{
		XMStoreFloat4x4(&pcbBones->m_mtxBoneTransform[i], XMMatrixTranspose(finalBoneVector[i]));
	}
	pd3dDeviceContext->Unmap(m_pd3dcbBones, 0);

	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_BONETRANSFORM, 1, &m_pd3dcbBones);
}

void CSkinnedMesh::ReleaseConstantBuffer()
{
	ReleaseCOM(m_pd3dcbBones);
}

void CSkinnedMesh::Interpolate_Blending(const AnimationClip& basicData, bool& enable, const AnimationClip& targetData, float fTimePos, vector<XMFLOAT4X4>& boneTransforms) const
{
	// 각 애니메이션 본의 갯수가 틀림
	assert(targetData.m_boneDataVector.size() == basicData.m_boneDataVector.size());
	
	XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR S1 = zero, S2 = zero;
	XMVECTOR P1 = zero, P2 = zero;
	XMVECTOR Q1 = zero, Q2 = zero;

	static float addTime = 0.0f;
	static float changeTime = 0.3f;
	float blendFactor = addTime / changeTime;
	
	addTime += SCENE_MGR->fTimeElapsed;

	if (blendFactor > 1) {
		// 애니메이션 완전히 전환 시점		
		addTime = 0.0f;
		enable = false;
	}

	float timeLerpPos = (fTimePos / targetData.GetClipEndTime()) * basicData.GetClipEndTime();
	
	for (UINT i = 0; i < targetData.m_boneDataVector.size(); ++i)
	{
		if (0 < basicData.m_boneDataVector[i].m_nAnimaitionKeys) {
			vector<KeyframeData> keyframeDataVector = basicData.m_boneDataVector[i].m_keyframeDataVector;

			for (UINT i = 0; i < keyframeDataVector.size() - 1; ++i)
			{
				if (keyframeDataVector[i].m_fAnimationTime <= timeLerpPos && timeLerpPos <= keyframeDataVector[i + 1].m_fAnimationTime)
				{
					float lerpPercent = (timeLerpPos - keyframeDataVector[i].m_fAnimationTime) / (keyframeDataVector[i + 1].m_fAnimationTime - keyframeDataVector[i].m_fAnimationTime);

					XMVECTOR s0 = XMLoadFloat3(&keyframeDataVector[i].m_xmf3Scale);
					XMVECTOR s1 = XMLoadFloat3(&keyframeDataVector[i + 1].m_xmf3Scale);

					XMVECTOR p0 = XMLoadFloat3(&keyframeDataVector[i].m_xmf3Translate);
					XMVECTOR p1 = XMLoadFloat3(&keyframeDataVector[i + 1].m_xmf3Translate);

					XMVECTOR q0 = XMLoadFloat4(&keyframeDataVector[i].m_xmf4Quaternion);
					XMVECTOR q1 = XMLoadFloat4(&keyframeDataVector[i + 1].m_xmf4Quaternion);

					S1 = XMVectorLerp(s0, s1, lerpPercent);
					P1 = XMVectorLerp(p0, p1, lerpPercent);
					Q1 = XMQuaternionSlerp(q0, q1, lerpPercent);

					break;
				}
			}
		}
		if (0 < targetData.m_boneDataVector[i].m_nAnimaitionKeys) {
			vector<KeyframeData> keyframeDataVector = targetData.m_boneDataVector[i].m_keyframeDataVector;

			if (fTimePos <= keyframeDataVector.front().m_fAnimationTime)
			{
				S2 = XMLoadFloat3(&keyframeDataVector.front().m_xmf3Scale);
				P2 = XMLoadFloat3(&keyframeDataVector.front().m_xmf3Translate);
				Q2 = XMLoadFloat4(&keyframeDataVector.front().m_xmf4Quaternion);
			}
			else if (keyframeDataVector.back().m_fAnimationTime <= fTimePos)
			{
				S2 = XMLoadFloat3(&keyframeDataVector.back().m_xmf3Scale);
				P2 = XMLoadFloat3(&keyframeDataVector.back().m_xmf3Translate);
				Q2 = XMLoadFloat4(&keyframeDataVector.back().m_xmf4Quaternion);
			}

			for (UINT i = 0; i < keyframeDataVector.size() - 1; ++i)
			{
				if (keyframeDataVector[i].m_fAnimationTime <= fTimePos && fTimePos <= keyframeDataVector[i + 1].m_fAnimationTime)
				{
					float lerpPercent = (fTimePos - keyframeDataVector[i].m_fAnimationTime) / (keyframeDataVector[i + 1].m_fAnimationTime - keyframeDataVector[i].m_fAnimationTime);

					XMVECTOR s0 = XMLoadFloat3(&keyframeDataVector[i].m_xmf3Scale);
					XMVECTOR s1 = XMLoadFloat3(&keyframeDataVector[i + 1].m_xmf3Scale);

					XMVECTOR p0 = XMLoadFloat3(&keyframeDataVector[i].m_xmf3Translate);
					XMVECTOR p1 = XMLoadFloat3(&keyframeDataVector[i + 1].m_xmf3Translate);

					XMVECTOR q0 = XMLoadFloat4(&keyframeDataVector[i].m_xmf4Quaternion);
					XMVECTOR q1 = XMLoadFloat4(&keyframeDataVector[i + 1].m_xmf4Quaternion);

					S2 = XMVectorLerp(s0, s1, lerpPercent);
					P2 = XMVectorLerp(p0, p1, lerpPercent);
					Q2 = XMQuaternionSlerp(q0, q1, lerpPercent);

					break;
				}
			}
		}
		else if (XMVector3Equal(S1, zero))
		{
			XMMATRIX identityMtx = XMMatrixIdentity();
			XMStoreFloat4x4(&boneTransforms[i], identityMtx);
			return;
		}

		XMVECTOR S = XMVectorLerp(S1, S2, blendFactor);
		XMVECTOR P = XMVectorLerp(P1, P2, blendFactor);
		XMVECTOR Q = XMQuaternionSlerp(Q1, Q2, blendFactor);

		XMStoreFloat4x4(&boneTransforms[i], XMMatrixAffineTransformation(S, zero, Q, P));
	}
}

void CSkinnedMesh::GetFinalTransforms(const string& clipName, float fTimePos)
{
	vector<XMFLOAT4X4> toRootTransforms(m_nBoneCount);

	auto clip = m_animationMap.find(clipName);
	clip->second.Interpolate(fTimePos, toRootTransforms);

	finalBoneVector.resize(m_nBoneCount);

	for (UINT i = 0; i < m_nBoneCount; ++i)
	{
		XMMATRIX mtxOffset = XMLoadFloat4x4(&boneOffsetsVector[i]);
		XMMATRIX mtxToRoot = XMLoadFloat4x4(&toRootTransforms[i]);

		finalBoneVector[i] = mtxOffset * mtxToRoot;
	}
}

void CSkinnedMesh::GetFinalTransformsBlending(AnimationTrack& prevAnim, const AnimationTrack& currAnim, float fTimePos)
{
	vector<XMFLOAT4X4> toRootTransforms(m_nBoneCount);

	auto currClip = m_animationMap.find(currAnim.m_strClipName);
	
	if (TWBAR_MGR->g_isAnimBlending) {
		if (prevAnim.m_bEnable == true) {
			auto pervClip = m_animationMap.find(prevAnim.m_strClipName);
			Interpolate_Blending(pervClip->second, prevAnim.m_bEnable, currClip->second, fTimePos, toRootTransforms);
		}
		else
			currClip->second.Interpolate(fTimePos, toRootTransforms);
	}
	else
		currClip->second.Interpolate(fTimePos, toRootTransforms);


	finalBoneVector.resize(m_nBoneCount);

	for (UINT i = 0; i < m_nBoneCount; ++i)
	{
		XMMATRIX mtxOffset = XMLoadFloat4x4(&boneOffsetsVector[i]);
		XMMATRIX mtxToRoot = XMLoadFloat4x4(&toRootTransforms[i]);

		finalBoneVector[i] = mtxOffset * mtxToRoot;
	}
}