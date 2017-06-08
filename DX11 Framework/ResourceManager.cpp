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
//	AddResourece(TextureTag::eLoadingBackGroundD,	"../Assets/Image/UI/LoadingScene.jpg");
	AddResourece(TextureTag::eLoadingBackGroundD,	"../Assets/Image/UI/LoadingScene2.jpg");

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
//	LoadEnvironmentData();
	
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
	AddResourece(TextureTag::eAim,				"../Assets/Image/UI/AimUI.png");
	AddResourece(TextureTag::eCaptureArea,		"../Assets/Image/UI/CaptureArea.png");
	AddResourece(TextureTag::eScoreUI,			"../Assets/Image/UI/ScoreUI.png");
	AddResourece(TextureTag::eMagazineUI,		"../Assets/Image/UI/MagazineUI.png");
	AddResourece(TextureTag::eLifeUI,			"../Assets/Image/UI/LifeUI.png");
	AddResourece(TextureTag::eDamagedCharacterUI, "../Assets/Image/UI/DamagedCharacter.png");
}

void CResourceManager::LoadMapData()
{
	// ============== Building ============== //
	AddResourece(TextureTag::eBuilding19D,		"../Assets/FBX Model/Building/Building19/19buildingD.jpg");
	AddResourece(TextureTag::eBuilding20D,		"../Assets/FBX Model/Building/Building20/20buildingD.jpg");
	AddResourece(TextureTag::eBuilding21D,		"../Assets/FBX Model/Building/Building21/21buildingD.jpg");
	AddResourece(TextureTag::eBuilding22D,		"../Assets/FBX Model/Building/Building22/22buildingD.jpg");
	AddResourece(TextureTag::eBuilding30D,		"../Assets/FBX Model/Building/Building30/30buildingD.jpg");
	AddResourece(TextureTag::eBuilding33D,		"../Assets/FBX Model/Building/Building33/33buildingD.jpg");
	AddResourece(TextureTag::eBuilding34D,		"../Assets/FBX Model/Building/Building34/34buildingD.jpg");
	AddResourece(TextureTag::eBuilding77D,		"../Assets/FBX Model/Building/Building77/77buildingD.jpg");
	AddResourece(TextureTag::eBuilding78D,		"../Assets/FBX Model/Building/Building78/78buildingD.jpg");
	AddResourece(TextureTag::eBuilding100D,		"../Assets/FBX Model/Building/Building100/Building100D.jpg");
	AddResourece(TextureTag::eBuilding103D,		"../Assets/FBX Model/Building/Building103/Building103D.jpg");
	
	AddResourece(MeshTag::eBuilding19,			"../Assets/FBX Model/Building/Building19/19building.model");
	AddResourece(MeshTag::eBuilding20,			"../Assets/FBX Model/Building/Building20/20building.model");
	AddResourece(MeshTag::eBuilding21,			"../Assets/FBX Model/Building/Building21/21building.model");
	AddResourece(MeshTag::eBuilding22,			"../Assets/FBX Model/Building/Building22/22building.model");
	AddResourece(MeshTag::eBuilding30,			"../Assets/FBX Model/Building/Building30/30building.model");
	AddResourece(MeshTag::eBuilding33,			"../Assets/FBX Model/Building/Building33/33building.model");
	AddResourece(MeshTag::eBuilding34,			"../Assets/FBX Model/Building/Building34/34building.model");
	AddResourece(MeshTag::eBuilding77,			"../Assets/FBX Model/Building/Building77/77building.model");
	AddResourece(MeshTag::eBuilding78,			"../Assets/FBX Model/Building/Building78/78building.model");
	AddResourece(MeshTag::eBuilding100,			"../Assets/FBX Model/Building/Building100/Building100.model");
	AddResourece(MeshTag::eBuilding103,			"../Assets/FBX Model/Building/Building103/Building103.model");
	AddResourece(MeshTag::eBuilding104,			"../Assets/FBX Model/Building/Building104/Building104.model");

	// Parking Lot
	AddResourece(TextureTag::eParkingLotD,		"../Assets/FBX Model/ParkingLot/ParkingLotD.jpg");
	AddResourece(MeshTag::eParkingLot,			"../Assets/FBX Model/ParkingLot/ParkingLot.model");

	// ============== Road ============== //
	AddResourece(TextureTag::eRoad1D,			"../Assets/FBX Model/Road/Road1D.dds");
	AddResourece(TextureTag::eRoad1N,			"../Assets/FBX Model/Road/Road1N.dds");
	AddResourece(TextureTag::eRoad2D,			"../Assets/FBX Model/Road/Road2D.dds");
	AddResourece(TextureTag::eRoad2N,			"../Assets/FBX Model/Road/Road2N.dds");
	AddResourece(TextureTag::eCenterRoadD,		"../Assets/FBX Model/Road/CenterRoadD.jpg");
	AddResourece(TextureTag::eCenterRoadN,		"../Assets/FBX Model/Road/CenterRoadN.dds");
	AddResourece(TextureTag::eCrossRoadD,		"../Assets/FBX Model/Road/CrossRoadD.jpg");
	AddResourece(TextureTag::eCrossRoadN,		"../Assets/FBX Model/Road/CrossRoadN.dds");


	AddResourece(MeshTag::eRoad,				"../Assets/FBX Model/Road/road.model");
	AddResourece(MeshTag::eCenterRoad,			"../Assets/FBX Model/Road/RoadCenter.model");
	AddResourece(MeshTag::eCrossRoad,			"../Assets/FBX Model/Road/CrossRoad.model");

	// Etc
	AddResourece(TextureTag::eBenchD,			"../Assets/FBX Model/Bench/BenchD.jpg");
	AddResourece(MeshTag::eBench,				"../Assets/FBX Model/Bench/Bench.model");

	AddResourece(TextureTag::eBusStopD,			"../Assets/FBX Model/Busstop/bus_stop_sign.png");
	AddResourece(MeshTag::eBusStop,				"../Assets/FBX Model/Busstop/BusStop.model");

//	AddResourece(TextureTag::eStreetLampD,		"../Assets/FBX Model/StreetLamp/bus_stop_sign.png");
	AddResourece(MeshTag::eStreetLamp,			"../Assets/FBX Model/StreetLamp/lamp.model");

	AddResourece(TextureTag::eGrassD,			"../Assets/FBX Model/Etc/GrassD.jpg");

	AddResourece(TextureTag::eBarricadeD,		"../Assets/FBX Model/Barricade/BarricadeD.jpg");
	AddResourece(MeshTag::eBarricade,			"../Assets/FBX Model/Barricade/streetprops_barricade3.model");


	AddResourece(TextureTag::eSideWalk1D,		"../Assets/FBX Model/SideWalk/5Diffuse.jpg");
	AddResourece(TextureTag::eSideWalk1ND,		"../Assets/FBX Model/SideWalk/5ND.dds");
	AddResourece(TextureTag::eSideWalk2D,		"../Assets/FBX Model/SideWalk/2Diffuse.jpg");
	AddResourece(TextureTag::eSideWalk2N,		"../Assets/FBX Model/SideWalk/2NorMap.png");
}

