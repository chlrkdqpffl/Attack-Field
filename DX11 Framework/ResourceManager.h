#pragma once
#include "SingletonManager.h"
#include "FbxModelSkinnedMesh.h"
#include "UserTag.h"

class CFbxMeshData
{
	vector<XMFLOAT3>	posVector;
	vector<XMFLOAT3>	normalVector;
	vector<XMFLOAT3>	tangentVector;
	vector<XMFLOAT3>	indexVector;
	vector<XMFLOAT2>	uvVector;
};

class CResourceManager : public CSingletonManager<CResourceManager>
{
	multimap<Resource_TextrueTag, string> textureMap;
	multimap<MeshData::Resource_MeshTag, string> meshMap;

	multimap<MeshData::Resource_MeshTag, string> m_mapMeshPool;

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