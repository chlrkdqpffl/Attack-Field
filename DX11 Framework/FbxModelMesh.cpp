#include "stdafx.h"
#include "FbxModelMesh.h"


CFbxModelMesh::CFbxModelMesh(ID3D11Device *pd3dDevice, const string& fileName, bool isTangent, float size) 
	: m_bTangent(isTangent), m_fileName(fileName), m_fModelSize(size)
{
}

CFbxModelMesh::~CFbxModelMesh()
{
	if (m_pNormals) delete[] m_pNormals;
	if (m_pTangents) delete[] m_pTangents;
	if (m_pTexCoords) delete[] m_pTexCoords;
	ReleaseCOM(m_pd3dTangentBuffer);
}

void CFbxModelMesh::Initialize(ID3D11Device *pd3dDevice)
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
		}
	}
	else {
		for (int i = 0; i < m_nVertices; ++i) {
			XMStoreFloat3(&m_pPositions[i], XMVectorScale(XMLoadFloat3(&posVector[i]), m_fModelSize));
			//		XMStoreFloat3(&m_pvNormals[i], XMVectorScale(XMLoadFloat3(&normalVector[i]), size));
			//		XMStoreFloat2(&m_pvTexCoords[i], XMVectorScale(XMLoadFloat2(&uvVector[i]), size));
			m_pNormals[i] = normalVector[i];
			m_pTexCoords[i] = uvVector[i];
		}
	}

	// Create Buffer
	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(XMFLOAT2), m_nVertices, m_pTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	for (UINT i = 0, j = 0; i < m_nIndices / 3; ++i, j += 3) {
		m_pnIndices[j] = (UINT)indexVector[i].x;
		m_pnIndices[j + 1] = (UINT)indexVector[i].y;
		m_pnIndices[j + 2] = (UINT)indexVector[i].z;
	}

	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

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

	m_bcBoundingBox.Center = XMFLOAT3(0, 0, 0);
	m_bcBoundingBox.Extents = XMFLOAT3(max_x, max_y, max_z);


	DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	DXUT_SetDebugName(m_pd3dNormalBuffer, "Normal");
	//	DXUT_SetDebugName(m_pd3dTangentBuffer, "Tangent");		 계산 안하고 있음
	DXUT_SetDebugName(m_pd3dTexCoordBuffer, "TexCoord");
	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");
}

bool CFbxModelMesh::LoadFBXfromFile(const string& fileName)
{
	//ifstream fin(fileName);
	ifstream fin(fileName, ios::binary);

	if (!fin.is_open())
		return false;

	UINT meshCount = 0;
	UINT vertexCount = 0, indexCount = 0, boneCount = 0, animationCount = 0;
	XMFLOAT3 pos, normal, tangent, index;
	XMFLOAT2 uv;
	XMFLOAT3 bBoxCenter, bBoxExtents, bBoxMax;

	string buf;

	while (!fin.eof()) {
		fin >> buf; // [FBX_META_DATA]
		fin >> buf >> meshCount;
		
		fin >> buf; // [MESH_DATA]
		fin >> vertexCount;
		{
			posVector.reserve(vertexCount);
			normalVector.reserve(vertexCount);
			if (m_bTangent)
				tangentVector.reserve(vertexCount);
			uvVector.reserve(vertexCount);
		}
		fin >> indexCount;
		{
			indexVector.reserve(indexCount / 3);
		}
		fin >> boneCount;
		fin >> animationCount;

		fin >> buf; // [BBox_DATA]
		fin >> buf; // Center
		fin >> bBoxCenter.x >> bBoxCenter.y >> bBoxCenter.z;
		fin >> buf; // Min
		fin >> buf >> buf >> buf;
		fin >> buf; // Max
		fin >> bBoxMax.x >> bBoxMax.y >> bBoxMax.z;

		fin >> buf; // [VERTEX_DATA]
		if (m_bTangent) {
			for (UINT i = 0; i < vertexCount; ++i) {
//				fin >> buf;	// Position
				fin >> pos.x >> pos.y >> pos.z;
//				fin >> buf;	// Normal
				fin >> normal.x >> normal.y >> normal.z;
//				fin >> buf; // Tangent
				fin >> tangent.x >> tangent.y >> tangent.z;
//				fin >> buf;	// UV
				fin >> uv.x >> uv.y;

				posVector.push_back(pos);
				normalVector.push_back(normal);
				tangentVector.push_back(tangent);
				uvVector.push_back(uv);
			}
		}
		else
		{
			for (UINT i = 0; i < vertexCount; ++i) {
//				fin >> buf;	// Position
				fin >> pos.x >> pos.y >> pos.z;
//				fin >> buf;	// Normal
				fin >> normal.x >> normal.y >> normal.z;
//				fin >> buf;	// UV
				fin >> uv.x >> uv.y;

				posVector.push_back(pos);
				normalVector.push_back(normal);
				uvVector.push_back(uv);
			}
		}

		fin >> buf; // [INDEX_DATA]
		for (UINT i = 0; i < indexCount / 3; ++i) {
			fin >> index.x >> index.y >> index.z;

			indexVector.push_back(index);
		}

		// End
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

	m_pPositions = new XMFLOAT3[m_nVertices];
	m_pNormals = new XMFLOAT3[m_nVertices];
	if (m_bTangent)
		m_pTangents = new XMFLOAT3[m_nVertices];
	m_pTexCoords = new XMFLOAT2[m_nVertices];
	m_pnIndices = new UINT[m_nIndices];

	return true;
}