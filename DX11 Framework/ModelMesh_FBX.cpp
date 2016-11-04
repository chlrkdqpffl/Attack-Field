#include "stdafx.h"
#include "ModelMesh_FBX.h"


CModelMesh_FBX::CModelMesh_FBX(const string& fileName, float size) : CMeshTexturedIlluminated(STATEOBJ_MGR->m_pd3dDevice.Get())
{
	bool isLoad = LoadFBXfromFile(fileName);
#if defined(DEBUG) || defined(_DEBUG)
	if (isLoad)
		cout << "File Load Success!! <" << fileName.c_str() << ">" << endl;
	else {
		cout << "File Load Error!! <" << fileName.c_str() << "> \t 파일 또는 경로를 확인하세요." << endl;
		return;
	}
#endif

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	if (size == 1.0f) {
		for (int i = 0; i < m_nVertices; ++i) {
			m_pd3dxvPositions[i] = posVector[i];
			m_pvNormals[i] = normalVector[i];
			m_pvTexCoords[i] = uvVector[i];
		}
	}
	else {
		for (int i = 0; i < m_nVertices; ++i) {
			XMStoreFloat3(&m_pd3dxvPositions[i], XMVectorScale(XMLoadFloat3(&posVector[i]), size));
			//		XMStoreFloat3(&m_pvNormals[i], XMVectorScale(XMLoadFloat3(&normalVector[i]), size));
			//		XMStoreFloat2(&m_pvTexCoords[i], XMVectorScale(XMLoadFloat2(&uvVector[i]), size));
			m_pvNormals[i] = normalVector[i];
			m_pvTexCoords[i] = uvVector[i];
		}
	}

	// Create Buffer
	m_pd3dPositionBuffer = CreateBuffer(STATEOBJ_MGR->m_pd3dDevice.Get(), sizeof(XMFLOAT3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(STATEOBJ_MGR->m_pd3dDevice.Get(), sizeof(XMFLOAT3), m_nVertices, m_pvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(STATEOBJ_MGR->m_pd3dDevice.Get(), sizeof(XMFLOAT2), m_nVertices, m_pvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	for (int i = 0, j = 0; i < m_nIndices / 3; ++i, j += 3) {
		m_pnIndices[j] = (UINT)indexVector[i].x;
		m_pnIndices[j + 1] = (UINT)indexVector[i].y;
		m_pnIndices[j + 2] = (UINT)indexVector[i].z;
	}

	m_pd3dIndexBuffer = CreateBuffer(STATEOBJ_MGR->m_pd3dDevice.Get(), sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	float max_x = m_pd3dxvPositions[0].x, max_y = m_pd3dxvPositions[0].y, max_z = m_pd3dxvPositions[0].z;
	for (int i = 0; i < m_nVertices; i++) {
		if (max_x < m_pd3dxvPositions[i].x) max_x = m_pd3dxvPositions[i].x;
		if (max_y < m_pd3dxvPositions[i].y) max_y = m_pd3dxvPositions[i].y;
		if (max_z < m_pd3dxvPositions[i].z) max_z = m_pd3dxvPositions[i].z;
	}
	float min_x = m_pd3dxvPositions[0].x, min_y = m_pd3dxvPositions[0].y, min_z = m_pd3dxvPositions[0].z;
	for (int i = 0; i < m_nVertices; i++) {
		if (min_x > m_pd3dxvPositions[i].x) min_x = m_pd3dxvPositions[i].x;
		if (min_y > m_pd3dxvPositions[i].y) min_y = m_pd3dxvPositions[i].y;
		if (min_z > m_pd3dxvPositions[i].z) min_z = m_pd3dxvPositions[i].z;
	}

	m_bcBoundingCube.Center = XMFLOAT3(0, 0, 0);
	m_bcBoundingCube.Extents = XMFLOAT3(max_x, max_y, max_z);
}

CModelMesh_FBX::~CModelMesh_FBX()
{
}

bool CModelMesh_FBX::LoadFBXfromFile(const string& fileName)
{
	ifstream fin(fileName);

	if (!fin.is_open())
		return false;

	int meshCount;
	int vertexCount, indexCount, boneCount, animationCount;
	XMFLOAT3 pos, normal, index;
	XMFLOAT2 uv;

	string buf;

	while (!fin.eof()) {
		fin >> buf;

		// FBX Meta Data
		if (buf == "MeshCount:")
			fin >> meshCount;
		// Mesh Data
		else if (buf == "VertexCount:") {
			fin >> vertexCount;
			posVector.reserve(vertexCount);
			normalVector.reserve(vertexCount);
			uvVector.reserve(vertexCount);
		}
		else if (buf == "IndexCount:") {
			fin >> indexCount;
			indexVector.reserve(indexCount);
		}
		else if (buf == "BoneCount:")
			fin >> boneCount;
		else if (buf == "AnimationClips:")
			fin >> animationCount;
		// Vertex Data

		else if (buf == "[VERTEX_DATA]") {
			for (int i = 0; i < vertexCount; ++i) {
				fin >> buf;		// Position:
				fin >> pos.x >> pos.y >> pos.z;
				fin >> buf;		// Normal: 
				fin >> normal.x >> normal.y >> normal.z;
				fin >> buf;		// UV: 
				fin >> uv.x >> uv.y;

				posVector.push_back(pos);
				normalVector.push_back(normal);
				uvVector.push_back(uv);

				// BonWegiht
				//				for (int i = 0; i < 10; ++i)
				//					fin >> buf;
			}
		}
		// Index Data
		else if (buf == "[INDEX_DATA]") {
			for (int i = 0; i < indexCount / 3; ++i) {
				fin >> index.x >> index.y >> index.z;

				indexVector.push_back(index);
			}
			fin >> buf;
			break;
		}
		//		else if (buf == "[BONE_HIERARCHY]") {
		//			cout << "끝";
		//			break;
	}
	fin.close();

	/*
	cout << "Mesh Count : " << meshCount << endl;
	cout << "Vertex Count : " << vertexCount << endl;
	cout << "Index Count : " << indexCount << endl;

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

	m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
	m_pvNormals = new XMFLOAT3[m_nVertices];
	m_pvTexCoords = new XMFLOAT2[m_nVertices];
	m_pnIndices = new UINT[m_nIndices];

	return true;
}