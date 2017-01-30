#include "stdafx.h"
#include "ModelSkinnedMesh.h"

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

void AnimationData::Interpolate(float fTimePos, vector<XMFLOAT4X4>& boneTransforms) const
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

float AnimationData::GetClipStartTime() const
{
	float t = FLT_MAX;

	for (UINT i = 0; i < m_boneDataVector.size(); ++i) {
		if (m_boneDataVector[i].m_keyframeDataVector.size() == 0) continue;
		t = fmin(t, m_boneDataVector[i].GetStartTime());
	}
	return t;
}

float AnimationData::GetClipEndTime() const
{
	float t = 0.0f;

	for (UINT i = 0; i < m_boneDataVector.size(); ++i) {
		if (m_boneDataVector[i].m_keyframeDataVector.size() == 0) continue;
		t = fmax(t, m_boneDataVector[i].GetEndTime());
	}

	return t;
}

CSkinnedMesh::CSkinnedMesh(ID3D11Device *pd3dDevice, const string& fileName, float size) : CModelMesh_FBX(pd3dDevice, fileName, size)
{
}

CSkinnedMesh::~CSkinnedMesh()
{
	if (m_pd3dBoneWeightBuffer) m_pd3dBoneWeightBuffer->Release();
	if (m_pd3dBoneIndiceBuffer) m_pd3dBoneIndiceBuffer->Release();
	if (m_pd3dcbBones) m_pd3dcbBones->Release();

	if (m_pboneWeights) delete[] m_pboneWeights;
	if (m_pboneIndices) delete[] m_pboneIndices;
//	if (m_pd3dxmtxFinalBone) delete[] m_pd3dxmtxFinalBone;
//	if (m_pd3dxmtxSQTTransform) delete[] m_pd3dxmtxSQTTransform;
}

void CSkinnedMesh::Initialize(ID3D11Device *pd3dDevice)
{
	bool isLoad = LoadFBXfromFile(m_fileName);
#if defined(DEBUG) || defined(_DEBUG)
	if (isLoad)
		cout << "File Load Success!! <" << m_fileName.c_str() << ">" << endl;
	else {
		cout << "File Load Error!! <" << m_fileName.c_str() << "> \t 파일 또는 경로를 확인하세요." << endl;
		return;
	}
#endif

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
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dBoneWeightBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT4), m_nVertices, m_pboneWeights, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dBoneIndiceBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT4), m_nVertices, m_pboneIndices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
	DXUT_SetDebugName(m_pd3dBoneWeightBuffer, "BoneWeight");
	DXUT_SetDebugName(m_pd3dBoneIndiceBuffer, "BoneIndice");

	ID3D11Buffer *pd3dBuffers[5] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer, m_pd3dBoneWeightBuffer, m_pd3dBoneIndiceBuffer };
	UINT pnBufferStrides[5] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT4), sizeof(XMFLOAT4) };
	UINT pnBufferOffsets[5] = { 0, 0, 0, 0, 0 };
	AssembleToVertexBuffer(5, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	for (int i = 0, j = 0; i < m_nIndices / 3; ++i, j += 3) {
		m_pnIndices[j] = (UINT)indexVector[i].x;
		m_pnIndices[j + 1] = (UINT)indexVector[i].y;
		m_pnIndices[j + 2] = (UINT)indexVector[i].z;
	}

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");

	float max_x = m_pPositions[0].x, max_y = m_pPositions[0].y, max_z = m_pPositions[0].z;
	for (int i = 0; i < m_nVertices; i++) {
		if (max_x < m_pPositions[i].x) max_x = m_pPositions[i].x;
		if (max_y < m_pPositions[i].y) max_y = m_pPositions[i].y;
		if (max_z < m_pPositions[i].z) max_z = m_pPositions[i].z;
	}
	float min_x = m_pPositions[0].x, min_y = m_pPositions[0].y, min_z = m_pPositions[0].z;
	for (int i = 0; i < m_nVertices; i++) {
		if (min_x > m_pPositions[i].x) min_x = m_pPositions[i].x;
		if (min_y > m_pPositions[i].y) min_y = m_pPositions[i].y;
		if (min_z > m_pPositions[i].z) min_z = m_pPositions[i].z;
	}

	m_bcBoundingCube.Center = XMFLOAT3(0, 0, 0);
	m_bcBoundingCube.Extents = XMFLOAT3(max_x, max_y, max_z);

	CreateConstantBuffer(pd3dDevice);

	m_strClipName = (--m_animationMap.end())->first;	// 제일 처음 들어간 애니메이션이 초기 애니메이션

//	m_nMaxFrameNum = 108; // idle
//	m_nMaxFrameNum = 37; // CrossPunch
}

