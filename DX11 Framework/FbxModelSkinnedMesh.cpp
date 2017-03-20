 #include "stdafx.h"
#include "FbxModelSkinnedMesh.h"

CFbxModelSkinnedMesh::CFbxModelSkinnedMesh(ID3D11Device *pd3dDevice, const MeshTag meshTag, const float size)
	: CFbxModelMesh(pd3dDevice, meshTag, size)
{
}

CFbxModelSkinnedMesh::~CFbxModelSkinnedMesh()
{
	if (m_pd3dBoneWeightBuffer) m_pd3dBoneWeightBuffer->Release();
	if (m_pd3dBoneIndiceBuffer) m_pd3dBoneIndiceBuffer->Release();
	if (m_pd3dcbBones) m_pd3dcbBones->Release();

	if (m_pboneWeights) delete[] m_pboneWeights;
	if (m_pboneIndices) delete[] m_pboneIndices;
}

void CFbxModelSkinnedMesh::Initialize(ID3D11Device *pd3dDevice)
{
	m_meshData = RESOURCE_MGR->CloneFbxMeshData(m_meshTag);

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nVertices = m_meshData.m_nVertexCount;
	m_nIndices = m_meshData.m_nIndexCount;
	
	m_pPositions = new XMFLOAT3[m_nVertices];
	m_pNormals = new XMFLOAT3[m_nVertices];
	if (m_meshData.m_bTangent)
		m_pTangents = new XMFLOAT3[m_nVertices];
	m_pTexCoords = new XMFLOAT2[m_nVertices];
	m_pnIndices = new UINT[m_nIndices];
	m_pboneIndices = new XMFLOAT4[m_nVertices];
	m_pboneWeights = new XMFLOAT4[m_nVertices];
	m_bcBoundingBox = m_meshData.m_boundingBox;

	if (m_fModelSize == 1.0f) {
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
			XMStoreFloat3(&m_pPositions[i], XMVectorScale(XMLoadFloat3(&m_meshData.m_vecPosition[i]), m_fModelSize));
			//		XMStoreFloat3(&m_pvNormals[i], XMVectorScale(XMLoadFloat3(&m_meshData.m_vecNormal[i]), size));
			//		XMStoreFloat2(&m_pvTexCoords[i], XMVectorScale(XMLoadFloat2(&m_meshData.m_vecUV[i]), size));
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


	/*
	// Tangent 수동 계산 보류중
	if (m_meshData.m_bTangent) {
		XMVECTOR* tangent = new XMVECTOR[m_nVertices];
		CalculateVertexTangent(tangent);

		for (int i = 0; i < m_nVertices; ++i) {
//			cout << m_pTangents[i].x << ", " << m_pTangents[i].y << ", " << m_pTangents[i].z << endl;
//			cout << XMVectorGetX(tangent[i]) << ", " << XMVectorGetY(tangent[i]) << ", " << XMVectorGetZ(tangent[i]) << endl;
//			cout << endl;
	//		XMStoreFloat3(&m_pTangents[i], tangent[i]);
		}

		delete[] tangent;

		cout << endl << endl;
	}
	
	*/


	// Create Buffer
	if (m_meshData.m_bTangent) {
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
	if (m_meshData.m_bTangent)
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
		XMStoreFloat4x4(&pcbBones->m_mtxBoneTransform[i], XMMatrixTranspose(finalBoneVector[i]));
	}
	pd3dDeviceContext->Unmap(m_pd3dcbBones, 0);

	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_BONETRANSFORM, 1, &m_pd3dcbBones);
}

void CFbxModelSkinnedMesh::ReleaseConstantBuffer()
{
	ReleaseCOM(m_pd3dcbBones);
}

void CFbxModelSkinnedMesh::Interpolate_Blending(const CAnimationClip& basicData, bool& enable, const CAnimationClip& targetData, float fTimePos, vector<XMFLOAT4X4>& boneTransforms) const
{
	// 각 애니메이션 본의 갯수가 틀림
	assert(targetData.m_vecBoneData.size() == basicData.m_vecBoneData.size());
	
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
	
	for (UINT i = 0; i < targetData.m_vecBoneData.size(); ++i)
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

void CFbxModelSkinnedMesh::GetFinalTransforms(const string& clipName, const float& fTimePos)
{
	vector<XMFLOAT4X4> toRootTransforms(m_meshData.m_nBoneCount);

	auto clip = m_meshData.m_mapAnimationClip.find(clipName);
	clip->second.Interpolate(fTimePos, toRootTransforms);

	finalBoneVector.resize(m_meshData.m_nBoneCount);

	for (UINT i = 0; i < m_meshData.m_nBoneCount; ++i)
	{
		XMMATRIX mtxOffset = XMLoadFloat4x4(&m_meshData.m_vecBoneOffsets[i]);
		XMMATRIX mtxToRoot = XMLoadFloat4x4(&toRootTransforms[i]);

		finalBoneVector[i] = mtxOffset * mtxToRoot;
	}
}

void CFbxModelSkinnedMesh::GetFinalTransformsBlending(AnimationTrack& prevAnim, const AnimationTrack& currAnim, const float& fTimePos)
{
	vector<XMFLOAT4X4> toRootTransforms(m_meshData.m_nBoneCount);

	auto currClip = m_meshData.m_mapAnimationClip.find(currAnim.m_strClipName);
	
	if (TWBAR_MGR->g_isAnimBlending) {
		if (prevAnim.m_bEnable == true) {
			auto pervClip = m_meshData.m_mapAnimationClip.find(prevAnim.m_strClipName);
			Interpolate_Blending(pervClip->second, prevAnim.m_bEnable, currClip->second, fTimePos, toRootTransforms);
		}
		else
			currClip->second.Interpolate(fTimePos, toRootTransforms);
	}
	else
		currClip->second.Interpolate(fTimePos, toRootTransforms);


	finalBoneVector.resize(m_meshData.m_nBoneCount);

	for (UINT i = 0; i < m_meshData.m_nBoneCount; ++i)
	{
		XMMATRIX mtxOffset = XMLoadFloat4x4(&m_meshData.m_vecBoneOffsets[i]);
		XMMATRIX mtxToRoot = XMLoadFloat4x4(&toRootTransforms[i]);

		finalBoneVector[i] = mtxOffset * mtxToRoot;
	}
}