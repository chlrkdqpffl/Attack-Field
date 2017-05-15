 #include "stdafx.h"
#include "FbxModelSkinnedMesh.h"

CFbxModelSkinnedMesh::CFbxModelSkinnedMesh(ID3D11Device *pd3dDevice, const MeshTag meshTag, const XMFLOAT3 size)
	: CFbxModelMesh(pd3dDevice, meshTag, size)
{
}

CFbxModelSkinnedMesh::~CFbxModelSkinnedMesh()
{
	ReleaseCOM(m_pd3dBoneWeightBuffer);
	ReleaseCOM(m_pd3dBoneIndiceBuffer);
	ReleaseCOM(m_pd3dcbBones);
	
	SafeDeleteArray(m_pboneWeights);
	SafeDeleteArray(m_pboneIndices);
}

void CFbxModelSkinnedMesh::Initialize(ID3D11Device *pd3dDevice, bool isCalcTangent)
{
	m_meshData = RESOURCE_MGR->CloneFbxMeshData(m_tagMesh);

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nVertices = m_meshData.m_nVertexCount;
	m_nIndices = m_meshData.m_nIndexCount;
	
	m_pPositions = new XMFLOAT3[m_nVertices];
	m_pNormals = new XMFLOAT3[m_nVertices];
	if (m_meshData.m_bTangent || isCalcTangent)
		m_pTangents = new XMFLOAT3[m_nVertices];
	m_pTexCoords = new XMFLOAT2[m_nVertices];
	m_pnIndices = new UINT[m_nIndices];
	m_pboneIndices = new XMFLOAT4[m_nVertices];
	m_pboneWeights = new XMFLOAT4[m_nVertices];
	m_bcBoundingBox = m_meshData.m_boundingBox;


	if ((m_fModelSize.x == 1) && (m_fModelSize.y == 1) && (m_fModelSize.z == 1)) {
		for (int i = 0; i < m_nVertices; ++i) {
			m_pPositions[i] = m_meshData.m_vecPosition[i];
			m_pNormals[i] = m_meshData.m_vecNormal[i];
			if (m_meshData.m_bTangent)
				m_pTangents[i] = m_meshData.m_vecTangent[i];
			m_pTexCoords[i] = m_meshData.m_vecUV[i];
			m_pboneIndices[i] = m_meshData.m_vecBoneIndices[i];
			m_pboneWeights[i] = m_meshData.m_vecBoneWeights[i];
		}
	}
	else {
		for (int i = 0; i < m_nVertices; ++i) {
			m_pPositions[i].x = m_meshData.m_vecPosition[i].x * m_fModelSize.x;
			m_pPositions[i].y = m_meshData.m_vecPosition[i].y * m_fModelSize.y;
			m_pPositions[i].z = m_meshData.m_vecPosition[i].z * m_fModelSize.z;

			if (m_meshData.m_bTangent)
				m_pTangents[i] = m_meshData.m_vecTangent[i];
			m_pNormals[i] = m_meshData.m_vecNormal[i];
			m_pTexCoords[i] = m_meshData.m_vecUV[i];
			m_pboneIndices[i] = m_meshData.m_vecBoneIndices[i];
			m_pboneWeights[i] = m_meshData.m_vecBoneWeights[i];
		}
	}

	// Create Index Buffer
	for (UINT i = 0, j = 0; i < m_nIndices / 3; ++i, j += 3) {
		m_pnIndices[j] = (UINT)m_meshData.m_vecIndex[i].x;
		m_pnIndices[j + 1] = (UINT)m_meshData.m_vecIndex[i].y;
		m_pnIndices[j + 2] = (UINT)m_meshData.m_vecIndex[i].z;
	}

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");

	if (isCalcTangent)
		CalculateVertexTangent(m_pTangents);

	// Create Buffer
	if (m_meshData.m_bTangent || isCalcTangent) {
		m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dTangentBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pTangents, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dBoneWeightBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT4), m_nVertices, m_pboneWeights, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dBoneIndiceBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT4), m_nVertices, m_pboneIndices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		ID3D11Buffer *pd3dBuffers[6] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTangentBuffer, m_pd3dTexCoordBuffer, m_pd3dBoneWeightBuffer, m_pd3dBoneIndiceBuffer };
		UINT pnBufferStrides[6] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT4), sizeof(XMFLOAT4) };
		UINT pnBufferOffsets[6] = { 0, 0, 0, 0, 0, 0 };
		AssembleToVertexBuffer(6, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	else {
		m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dBoneWeightBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT4), m_nVertices, m_pboneWeights, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		m_pd3dBoneIndiceBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT4), m_nVertices, m_pboneIndices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		ID3D11Buffer *pd3dBuffers[5] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer, m_pd3dBoneWeightBuffer, m_pd3dBoneIndiceBuffer };
		UINT pnBufferStrides[5] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT4), sizeof(XMFLOAT4) };
		UINT pnBufferOffsets[5] = { 0, 0, 0, 0, 0 };
		AssembleToVertexBuffer(5, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	
	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	if (m_meshData.m_bTangent || isCalcTangent)
		DXUT_SetDebugName(m_pd3dTangentBuffer, "Tangent");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
	DXUT_SetDebugName(m_pd3dBoneWeightBuffer, "BoneWeight");
	DXUT_SetDebugName(m_pd3dBoneIndiceBuffer, "BoneIndice");

	CreateConstantBuffer(pd3dDevice);

	m_strClipName = m_meshData.m_mapAnimationClip.rbegin()->first; // 제일 처음 들어간 애니메이션이 초기 애니메이션
}

