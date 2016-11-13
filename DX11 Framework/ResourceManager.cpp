#include "stdafx.h"
#include "ResourceManager.h"

CResourceManager::CResourceManager()
{
}


CResourceManager::~CResourceManager()
{
}

void CResourceManager::InitializeManager()
{
	// ------------------------ Texture ---------------------------- //

	// Model
	AddResourece(eTexture_DarkFighterColor,		"../Assets/FBX Model/Dark Fighter 6/dark_fighter_6_color.png");

	// UI
	AddResourece(eTexture_Title,				"../Assets/Image/UI/BackGround2.jpg");

	// Environment
	AddResourece(eTexture_Terrain,				"../Assets/Image/Terrain/Base_Texture.jpg");
	AddResourece(eTexture_TerrainDetail,		"../Assets/Image/Terrain/Detail_Texture_7.jpg");
	AddResourece(eTexture_HeightMap,			"../Assets/Image/Terrain/HeightMap.raw");

	//	AddResourece(eTexture_Terrain,				"../Assets/Image/Terrain/TerrainDiffuse2.jpg");
	//	AddResourece(eTexture_TerrainDetail,		"../Assets/Image/Terrain/Detail_Texture_7.jpg");
	//	AddResourece(eTexture_HeightMap,			"../Assets/Image/Terrain/TerrainHeightMap2.raw");
	AddResourece(eTexture_TerrainNormal,		"../Assets/Image/Terrain/TerrainNormal2.jpg");
	

	AddResourece(eTexture_Water,				"../Assets/Image/Terrain/water.jpg");
	AddResourece(eTexture_WaterDetail,			"../Assets/Image/Terrain/Water_Detail_Texture_0.dds");

	//  etc
	AddResourece(eTexture_Stone,				"../Assets/Image/Miscellaneous/stones.dds");
	AddResourece(eTexture_StoneNormal,			"../Assets/Image/Miscellaneous/stones_nmap.dds");
	AddResourece(eTexture_Bricks,				"../Assets/Image/Miscellaneous/bricks.dds");
	AddResourece(eTexture_BricksNormal,			"../Assets/Image/Miscellaneous/bricks_nmap.dds");
	AddResourece(eTexture_Wood,					"../Assets/Image/Miscellaneous/Wood01.jpg");



	// ------------------------ Mesh ---------------------------- //
	AddResourece(eMesh_DarkFighter,				"../Assets/FBX Model/Dark Fighter 6/dark_fighter_6.data");
}

void CResourceManager::ReleseManager()
{
}


void CResourceManager::AddResourece(Resource_TextrueTag resourceTag, string source)
{
	textureMap.insert(make_pair(resourceTag, source));

	if (1 < textureMap.count(resourceTag)) {
		cout << "TextureTag No." << resourceTag << " : 다중 텍스쳐" << endl;
	}
}

void CResourceManager::AddResourece(Resource_MeshTag resourceTag, string source)
{
	meshMap.insert(make_pair(resourceTag, source));

	if (1 < meshMap.count(resourceTag)) {
		cout << "MeshTag No." << resourceTag << " : 다중 메쉬" << endl;
	}
}

wstring CResourceManager::FindResourcePath(Resource_TextrueTag resourceTag)
{
	auto find = textureMap.find(resourceTag);
	if (find == textureMap.end()) {
		cout << "Resource Texture Tag " << resourceTag << " is not exist." << endl;
		return nullptr;
	}
	string str = (*find).second.c_str();
	wstring wstr;
	wstr.assign(str.begin(), str.end());

	return wstr;
}

string CResourceManager::FindResourcePath(Resource_MeshTag resourceTag)
{
	auto find = meshMap.find(resourceTag);
	if (find == meshMap.end()) {
		cout << "Resource Mesh Tag " << resourceTag << " is not exist." << endl;
		return nullptr;
	}
	return (*find).second;
}

ID3D11ShaderResourceView* CResourceManager::FindResourceAndCreateSRV(Resource_TextrueTag resourceTag)
{
	auto findResource = textureMap.find(resourceTag);
	if (findResource == textureMap.end()) {
		cout << "Resource Texture Tag " << resourceTag << " is not exist." << endl;
		return nullptr;
	}
	string str = (*findResource).second.c_str();
	wstring wstr;
	wstr.assign(str.begin(), str.end());
	
	HRESULT hResult;
	ID3D11ShaderResourceView *pd3dsrvTexture = NULL;
	D3DX11CreateShaderResourceViewFromFile(STATEOBJ_MGR->g_pd3dDevice.Get(), wstr.c_str(), NULL, NULL, &pd3dsrvTexture, &hResult);
	
#if defined(DEBUG) || defined(_DEBUG)
	if ((HRESULT)hResult >= 0)
		cout << "File Load Success!! <" << (*findResource).second << ">" << endl;
	else 
		cout << "File Load Error!! <" << (*findResource).second << "> \t 파일 또는 경로를 확인하세요." << endl;
#endif

	return pd3dsrvTexture;
}