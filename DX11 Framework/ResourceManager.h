#pragma once
#include "SingletonManager.h"
#include "FbxMeshData.h"
#include "UserTag.h"

class CResourceManager : public CSingletonManager<CResourceManager>
{
	multimap<TextureTag, ID3D11ShaderResourceView*>		m_mapTexturePool;
	multimap<TextureTag, string>						m_mapPathPool;
	multimap<MeshTag, CFbxMeshData>						m_mapMeshPool;

public:
	CResourceManager();
	virtual ~CResourceManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	void AddResourece(const TextureTag& textureTag, const string& source);
	void AddResourece(const MeshTag& meshTag, const string& source);
	void AddResourecePath(const TextureTag& textureTag, const string& source);

	wstring FindResourcePath(const TextureTag& textureTag);
	ID3D11ShaderResourceView* CloneShaderResourceView(const TextureTag& textureTag);
	CFbxMeshData CloneFbxMeshData(const MeshTag& meshTag);

	void ShowImageInfo(const string& source);
};