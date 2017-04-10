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
	LoadResourceTitleScene();
}

void CResourceManager::ReleseManager()
{
	for (auto& index = m_mapNoneFbxMeshPool.begin(); index != m_mapNoneFbxMeshPool.end(); ++index)
		SafeDelete(index->second);
}

void CResourceManager::LoadResourceTitleScene()
{
	// BackGround Image
	AddResourece(TextureTag::eTitleBackGroundD,		"../Assets/Image/UI/TitleScene.jpg");
	AddResourece(TextureTag::eLoadingBackGroundD,	"../Assets/Image/UI/LoadingScene.jpg");

	// Character
//	AddResourece(MeshTag::eTerrorist,				"../Assets/FBX Model/Character/Terrorist/Terrorist(use).model");

	// UI
	AddResourece(TextureTag::eStartButtonOn,		"../Assets/Image/UI/StartOn.png");
	AddResourece(TextureTag::eStartButtonOff,		"../Assets/Image/UI/StartOff.png");
	AddResourece(TextureTag::eExitButtonOn,			"../Assets/Image/UI/ExitOn.png");
	AddResourece(TextureTag::eExitButtonOff,		"../Assets/Image/UI/ExitOff.png");
}

bool CResourceManager::LoadResourceAll()
{
	cout << "=============================================================================================" << endl;
	cout << "===================================== Resource Loading ======================================" << endl;
	
	cout << " ----- Character Data -----" << endl;
	LoadCharacterData();

	cout << " ----- Map Data -----" << endl;
	LoadMapData();
	
	cout << " ----- Environment Data -----" << endl;
	LoadEnvironmentData();
	
	cout << " ----- UI Data -----" << endl;
	LoadUIData();
	
	cout << " ----- Etc Data -----" << endl;
	LoadEtcData();

	cout << endl;
	cout << "=============================== Resource Loading Complete ==================================" << endl;
	cout << "============================================================================================" << endl << endl;

	return true;
}

void CResourceManager::LoadEnvironmentData()
{
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
}

void CResourceManager::LoadUIData()
{
	AddResourece(TextureTag::eCaptureArea,		 "../Assets/Image/UI/CaptureArea.png");
}

void CResourceManager::LoadMapData()
{
	// ============== Building ============== //
	AddResourece(TextureTag::eBuilding16D,		"../Assets/FBX Model/Building/16building/16buildingD.jpg");
	AddResourece(TextureTag::eBuilding19D,		"../Assets/FBX Model/Building/19building/19buildingD.jpg");
	AddResourece(TextureTag::eBuilding20D,		"../Assets/FBX Model/Building/20building/20buildingD.jpg");
	AddResourece(TextureTag::eBuilding21D,		"../Assets/FBX Model/Building/21building/21buildingD.jpg");
	AddResourece(TextureTag::eBuilding22D,		"../Assets/FBX Model/Building/22building/22buildingD.jpg");
	AddResourece(TextureTag::eBuilding26D,		"../Assets/FBX Model/Building/26building/26buildingD.jpg");
	AddResourece(TextureTag::eBuilding29D,		"../Assets/FBX Model/Building/29building/29buildingD.jpg");
	AddResourece(TextureTag::eBuilding30D,		"../Assets/FBX Model/Building/30building/30buildingD.jpg");
	AddResourece(TextureTag::eBuilding33D,		"../Assets/FBX Model/Building/33building/33buildingD.jpg");
	AddResourece(TextureTag::eBuilding34D,		"../Assets/FBX Model/Building/34building/34buildingD.jpg");
	AddResourece(TextureTag::eBuilding50D,		"../Assets/FBX Model/Building/50building/50buildingD.jpg");

	AddResourece(MeshTag::eBuilding16,			"../Assets/FBX Model/Building/16building/16building.model");
	AddResourece(MeshTag::eBuilding19,			"../Assets/FBX Model/Building/19building/19building.model");
	AddResourece(MeshTag::eBuilding20,			"../Assets/FBX Model/Building/20building/20building.model");
	AddResourece(MeshTag::eBuilding21,			"../Assets/FBX Model/Building/21building/21building.model");
	AddResourece(MeshTag::eBuilding22,			"../Assets/FBX Model/Building/22building/22building.model");
	AddResourece(MeshTag::eBuilding26,			"../Assets/FBX Model/Building/26building/26building.model");
	AddResourece(MeshTag::eBuilding29,			"../Assets/FBX Model/Building/29building/29building.model");
	AddResourece(MeshTag::eBuilding30,			"../Assets/FBX Model/Building/30building/30building.model");
	AddResourece(MeshTag::eBuilding33,			"../Assets/FBX Model/Building/33building/33building.model");
	AddResourece(MeshTag::eBuilding34,			"../Assets/FBX Model/Building/34building/34building.model");
	AddResourece(MeshTag::eBuilding50,			"../Assets/FBX Model/Building/50building/50building.model");


	// ============== Road ============== //
	AddResourece(TextureTag::eRoad1D,			"../Assets/FBX Model/Road/Road1D.dds");
	AddResourece(TextureTag::eRoad2D,			"../Assets/FBX Model/Road/Road2D.dds");
	AddResourece(TextureTag::eCenterRoadD,		"../Assets/FBX Model/Road/CenterRoad.dds");
	AddResourece(TextureTag::eCrossRoadD,		"../Assets/FBX Model/Road/CrossRoadD.jpg");

	AddResourece(MeshTag::eRoad,				"../Assets/FBX Model/Road/road.model");
	AddResourece(MeshTag::eCenterRoad,			"../Assets/FBX Model/Road/RoadCenter.model");
	AddResourece(MeshTag::eCrossRoad,			"../Assets/FBX Model/Road/CrossRoad.model");

}