bool CSkinnedMesh::LoadFBXfromFile(const string& fileName)
{
	ifstream fin(fileName);

	if (!fin.is_open())
		return false;

	UINT meshCount = 0;
	UINT vertexCount = 0, indexCount = 0, boneCount = 0, animationClips = 0;
	XMFLOAT3 pos, normal, index;
	XMFLOAT2 uv;
	XMFLOAT4 boneIndex, boneWeight;
	int boneHierarchy;
	XMFLOAT4X4 boneOffset, SQTTransform, finalBone;

	AnimationData animaitionData;
	KeyframeData keyframeData;
	BoneData boneData;
	string animationName;

	string buf;

	while (!fin.eof()) {
		fin >> buf; // [FBX_META_DATA]
		fin >> buf >> meshCount;

		fin >> buf; // [MESH_DATA]
		fin >> vertexCount;
		{
			posVector.reserve(vertexCount);
			normalVector.reserve(vertexCount);
			uvVector.reserve(vertexCount);
		}
		fin >> indexCount;
		{
			indexVector.reserve(indexCount / 3);
		}
		fin >> boneCount;
		fin >> animationClips;

		fin >> buf; // [VERTEX_DATA]
		for (int i = 0; i < vertexCount; ++i) {
//			fin >> buf;	// Position
			fin >> pos.x >> pos.y >> pos.z;
//			fin >> buf;	// Normal
			fin >> normal.x >> normal.y >> normal.z;
//			fin >> buf;	// UV
			fin >> uv.x >> uv.y;
//			fin >> buf;	// BoneIndices 
			fin >> boneIndex.x >> boneIndex.y >> boneIndex.z >> boneIndex.w;
//			fin >> buf;	// BoneWeights 
			fin >> boneWeight.x >> boneWeight.y >> boneWeight.z >> boneWeight.w;

			posVector.push_back(pos);
			normalVector.push_back(normal);
			uvVector.push_back(uv);
			boneIndicesVector.push_back(boneIndex);
			boneWeightsVector.push_back(boneWeight);
		}

		fin >> buf; // [INDEX_DATA]
		for (int i = 0; i < indexCount / 3; ++i) {
			fin >> index.x >> index.y >> index.z;

			indexVector.push_back(index);
		}

		fin >> buf; // [BONE_HIERARCHY]
		for (int i = 0; i < boneCount; ++i) {
			fin >> boneHierarchy;

			boneHierarchyVector.push_back(boneHierarchy);
		}

		fin >> buf; // [OFFSET_MATRIX]
		for (int i = 0; i < boneCount; ++i) {
//			fin >> buf; // BoneOffSet
			fin >> boneOffset._11 >> boneOffset._12 >> boneOffset._13 >> boneOffset._14;
			fin >> boneOffset._21 >> boneOffset._22 >> boneOffset._23 >> boneOffset._24;
			fin >> boneOffset._31 >> boneOffset._32 >> boneOffset._33 >> boneOffset._34;
			fin >> boneOffset._41 >> boneOffset._42 >> boneOffset._43 >> boneOffset._44;

			boneOffsetsVector.push_back(boneOffset);
		}

		animaitionData.m_boneDataVector.reserve(boneCount);
		fin >> buf; // [ANIMATION_CLIPS]
		for (int clip = 0; clip < animationClips; ++clip) {
			fin >> buf; // AnimationClip 
			fin >> animationName;
//			fin >> buf;	// {
			
			for (int bone = 0; bone < boneCount; ++bone) {
				fin >> buf; // Bone			Bone0 : 351
				fin >> boneData.m_nAnimaitionKeys;
//				fin >> buf;	// {
		
				boneData.m_keyframeDataVector.reserve(boneData.m_nAnimaitionKeys);
				for (int frame = 0; frame < boneData.m_nAnimaitionKeys; ++frame) {
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

	/*
	cout << "Mesh Count : " << meshCount << endl;
	cout << "Vertex Count : " << vertexCount << endl;
	cout << "Index Count : " << indexCount << endl;

	cout << "PosVector Size : " << posVector.size() << " Capacity : " << posVector.capacity() << endl;
	cout << "NormalVector Size : " << normalVector.size() << " Capacity : " << normalVector.capacity() << endl;
	cout << "UvVector Size : " << uvVector.size() << " Capacity : " << uvVector.capacity() << endl;
	cout << "IndexVector Size : " << indexVector.size() << " Capacity : " << indexVector.capacity() << endl;

	for (auto i : posVector)
	cout << "Position : " << i.x << ", " << i.y << ", " << i.z << endl;

	for (auto i : normalVector)
	cout << "Normal : " << i.x << ", " << i.y << ", " << i.z << endl;

	for (auto i : uvVector)
	cout << "UV : " << i.x << ", " << i.y << endl;

	for (auto i : indexVector) {
	cout << "Index : " << i.x << ", " << i.y << ", " << i.z << endl;
	}
	*/

	m_nVertices = vertexCount;
	m_nIndices = indexCount;
	m_nBoneCount = boneCount;
	m_nAnimationClip = animationClips;

	m_pPositions = new XMFLOAT3[m_nVertices];
	m_pNormals = new XMFLOAT3[m_nVertices];
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
	for (int i = 0; i < m_nBoneCount; i++)
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

void CSkinnedMesh::MakeBoneMatrix(int nNowframe, int nBoneNum, vector<XMMATRIX> &SQTTransformVector) const
{
	XMMATRIX mtxBone;
	auto anim = m_animationMap.find(m_strClipName);
	
	if (anim->second.m_boneDataVector[nBoneNum].m_nAnimaitionKeys > 0){
		XMVECTOR vScale = XMLoadFloat3(&anim->second.m_boneDataVector[nBoneNum].m_keyframeDataVector[nNowframe].m_xmf3Scale);
		XMVECTOR vTranslate = XMLoadFloat3(&anim->second.m_boneDataVector[nBoneNum].m_keyframeDataVector[nNowframe].m_xmf3Translate);
		XMVECTOR vQuaternion = XMLoadFloat4(&anim->second.m_boneDataVector[nBoneNum].m_keyframeDataVector[nNowframe].m_xmf4Quaternion);

		mtxBone = XMMatrixAffineTransformation(vScale, XMVectorZero(), vQuaternion, vTranslate);
	}
	else
		mtxBone = XMMatrixIdentity();
	
	SQTTransformVector.push_back(mtxBone);
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

void CSkinnedMesh::UpdateBoneTransform(ID3D11DeviceContext *pd3dDeviceContext)
{
	vector<XMMATRIX> SQTTransformVector;

	for (int i = 0; i < m_nBoneCount; i++)
		MakeBoneMatrix(m_nNowFrameNum, i, SQTTransformVector);

	finalBoneVector.resize(m_nBoneCount);

	for (int i = 0; i < m_nBoneCount; i++)
	{
		XMMATRIX mtxOffset = XMLoadFloat4x4(&boneOffsetsVector[i]);
		XMMATRIX mtxToRoot = SQTTransformVector[i];

		finalBoneVector[i] = mtxOffset * mtxToRoot;
	}
}

void CSkinnedMesh::Update(float fTimeElapsed)
{
	m_fAnimationTime += fTimeElapsed;

	if (m_fAnimationTime > ANIMFRAMETIME)
	{
		if (m_nNowFrameNum < m_nMaxFrameNum - 1)
		{
			m_nNowFrameNum++;
			m_fAnimationTime = 0.0f;
		}
		else
		{
			m_nNowFrameNum = 0;
			m_fAnimationTime = 0.0f;
		}
	}
}


/*
// 다음 애니메이션을 위한 프레임으로 넘긴다.
// 추가적인 애니메이션 관리를 위해 마지막 프레임일 경우 true를 리턴한다.
bool CSkinnedMesh::FBXFrameAdvance(float fTimeElapsed)//2016.5.14 수정된 부분
{
	m_fFBXAnimationTime += fTimeElapsed;

	if (m_fFBXAnimationTime > 0.033333f)	// 0.0333333f초가 지나면 1프레임 올리자.
	{
		if (m_nFBXNowFrameNum < m_nFBXMaxFrameNum - 1)
		{
			m_nFBXNowFrameNum++;
			m_fFBXAnimationTime = 0.0f;//
			return false;//애니메이션이 끝났는가?
		}
		else
		{
			m_nFBXNowFrameNum = m_nFBXStartFrameNum;//수정됨
			m_fFBXAnimationTime = 0.0f;
			return true;//애니메이션이 끝났는가?
		}
	}
	else
		return false;//애니메이션이 끝났는가?
}

void CSkinnedMesh::SetAnimation(int nFBXAnimation) // 의미없는 코드부다. 조만간 삭제할 예정임
{
	m_nFBXAnimationNum = nFBXAnimation;
	m_nFBXNowFrameNum = 0;
	m_nFBXMaxFrameNum = s_ppBoneAnimationData[nFBXAnimation][1].m_nFrameCount;//2016.5.13 박종혁 0은 원래 1이었다.
																			  //m_nFBXMaxFrameNum = 81;
	m_fFBXAnimationTime = 0.0f;
}

void CSkinnedMesh::IdleSet()
{
	m_nFBXAnimationNum = 0;// ANIM_STATIC;
	m_fFBXAnimationTime = 0.0f;

	m_nFBXStartFrameNum = 0;
	m_nFBXMaxFrameNum = 50;
	m_nFBXNowFrameNum = m_nFBXStartFrameNum;
}

void CSkinnedMesh::WalkAnimationSet()
{
	m_nFBXAnimationNum = 0;// ANIM_STATIC;
	m_fFBXAnimationTime = 0.0f;

	m_nFBXStartFrameNum = 51;
	m_nFBXMaxFrameNum = 80;
	m_nFBXNowFrameNum = m_nFBXStartFrameNum;
}


void CSkinnedMesh::AnimationDestroy()
{
	if (s_pd3dxmtxBoneOffsets)
		delete[]s_pd3dxmtxBoneOffsets;
	if (s_ppBoneAnimationData)
	{
		for (int i = 0; i < s_nAnimationClip; i++)
		{
			for (int j = 0; j < s_nBoneCount; j++)
			{
				if (s_ppBoneAnimationData[i][j].m_nFrameCount != 0)
				{
					delete[] s_ppBoneAnimationData[i][j].m_pd3dxvTranslate;
					delete[] s_ppBoneAnimationData[i][j].m_pd3dxvScale;
					delete[] s_ppBoneAnimationData[i][j].m_pd3dxvQuaternion;
					delete[] s_ppBoneAnimationData[i][j].m_pfAniTime;
				}
			}
			delete[] s_ppBoneAnimationData[i];
		}
		delete[] s_ppBoneAnimationData;
	}
}
*/