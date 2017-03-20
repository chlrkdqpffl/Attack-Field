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
	cout << "=============================================================================================" << endl;
	cout << "====================================== Texture Loading ======================================" << endl;
	// Model
	AddResourece(TextureTag::eDarkFighterD,			"../Assets/FBX Model/Dark Fighter 6/dark_fighter_6_color.png");
	AddResourece(TextureTag::eDarkFighterN,			"../Assets/FBX Model/Dark Fighter 6/dark_fighter_6_NM.jpg");
	
	AddResourece(TextureTag::eTerroristD,			"../Assets/FBX Model/Character/Terrorist/TerrorristD.dds");
	AddResourece(TextureTag::eTerroristND,			"../Assets/FBX Model/Character/Terrorist/TerrorristNM_D.dds.dds");
	AddResourece(TextureTag::ePoliceD,				"../Assets/FBX Model/Character/Police/AllTex.PNG");
	
	AddResourece(TextureTag::eRifleD,				"../Assets/FBX Model/Weapon/Rifle/rifle.bmp");
	AddResourece(TextureTag::eRifleN,				"../Assets/FBX Model/Weapon/Rifle/RifleNM.png");
	
	// UI
	AddResourece(TextureTag::eTitleD,				"../Assets/Image/UI/BackGround2.jpg");

	// Environment
	AddResourece(TextureTag::eTerrainD,				"../Assets/Image/Terrain/Base_Texture.jpg");
	AddResourece(TextureTag::eTerrainDetailD,		"../Assets/Image/Terrain/Detail_Texture_7.jpg");
	AddResourecePath(TextureTag::eTerrainH,			"../Assets/Image/Terrain/HeightMap.raw");

	//	AddResourece(eTerrain,							"../Assets/Image/Terrain/TerrainDiffuse2.jpg");
	//	AddResourece(eTerrainDetail,					"../Assets/Image/Terrain/Detail_Texture_7.jpg");
	//	AddResourece(eHeightMap,						"../Assets/Image/Terrain/TerrainHeightMap2.raw");
	AddResourece(TextureTag::eTerrainN,				"../Assets/Image/Terrain/TerrainNormal2.jpg");
	

	AddResourece(TextureTag::eWaterD,				"../Assets/Image/Terrain/water.jpg");
	AddResourece(TextureTag::eWaterDetailD,			"../Assets/Image/Terrain/Water_Detail_Texture_0.dds");

	// Building
	AddResourece(TextureTag::eBuilding18,			"../Assets/FBX Model/novotel.jpg");


	// Etc
	AddResourece(TextureTag::eStoneD,				"../Assets/Image/Miscellaneous/stones.dds");
	AddResourece(TextureTag::eStoneN,				"../Assets/Image/Miscellaneous/stones_nmap.dds");
	AddResourece(TextureTag::eStone02D,				"../Assets/Image/Miscellaneous/Stone02_Diffuse.jpg");
	AddResourece(TextureTag::eStone02N,				"../Assets/Image/Miscellaneous/Stone02_nmap.dds");
	AddResourece(TextureTag::eBricksD,				"../Assets/Image/Miscellaneous/bricks.dds");
	AddResourece(TextureTag::eBricksN,				"../Assets/Image/Miscellaneous/bricks_nmap.dds");
	AddResourece(TextureTag::eWallD,				"../Assets/Image/Miscellaneous/wall.jpg");
	AddResourece(TextureTag::eWallND,				"../Assets/Image/Miscellaneous/wall_NM_height.dds");
	

	AddResourece(TextureTag::eTest,					"../Assets/FBX Model/Soldier_1_albedo.png");

	cout << endl;
	cout << "==========================================================================================" << endl;
	cout << "====================================== Mesh Loading ======================================" << endl;

	// Character
	AddResourece(MeshTag::eDarkFighter,				"../Assets/FBX Model/Dark Fighter 6/dark_fighter_6.model");

	AddResourece(MeshTag::ePolice,					"../Assets/FBX Model/Character/Police/Police.model");
	AddResourece(MeshTag::eTerrorist,				"../Assets/FBX Model/Character/Terrorist/Terrorist.model");
	AddResourece(MeshTag::eRifle,					"../Assets/FBX Model/Weapon/Rifle/rifle(120).model");

	// Object