void CResourceManager::LoadCharacterData()
{
	AddResourece(TextureTag::eTerroristD,		"../Assets/FBX Model/Character/Terrorist/TerrorristD.dds");
	AddResourece(TextureTag::eTerroristND,		"../Assets/FBX Model/Character/Terrorist/TerrorristNM_D.dds.dds");

//	AddResourece(TextureTag::ePoliceD,			"../Assets/FBX Model/Character/Police/AllTex.PNG");

	AddResourece(TextureTag::eRifleD,			"../Assets/FBX Model/Weapon/Rifle/rifle.bmp");
	AddResourece(TextureTag::eRifleN,			"../Assets/FBX Model/Weapon/Rifle/RifleNM.png");


//	AddResourece(MeshTag::ePolice,				"../Assets/FBX Model/Character/Police/Police.model");
	AddResourece(MeshTag::eTerrorist,			"../Assets/FBX Model/Character/Terrorist/Terrorist.model");


	AddResourece(MeshTag::eRifle,				"../Assets/FBX Model/Weapon/Rifle/rifle.model");

}

void CResourceManager::LoadEtcData()
{
	AddResourece(TextureTag::eStoneD,		"../Assets/Image/Miscellaneous/stones.dds");
	AddResourece(TextureTag::eStoneN,		"../Assets/Image/Miscellaneous/stones_nmap.dds");
	AddResourece(TextureTag::eStone02D,		"../Assets/Image/Miscellaneous/Stone02_Diffuse.jpg");
	AddResourece(TextureTag::eStone02N,		"../Assets/Image/Miscellaneous/Stone02_nmap.dds");
	AddResourece(TextureTag::eBricksD,		"../Assets/Image/Miscellaneous/bricks.dds");
	AddResourece(TextureTag::eBricksN,		"../Assets/Image/Miscellaneous/bricks_nmap.dds");
	AddResourece(TextureTag::eWallD,		"../Assets/Image/Miscellaneous/wall.jpg");
	AddResourece(TextureTag::eWallND,		"../Assets/Image/Miscellaneous/wall_NM_height.dds");

	AddResourece(MeshTag::eTest,			"../Assets/FBX Model/character2.model");
	AddResourece(MeshTag::eTest2,			"../Assets/FBX Model/basic2.model");
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
	else {
		if (meshData.LoadFbxModelSkinnedDatafromFile(source))
			ShowTaskSuccess("\t Success!!");
		else {
			ShowTaskFail("\t Error!! \t 파일 또는 경로를 확인하세요.");
			return;
		}
	}

	m_mapFbxMeshPool.insert(make_pair(meshTag, meshData));

	// 한 태그에 여러개 등록되었음
	assert(m_mapFbxMeshPool.count(meshTag) <= 1);
}

void CResourceManager::AddNoneFbxMesh(const MeshTag& meshTag)
{
	/*
	XMFLOAT3 scale;
	CMesh* pMesh;
	int nObjectCount = MAPDATA_MGR->GetDataVector(meshTag).size();

	for (int index = 0; index < nObjectCount; ++index) {
		switch (meshTag) {
		case MeshTag::eRoad:
			scale = MAPDATA_MGR->GetScale(ObjectTag::eRoad, index);
			pMesh = new CCubeMeshTexturedIlluminated(STATEOBJ_MGR->g_pd3dDevice, scale.x , scale.y, scale.z);

			m_mapNoneFbxMeshPool.insert(make_pair(meshTag, pMesh));
			break;
		}
	}
	*/

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
	if (findResource == m_mapPathPool.end())
		MessageBox(NULL, s_to_ws("Texture Tag : " + to_string(static_cast<int>(textureTag))).c_str(), L"Resource Error", MB_OK);

	return s_to_ws((*findResource).second.c_str());
}

ID3D11ShaderResourceView* CResourceManager::CloneShaderResourceView(const TextureTag& textureTag)
{
	auto findResource = m_mapTexturePool.find(textureTag);

	// Pool에 해당 데이터가 존재하지 않는다.
	if(findResource == m_mapTexturePool.end())
		MessageBox(NULL, s_to_ws("Texture Tag : " + to_string(static_cast<int>(textureTag))).c_str(), L"Resource Error", MB_OK);

	return (*findResource).second;
}

CFbxMeshData CResourceManager::CloneFbxMeshData(const MeshTag& meshTag) 
{
	auto findResource = m_mapFbxMeshPool.find(meshTag);

	// Pool에 해당 데이터가 존재하지 않는다.
	if (findResource == m_mapFbxMeshPool.end())
		MessageBox(NULL, s_to_ws("Mesh Tag : " + to_string(static_cast<int>(meshTag))).c_str(), L"Resource Error", MB_OK);

	return (*findResource).second;
}

CMesh* CResourceManager::CloneNoneFbxMeshData(const MeshTag& meshTag)
{
	auto findResource = m_mapNoneFbxMeshPool.find(meshTag);

	// Pool에 해당 데이터가 존재하지 않는다.
	if (findResource == m_mapNoneFbxMeshPool.end())
		MessageBox(NULL, s_to_ws("Mesh Tag : " + to_string(static_cast<int>(meshTag))).c_str(), L"Resource Error", MB_OK);

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
