#include "stdafx.h"
#include "MapDataManager.h"


CMapDataManager::CMapDataManager()
{
}

CMapDataManager::~CMapDataManager()
{
}

void CMapDataManager::InitializeManager()
{
	cout << "============================================================================================" << endl;
	cout << "================================== Map Data Loading ========================================" << endl;

	AddMapData(MeshTag::eRoad,						"../Assets/MapData/Road.txt");
	AddMapData(MeshTag::eBuilding21,				"../Assets/MapData/18 floor building.txt");
	AddMapData(MeshTag::eBuilding50,				"../Assets/MapData/23floor building.txt");


	cout << "=============================== Map Data Loading Complete ==================================" << endl;
	cout << "============================================================================================" << endl << endl;
}

void CMapDataManager::ReleseManager()
{
}

bool CMapDataManager::AddMapData(const MeshTag& tag, const string& source)
{
	ifstream fin(source);
	vector<MapData> vecMapData;
	MapData data;

	if (!fin.is_open())
		return false;

	while (!fin.eof()) {
		fin >> data.m_Position.x >> data.m_Position.y >> data.m_Position.z;
		fin >> data.m_Rotation.x >> data.m_Rotation.y >> data.m_Rotation.z;
		fin >> data.m_Scale.x >> data.m_Scale.y >> data.m_Scale.z;

		vecMapData.push_back(data);
	}

	fin.close();

	m_mapDataPool.insert(make_pair(tag, vecMapData));

	return true;
}

vector<MapData> CMapDataManager::GetDataVector(const MeshTag& tag) const
{
	auto findResource = m_mapDataPool.find(tag);

	// Pool에 해당 데이터가 존재하지 않는다.
	assert(findResource != m_mapDataPool.end());

	return findResource->second;
}

XMFLOAT3 CMapDataManager::GetPosition(const MeshTag& tag, const int index) const
{
	auto findResource = m_mapDataPool.find(tag);

	// Pool에 해당 데이터가 존재하지 않는다.
	assert(findResource != m_mapDataPool.end());

	return findResource->second[index].m_Position;
}

XMFLOAT3 CMapDataManager::GetRotation(const MeshTag& tag, const int index) const
{
	auto findResource = m_mapDataPool.find(tag);

	// Pool에 해당 데이터가 존재하지 않는다.
	assert(findResource != m_mapDataPool.end());

	return findResource->second[index].m_Rotation;
}

XMFLOAT3 CMapDataManager::GetScale(const MeshTag& tag, const int index) const
{
	auto findResource = m_mapDataPool.find(tag);

	// Pool에 해당 데이터가 존재하지 않는다.
	assert(findResource != m_mapDataPool.end());

	return findResource->second[index].m_Scale;
}