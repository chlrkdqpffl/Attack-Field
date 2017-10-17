#pragma once
#include "SingletonManager.h"
#include "FbxMeshData.h"
#include "UserTag.h"
#include "Mesh.h"

class CResourceManager : public CSingletonManager<CResourceManager>
{
	bool	m_bIsLoading = false;

	unordered_map<TextureTag, ID3D11ShaderResourceView*>		m_mapTexturePool;
	unordered_map<TextureTag, string>							m_mapPathPool;
	unordered_map<MeshTag, CFbxMeshData>						m_mapFbxMeshPool;

public:
	CResourceManager() {}
	virtual ~CResourceManager() {}

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	void AddResourece(const TextureTag& textureTag, const string& source);
	void AddResourece(const MeshTag& meshTag, const string& source);
	void AddResourecePath(const TextureTag& textureTag, const string& source);
	
	wstring FindResourcePath(const TextureTag& textureTag);
	ID3D11ShaderResourceView* CloneShaderResourceView(const TextureTag& textureTag);
	CFbxMeshData CloneFbxMeshData(const MeshTag& meshTag);
	
	void ShowImageInfo(const string& source);

	void LoadResourceTitleScene();
	bool LoadResourceAll();
private:
	void LoadEnvironmentData();
	void LoadUIData();
	void LoadSpriteData();
	void LoadEtcData();
	void LoadMapData();
	void LoadCharacterData();
};