#pragma once
#include "SingletonManager.h"
#include "FbxMeshData.h"
#include "UserTag.h"

class CResourceManager : public CSingletonManager<CResourceManager>
{
	multimap<Resource_TextrueTag, string>			textureMap;

	multimap<MeshTag, CFbxMeshData>					m_mapMeshPool;

public:
	CResourceManager();
	virtual ~CResourceManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	void AddResourece(Resource_TextrueTag resourceTag, string source);
	void AddResourece(MeshTag meshTag, string source);

	wstring FindResourcePath(Resource_TextrueTag resourceTag);

	ID3D11ShaderResourceView* FindResourceAndCreateSRV(Resource_TextrueTag);
	CFbxMeshData CloneFbxMeshData(MeshTag meshTag);

	void ShowImageInfo(Resource_TextrueTag resourceTag);
};