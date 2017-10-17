#pragma once
#include "SingletonManager.h"

struct MapData {
	XMFLOAT3	m_Position;
	XMFLOAT3	m_Rotation;
	XMFLOAT3	m_Scale;
};

class CMapDataManager : public CSingletonManager<CMapDataManager>
{
	unordered_multimap<ObjectTag, vector<MapData>>		m_mapDataPool;
								
public:
	CMapDataManager();
	virtual ~CMapDataManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	bool AddMapData(const ObjectTag& tag, const string& source);

	vector<MapData> GetDataVector(const ObjectTag& tag) const;
	XMFLOAT3 GetPosition(const ObjectTag& tag, const int index) const;
	XMFLOAT3 GetRotation(const ObjectTag& tag, const int index) const;
	XMFLOAT3 GetScale(const ObjectTag& tag, const int index) const;
};