void CResourceManager::LoadCharacterData()
{
	AddResourece(TextureTag::eTerroristD,		"../Assets/FBX Model/Character/Terrorist/TerrorristD.dds");
//	AddResourece(TextureTag::eTerroristD,		"../Assets/FBX Model/Character/Terrorist/test.jpg");
	AddResourece(TextureTag::eTerroristND,		"../Assets/FBX Model/Character/Terrorist/TerrorristNM_D.dds.dds");

//	AddResourece(TextureTag::ePoliceD,			"../Assets/FBX Model/Character/Police/AllTex.PNG");

	AddResourece(TextureTag::eRifleD,			"../Assets/FBX Model/Weapon/Rifle/rifle.bmp");
	AddResourece(TextureTag::eRifleN,			"../Assets/FBX Model/Weapon/Rifle/RifleNM.png");


	AddResourece(MeshTag::eTerrorist,			"../Assets/FBX Model/Character/Terrorist/Terrorist.model");
	AddResourece(MeshTag::eTerrorist_Arm,		"../Assets/FBX Model/Character/Terrorist/Terrorist(Arm).model");


	AddResourece(MeshTag::eRifle,				"../Assets/FBX Model/Weapon/Rifle/rifle.model");

}

void CResourceManager::LoadEtcData()
{
	AddResourece(TextureTag::eStoneD,		"../Assets/Image/Miscellaneous/StonesD.dds");
	AddResourece(TextureTag::eStoneND,		"../Assets/Image/Miscellaneous/StoneND.dds");
	
	AddResourece(TextureTag::eStoneWallD,	"../Assets/FBX Model/StoneWall/stonewall_diff.jpg");
	AddResourece(TextureTag::eStoneWallN,	"../Assets/FBX Model/StoneWall/stonewall_norm.jpg");

	AddResourece(TextureTag::eBricksD,		"../Assets/Image/Miscellaneous/bricks.dds");
	AddResourece(TextureTag::eBricksN,		"../Assets/Image/Miscellaneous/bricks_nmap.dds");
	AddResourece(TextureTag::eWallD,		"../Assets/Image/Miscellaneous/wall.jpg");
	AddResourece(TextureTag::eWallND,		"../Assets/Image/Miscellaneous/wall_NM_height.dds");

	AddResourece(MeshTag::eTest,			"../Assets/FBX Model/character.model");

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