float CFbxModelSkinnedMesh::GetClipStartTime(const string& clipName) const
{
	auto clip = m_meshData.m_mapAnimationClip.find(clipName);
	return clip->second.GetClipStartTime();
}

float CFbxModelSkinnedMesh::GetClipEndTime(const string& clipName) const
{
	auto clip = m_meshData.m_mapAnimationClip.find(clipName);
	return clip->second.GetClipEndTime();
}

void CFbxModelSkinnedMesh::CreateConstantBuffer(ID3D11Device *pd3dDevice)
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

void CFbxModelSkinnedMesh::UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbBones, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_SKINNED *pcbBones = (VS_CB_SKINNED*)d3dMappedResource.pData;
	for (UINT i = 0; i < m_meshData.m_nBoneCount; i++)
	{
		XMStoreFloat4x4(&pcbBones->m_mtxBoneTransform[i], XMMatrixTranspose(m_vecFinalBone[i]));
	}
	pd3dDeviceContext->Unmap(m_pd3dcbBones, 0);
	
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_BONETRANSFORM, 1, &m_pd3dcbBones);
}

void CFbxModelSkinnedMesh::ReleaseConstantBuffer()
{
	ReleaseCOM(m_pd3dcbBones);
}

void CFbxModelSkinnedMesh::Interpolate_Blending(const CAnimationClip& basicData, bool& enable, const CAnimationClip& targetData, float fTimePos, vector<XMFLOAT4X4>& boneTransforms)
{
	// 각 애니메이션 본의 갯수가 틀림
	assert(targetData.m_vecBoneData.size() == basicData.m_vecBoneData.size());

	XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR S1 = zero, S2 = zero;
	XMVECTOR P1 = zero, P2 = zero;
	XMVECTOR Q1 = zero, Q2 = zero;

	static float changeTime = 0.2f;		// 애니메이션 전환 시간 0.2초
	float blendFactor = m_fUpperBlendingTimeElapsed / changeTime;

	m_fUpperBlendingTimeElapsed += SCENE_MGR->g_fDeltaTime;

	if (blendFactor > 1) 		// 애니메이션 전환 시점
	{
		m_fUpperBlendingTimeElapsed = 0.0f;
		enable = false;
	}

	float timeLerpPos = (fTimePos / targetData.GetClipEndTime()) * basicData.GetClipEndTime();


	for (UINT i = 0; i < m_meshData.m_nBoneCount; ++i)
	{
		if (0 < basicData.m_vecBoneData[i].m_nAnimaitionKeys) {
			vector<CKeyframeData> keyframeDataVector = basicData.m_vecBoneData[i].m_vecKeyframeData;

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
		if (0 < targetData.m_vecBoneData[i].m_nAnimaitionKeys) {
			vector<CKeyframeData> keyframeDataVector = targetData.m_vecBoneData[i].m_vecKeyframeData;

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

void CFbxModelSkinnedMesh::Interpolate_Blending(const CAnimationClip& basicData, bool& enable, const CAnimationClip& targetData, float fTimePos, AnimationData::Parts type, vector<XMFLOAT4X4>& boneTransforms)
{
	// 각 애니메이션 본의 갯수가 틀림
	assert(targetData.m_vecBoneData.size() == basicData.m_vecBoneData.size());
	
	XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR S1 = zero, S2 = zero;
	XMVECTOR P1 = zero, P2 = zero;
	XMVECTOR Q1 = zero, Q2 = zero;

	//static float changeTime = 0.2f;		
	static const float changeTime = 0.2f;	// 애니메이션 전환 시간 0.3초
	float blendFactor;
	if (type == AnimationData::Parts::UpperBody) {
		blendFactor = m_fUpperBlendingTimeElapsed / changeTime;
		m_fUpperBlendingTimeElapsed += SCENE_MGR->g_fDeltaTime;
	}
	else {
		blendFactor = m_fLowerBlendingTimeElapsed / changeTime;
		m_fLowerBlendingTimeElapsed += SCENE_MGR->g_fDeltaTime;
	}

	if (blendFactor > 1) 		// 애니메이션 전환 시점
	{
		if (type == AnimationData::Parts::UpperBody)
			m_fUpperBlendingTimeElapsed = 0.0f;
		else
			m_fLowerBlendingTimeElapsed = 0.0f;

		enable = false;
	}

	float timeLerpPos = (fTimePos / targetData.GetClipEndTime()) * basicData.GetClipEndTime();
	
	if (type == AnimationData::Parts::UpperBody) {
		for (UINT i = 0; i < m_nBodyBoundaryIndex; ++i)
		{
			if (0 < basicData.m_vecBoneData[i].m_nAnimaitionKeys) {
				vector<CKeyframeData> keyframeDataVector = basicData.m_vecBoneData[i].m_vecKeyframeData;

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
			if (0 < targetData.m_vecBoneData[i].m_nAnimaitionKeys) {
				vector<CKeyframeData> keyframeDataVector = targetData.m_vecBoneData[i].m_vecKeyframeData;

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
	else {
		for (UINT i = m_nBodyBoundaryIndex; i < m_meshData.m_nBoneCount; ++i)
		{
			if (0 < basicData.m_vecBoneData[i].m_nAnimaitionKeys) {
				vector<CKeyframeData> keyframeDataVector = basicData.m_vecBoneData[i].m_vecKeyframeData;

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
			if (0 < targetData.m_vecBoneData[i].m_nAnimaitionKeys) {
				vector<CKeyframeData> keyframeDataVector = targetData.m_vecBoneData[i].m_vecKeyframeData;

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
}

void CFbxModelSkinnedMesh::GetFinalTransformsBlending(AnimationTrack& prevAnim, const AnimationTrack& currAnim, const float& fTimePos)
{
	vector<XMFLOAT4X4> toRootTransforms(m_meshData.m_nBoneCount);

	auto currClip = m_meshData.m_mapAnimationClip.find(currAnim.m_strClipName);

	if (prevAnim.m_bEnable == true) {
			auto pervClip = m_meshData.m_mapAnimationClip.find(prevAnim.m_strClipName);
			Interpolate_Blending(pervClip->second, prevAnim.m_bEnable, currClip->second, fTimePos, toRootTransforms);
	}
	else
		currClip->second.Interpolate(fTimePos, toRootTransforms);
	
	m_vecFinalBone.resize(m_meshData.m_nBoneCount);

	// 아래의 코드는 Terrorist Character에만 적용되므로 다른 캐릭터를 사용한다면 해당 본을 찾아서 범위를 조절해야함
	for (UINT i = 0; i < m_meshData.m_nBoneCount; ++i)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMVectorSet(1.0f, 0, 0, 0), XMConvertToRadians(m_fPitch));
		XMMATRIX mtxRotateHalf = XMMatrixRotationAxis(XMVectorSet(1.0f, 0, 0, 0), XMConvertToRadians(m_fPitch / 2));	// 유연하게 회전하기 위해 추가 생성
		XMMATRIX mtxOffset = XMLoadFloat4x4(&m_meshData.m_vecBoneOffsets[i]);
		XMMATRIX mtxToRoot = XMLoadFloat4x4(&toRootTransforms[i]);

		if (i < 2)
			m_vecFinalBone[i] = mtxOffset * mtxToRoot * mtxRotateHalf;
		else if (2 <= i && i <= 31)
			m_vecFinalBone[i] = mtxOffset * mtxToRoot * mtxRotate;
		else
			m_vecFinalBone[i] = mtxOffset * mtxToRoot;
	}
}

void CFbxModelSkinnedMesh::CalcFinalTransformsBlending(AnimationTrack& prevAnim, const AnimationTrack& currAnim, const float& fTimePos, AnimationData::Parts type)
{
	vector<XMFLOAT4X4> toRootTransforms(m_meshData.m_nBoneCount);

	auto currClip = m_meshData.m_mapAnimationClip.find(currAnim.m_strClipName);

	if (prevAnim.m_bEnable == true) {
		auto pervClip = m_meshData.m_mapAnimationClip.find(prevAnim.m_strClipName);
		Interpolate_Blending(pervClip->second, prevAnim.m_bEnable, currClip->second, fTimePos, type, toRootTransforms);
	}
	else {
		if (type == AnimationData::Parts::UpperBody)
			currClip->second.Interpolate(fTimePos, 0, m_nBodyBoundaryIndex, toRootTransforms);
		else
			currClip->second.Interpolate(fTimePos, m_nBodyBoundaryIndex, m_meshData.m_nBoneCount, toRootTransforms);
	}
	m_vecFinalBone.resize(m_meshData.m_nBoneCount);


	// 아래의 코드는 Terrorist Character에만 적용되므로 다른 캐릭터를 사용한다면 해당 본을 찾아서 범위를 조절해야함
	if (type == AnimationData::Parts::UpperBody) {
		for (UINT i = 0; i < m_nBodyBoundaryIndex; ++i)
		{
			XMMATRIX mtxRotate = XMMatrixRotationAxis(XMVectorSet(1.0f, 0, 0, 0), XMConvertToRadians(m_fPitch));
			XMMATRIX mtxRotateHalf = XMMatrixRotationAxis(XMVectorSet(1.0f, 0, 0, 0), XMConvertToRadians(m_fPitch / 2));	// 유연하게 회전하기 위해 추가 생성
			XMMATRIX mtxOffset = XMLoadFloat4x4(&m_meshData.m_vecBoneOffsets[i]);
			XMMATRIX mtxToRoot = XMLoadFloat4x4(&toRootTransforms[i]);

			if (i < 2)
				m_vecFinalBone[i] = mtxOffset * mtxToRoot * mtxRotateHalf;
			else
				m_vecFinalBone[i] = mtxOffset * mtxToRoot * mtxRotate;
		}
	}
	else {
		for (UINT i = m_nBodyBoundaryIndex; i < m_meshData.m_nBoneCount; ++i)
		{
			XMMATRIX mtxOffset = XMLoadFloat4x4(&m_meshData.m_vecBoneOffsets[i]);
			XMMATRIX mtxToRoot = XMLoadFloat4x4(&toRootTransforms[i]);

			m_vecFinalBone[i] = mtxOffset * mtxToRoot;
		}
	}
}