#pragma once
#include "SingletonManager.h"
#include "Object.h"

class CCollisionManager : public CSingletonManager<CCollisionManager>
{
	DWORD								m_dwInitTime = 0;
public:
	vector<CGameObject*>				m_vecStaticMeshContainer;
	vector<CGameObject*>				m_vecDynamicMeshContainer;

public:
	CCollisionManager();
	virtual ~CCollisionManager();

	virtual void InitializeManager() override;
	virtual void UpdateManager() override;
	virtual void ReleseManager() override;
};