//	AddResourece(MeshTag::eRoad,					"../Assets/FBX Model/Road.model");
	
	// Building

	// Etc
	AddResourece(MeshTag::eTest,					"../Assets/FBX Model/Soldier.model");
	

	cout << endl;
	cout << "=============================== Resource Loading Complete ==================================" << endl;
	cout << "============================================================================================" << endl;
}

void CResourceManager::ReleseManager()
{
}

void CResourceManager::AddResourece(const TextureTag& textureTag, const string& source)
{
	cout << "  File Loading < " + source + " > ";
	
	HRESULT hResult;
	ID3D11ShaderResourceView *pd3dsrvTexture = NULL;
	D3DX11CreateShaderResourceViewFromFile(STATEOBJ_MGR->g_pd3dDevice, s_to_ws(source).c_str(), NULL, NULL, &pd3dsrvTexture, &hResult);

	if ((HRESULT)hResult >= 0)
		ShowTaskSuccess("\t Success!!");
	else
		ShowTaskFail("\t Error!! \t\t 파일 또는 경로를 확인하세요.");

	string str = "TextureTag : " + to_string(static_cast<int>(textureTag));
	DXUT_SetDebugName(pd3dsrvTexture, str.c_str());

	m_mapTexturePool.insert(make_pair(textureTag, pd3dsrvTexture));

	// 한 태그에 여러개 등록되었음
	assert(m_mapTexturePool.count(textureTag) <= 1);
}

void CResourceManager::AddResourece(const MeshTag& meshTag, const string& source)
{
	CFbxMeshData meshData;

	cout << "  File Loading < " + source + " > ";

	if (MeshTag::MaxSkinnedMesh < meshTag) {
		if (meshData.LoadFbxModelDatafromFile(source))
			ShowTaskSuccess("\t Success!!");
		else {
			ShowTaskFail("\t Error!! \t 파일 또는 경로를 확인하세요.");
			return;
		}
	}
	else
	{
		if (meshData.LoadFbxModelSkinnedDatafromFile(source))
			ShowTaskSuccess("\t Success!!");
		else {
			ShowTaskFail("\t Error!! \t 파일 또는 경로를 확인하세요.");
			return;
		}
	}

	m_mapMeshPool.insert(make_pair(meshTag, meshData));

	// 한 태그에 여러개 등록되었음
	assert(m_mapMeshPool.count(meshTag) <= 1);
}

void CResourceManager::AddResourecePath(const TextureTag& textureTag, const string& source)
{
	m_mapPathPool.insert(make_pair(textureTag, source));

	// 한 태그에 여러개 등록되었음
	assert(m_mapPathPool.count(textureTag) <= 1);
}

wstring CResourceManager::FindResourcePath(const TextureTag& textureTag)
{
	auto findResource = m_mapPathPool.find(textureTag);

	// Pool에 해당 데이터가 존재하지 않는다.
	assert(findResource != m_mapPathPool.end());

	return s_to_ws((*findResource).second.c_str());
}


ID3D11ShaderResourceView* CResourceManager::CloneShaderResourceView(const TextureTag& textureTag)
{
	auto findResource = m_mapTexturePool.find(textureTag);

	// Pool에 해당 데이터가 존재하지 않는다.
	assert(findResource != m_mapTexturePool.end());

	return (*findResource).second;
}

CFbxMeshData CResourceManager::CloneFbxMeshData(const MeshTag& meshTag) 
{
	auto findResource = m_mapMeshPool.find(meshTag);

	// Pool에 해당 데이터가 존재하지 않는다.
	assert(findResource != m_mapMeshPool.end());

	return (*findResource).second;
}

void CResourceManager::ShowImageInfo(const string& source)
{
	HRESULT hResult;
	D3DX11_IMAGE_INFO info;

	D3DX11GetImageInfoFromFile(s_to_ws(source).c_str(), nullptr, &info, &hResult);

	if ((HRESULT)hResult >= 0) {
		cout << endl << " ===== < Image File Info > =====" << endl;
		cout << " Image Name	: " << source << endl;
		cout << " Array Size	: " << info.ArraySize << endl;
		cout << " Depth		: " << info.Depth << endl;
		cout << " Width		: " << info.Width << endl;
		cout << " Height		: " << info.Height << endl;
		cout << " MipLevels	: " << info.MipLevels << endl;
		cout << " Format		: " << info.Format << endl;
		cout << " ===============================" << endl << endl;
	}
	else
		ShowTaskFail("File Loading < " + source + " > \t Error!! \t\t 파일 또는 경로를 확인하세요.");
}
