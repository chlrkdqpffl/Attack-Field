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
	cout << "================================== Map Data Loading ========================================" << endl << endl;

	// Road
	AddMapData(ObjectTag::eRoad1,						"../Assets/MapData/Road1.txt");
	AddMapData(ObjectTag::eRoad2,						"../Assets/MapData/Road2.txt");
	AddMapData(ObjectTag::eCenterRoad,					"../Assets/MapData/Center_Road.txt");
	AddMapData(ObjectTag::eCrossRoad,					"../Assets/MapData/Cross_Road.txt");
	
	// Building
	AddMapData(ObjectTag::eBuilding19,					"../Assets/MapData/Building19.txt");
	AddMapData(ObjectTag::eBuilding20,					"../Assets/MapData/Building20.txt");
	AddMapData(ObjectTag::eBuilding21,					"../Assets/MapData/Building21.txt");
	AddMapData(ObjectTag::eBuilding22,					"../Assets/MapData/Building22.txt");
	AddMapData(ObjectTag::eBuilding30,					"../Assets/MapData/Building30.txt");
	AddMapData(ObjectTag::eBuilding33,					"../Assets/MapData/Building33.txt");
	AddMapData(ObjectTag::eBuilding34,					"../Assets/MapData/Building34.txt");
	AddMapData(ObjectTag::eBuilding77,					"../Assets/MapData/Building77.txt");
	AddMapData(ObjectTag::eBuilding78,					"../Assets/MapData/Building78.txt");
	AddMapData(ObjectTag::eBuilding100,					"../Assets/MapData/Building100.txt");
	AddMapData(ObjectTag::eBuilding103,					"../Assets/MapData/Building103.txt");
	AddMapData(ObjectTag::eBuilding104,					"../Assets/MapData/Building104.txt");
	AddMapData(ObjectTag::eParkingLot,					"../Assets/MapData/Parking.txt");
	AddMapData(ObjectTag::eHotel,						"../Assets/MapData/Hotel.txt");
	AddMapData(ObjectTag::eBarricade,					"../Assets/MapData/Barricaed.txt");

	// Etc
	AddMapData(ObjectTag::eBusStop,						"../Assets/MapData/Bus_Stop.txt");
	AddMapData(ObjectTag::eStreetLamp,					"../Assets/MapData/Street_lamp.txt");
	AddMapData(ObjectTag::eBench,						"../Assets/MapData/Bench.txt");
	AddMapData(ObjectTag::eGrass,						"../Assets/MapData/Grass.txt");
	AddMapData(ObjectTag::eSideWalk1,					"../Assets/MapData/SideWalk.txt");
	AddMapData(ObjectTag::eSideWalk2,					"../Assets/MapData/SideWalk2.txt");
	cout << endl;
	cout << "=============================== Map Data Loading Complete ==================================" << endl;
	cout << "============================================================================================" << endl << endl;
}

void CMapDataManager::ReleseManager()
{
}

bool CMapDataManager::AddMapData(const ObjectTag& tag, const string& source)
{
	ifstream fin(source);
	vector<MapData> vecMapData;
	MapData data;

	cout << "  Map Data File Loading < " + source + " > ";

	if (!fin.is_open()) {
		ShowTaskFail("\t Error!! \t\t 파일 또는 경로를 확인하세요.");
		return false;
	}

	while (!fin.eof()) {
		fin >> data.m_Position.x >> data.m_Position.y >> data.m_Position.z;
		fin >> data.m_Rotation.x >> data.m_Rotation.y >> data.m_Rotation.z;
		fin >> data.m_Scale.x >> data.m_Scale.y >> data.m_Scale.z;
	
		vecMapData.push_back(data);
	}
	vecMapData.pop_back();
	fin.close();

	m_mapDataPool.insert(make_pair(tag, vecMapData));

	ShowTaskSuccess("\t Success!!");
	return true;
}

vector<MapData> CMapDataManager::GetDataVector(const ObjectTag& tag) const
{
	auto findResource = m_mapDataPool.find(tag);

	if (findResource == m_mapDataPool.end())
		MessageBox(NULL, s_to_ws("Object Tag : " + to_string(static_cast<int>(tag))).c_str(), L"Resource Error", MB_OK);

	return findResource->second;
}

XMFLOAT3 CMapDataManager::GetPosition(const ObjectTag& tag, const int index) const
{
	auto findResource = m_mapDataPool.find(tag);

	// Pool에 해당 데이터가 존재하지 않는다.
	assert(findResource != m_mapDataPool.end());

	return findResource->second[index].m_Position;
}

XMFLOAT3 CMapDataManager::GetRotation(const ObjectTag& tag, const int index) const
{
	auto findResource = m_mapDataPool.find(tag);

	// Pool에 해당 데이터가 존재하지 않는다.
	assert(findResource != m_mapDataPool.end());

	return findResource->second[index].m_Rotation;
}

XMFLOAT3 CMapDataManager::GetScale(const ObjectTag& tag, const int index) const
{
	auto findResource = m_mapDataPool.find(tag);

	// Pool에 해당 데이터가 존재하지 않는다.
	assert(findResource != m_mapDataPool.end());

	return findResource->second[index].m_Scale;
}