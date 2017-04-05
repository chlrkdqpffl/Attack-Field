#pragma once
#include "SingletonManager.h"

struct MapData {
	XMFLOAT3	m_Position;
	XMFLOAT3	m_Rotation;
	XMFLOAT3	m_Scale;
};

class CMapDataManager : public CSingletonManager<CMapDataManager>
{
	multimap<MeshTag, vector<MapData>>		m_mapDataPool;
								
public:
	CMapDataManager();
	virtual ~CMapDataManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	bool AddMapData(const MeshTag& tag, const string& source);

	vector<MapData> GetDataVector(const MeshTag& tag) const;
	XMFLOAT3 GetPosition(const MeshTag& tag, const int index) const;
	XMFLOAT3 GetRotation(const MeshTag& tag, const int index) const;
	XMFLOAT3 GetScale(const MeshTag& tag, const int index) const;
};