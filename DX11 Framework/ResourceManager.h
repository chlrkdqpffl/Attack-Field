#pragma once
#include "SingletonManager.h"
#include "ModelSkinnedMesh.h"
#include "UserTag.h"

class CResourceManager : public CSingletonManager<CResourceManager>
{
	multimap<Resource_TextrueTag, string> textureMap;
	multimap<MeshData::Resource_MeshTag, string> meshMap;

public:
	CResourceManager();
	virtual ~CResourceManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	void AddResourece(Resource_TextrueTag resourceTag, string source);
	void AddResourece(MeshData::Resource_MeshTag resourceTag, string source);

	wstring FindResourcePath(Resource_TextrueTag resourceTag);
	string FindResourcePath(MeshData::Resource_MeshTag resourceTag);

	ID3D11ShaderResourceView* FindResourceAndCreateSRV(Resource_TextrueTag);

	CSkinnedMesh* CloneSkinnedMesh();
	void ShowImageInfo(Resource_TextrueTag resourceTag);

};