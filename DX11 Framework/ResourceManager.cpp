#include "stdafx.h"
#include "ResourceManager.h"

void CResourceManager::InitializeManager()
{
	cout << "=============================================================================================" << endl;
	cout << "===================================== Resource Loading ======================================" << endl;
	cout << " ----- Title Scene Data -----" << endl;
	LoadResourceTitleScene();
}

void CResourceManager::ReleseManager()
{
}

bool CResourceManager::LoadResourceAll()
{	
	if (m_bIsLoading)
		return true;

	LoadCharacterData();
	LoadMapData();
//	LoadEnvironmentData();
	LoadSpriteData();
	LoadUIData();
	PARTICLE_MGR->InitializeManager();
	LoadEtcData();

	cout << endl;
	cout << "=============================== Resource Loading Complete ==================================" << endl;
	cout << "============================================================================================" << endl << endl;

	m_bIsLoading = true;
	return true;
}

void CResourceManager::LoadResourceTitleScene()
{
	// BackGround Image
	AddResourece(TextureTag::eTitleBackGroundD,		"../Assets/Image/UI/TitleScene.jpg");
	AddResourece(TextureTag::eLoadingBackGroundD,	"../Assets/Image/UI/LoadingScene(Final).jpg");
	AddResourece(TextureTag::eDeathOn,				"../Assets/Image/UI/DeathMatchOn.jpg");
	AddResourece(TextureTag::eOccupyOn,				"../Assets/Image/UI/OccupyMatchOn.jpg");
	AddResourece(TextureTag::eDeathOff,				"../Assets/Image/UI/DeathMatchOff.jpg");
	AddResourece(TextureTag::eOccupyOff,			"../Assets/Image/UI/OccupyMatchOff.jpg");
	AddResourece(TextureTag::eDeathSearch,			"../Assets/Image/UI/DeathMatchWaiting.jpg");
	AddResourece(TextureTag::eOccupySearch,			"../Assets/Image/UI/OccupyMatchOnWaiting.jpg");

	// UI
	AddResourece(TextureTag::eStartButtonOn,		"../Assets/Image/UI/StartOn.png");
	AddResourece(TextureTag::eStartButtonOff,		"../Assets/Image/UI/StartOff.png");
	AddResourece(TextureTag::eExitButtonOn,			"../Assets/Image/UI/ExitOn.png");
	AddResourece(TextureTag::eExitButtonOff,		"../Assets/Image/UI/ExitOff.png");

	//search UI
	AddResourece(TextureTag::eSearch1, "../Assets/Image/loading/1.png");
	AddResourece(TextureTag::eSearch2, "../Assets/Image/loading/2.png");
	AddResourece(TextureTag::eSearch3, "../Assets/Image/loading/3.png");
	AddResourece(TextureTag::eSearch4, "../Assets/Image/loading/4.png");
	AddResourece(TextureTag::eSearch5, "../Assets/Image/loading/5.png");
	AddResourece(TextureTag::eSearch6, "../Assets/Image/loading/6.png");
	AddResourece(TextureTag::eSearch7, "../Assets/Image/loading/7.png");
	AddResourece(TextureTag::eSearch8, "../Assets/Image/loading/8.png");
	AddResourece(TextureTag::eSearch9, "../Assets/Image/loading/9.png");
}

void CResourceManager::LoadEnvironmentData()
{
	cout << endl << " ----- Environment Data -----" << endl;

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
	cout << endl << " ----- UI Data -----" << endl;

	AddResourece(TextureTag::eCaptureArea,			"../Assets/Image/UI/CaptureArea.png");
	AddResourece(TextureTag::eScoreUI,				"../Assets/Image/UI/ScoreUI.png");
	AddResourece(TextureTag::eMagazineUI,			"../Assets/Image/UI/MagazineUI.png");
	AddResourece(TextureTag::eLifeUI,				"../Assets/Image/UI/LifeUI.png");
	AddResourece(TextureTag::eDamagedCharacterUI,	"../Assets/Image/UI/DamagedCharacter.png");
	AddResourece(TextureTag::eRespawnGageBar,		"../Assets/Image/UI/RespawnGage.png");
	AddResourece(TextureTag::eRespawnGageWhiteBar,	"../Assets/Image/UI/RespawnGage(White).png");

	AddResourece(TextureTag::eZoomScopeD,			"../Assets/Image/UI//Zoom_Scope.png");
	AddResourece(TextureTag::eBoom,					"../Assets/Image/UI//Boom.png");

	AddResourece(TextureTag::eOccupyGageBar,		"../Assets/Image/UI/RespawnGage.png");	// 현재는 동일한 텍스쳐 사용중
	AddResourece(TextureTag::eOccupyGageWhiteBar,	"../Assets/Image/UI/RespawnGage(White).png");

	// Damage Direction
	AddResourece(TextureTag::eDamageDirection_Left,		"../Assets/Image/UI/DamageDirection/DamageDir_Left.png");
	AddResourece(TextureTag::eDamageDirection_LeftTop,	"../Assets/Image/UI/DamageDirection/DamageDir_LeftTop.png");
	AddResourece(TextureTag::eDamageDirection_Top,		"../Assets/Image/UI/DamageDirection/DamageDir_Top.png");
	AddResourece(TextureTag::eDamageDirection_RightTop, "../Assets/Image/UI/DamageDirection/DamageDir_RightTop.png");
	AddResourece(TextureTag::eDamageDirection_Right,	"../Assets/Image/UI/DamageDirection/DamageDir_Right.png");
	AddResourece(TextureTag::eDamageDirection_Bottom,	"../Assets/Image/UI/DamageDirection/DamageDir_Bottom.png");
}

void CResourceManager::LoadSpriteData()
{
	cout << endl << " ----- Sprite Image Data -----" << endl;
	
	AddResourece(TextureTag::eExplosionSprite,			"../Assets/Image/Sprite/Explosion.png");
	AddResourece(TextureTag::eGunExplosionOtherPlayer,	"../Assets/Image/Sprite/Explosion3.png");
	AddResourece(TextureTag::eGunExplosionPlayer,		"../Assets/Image/Sprite/Explosion4.png");
}

void CResourceManager::LoadMapData()
{
	cout << endl << " ----- Map Data -----" << endl;

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

	// =================== Container =================== //
	AddResourece(TextureTag::eContainerRedD,	"../Assets/FBX Model/Container/ContainerRED.dds");
	AddResourece(TextureTag::eContainerBlueD,	"../Assets/FBX Model/Container/ContainerBLUE.dds");
	AddResourece(TextureTag::eContainerYellowD, "../Assets/FBX Model/Container/ContainerYELLOW.dds");
	AddResourece(TextureTag::eContainerN,		"../Assets/FBX Model/Container/ContainerN.dds");

	AddResourece(MeshTag::eContainer1,			"../Assets/FBX Model/Container/Container1.model");
	AddResourece(MeshTag::eContainer2,			"../Assets/FBX Model/Container/Container2.model");
	AddResourece(MeshTag::eContainer3,			"../Assets/FBX Model/Container/Container3.model");

	// =============== Wood Box, Board =============== //
	AddResourece(TextureTag::eWoodBoxD,			"../Assets/FBX Model/WoodBox/WoodBoxD.jpg");
	AddResourece(TextureTag::eWoodBoxN,			"../Assets/FBX Model/WoodBox/WoodBoxN.jpg");
	AddResourece(TextureTag::eWoodBoardD,		"../Assets/FBX Model/WoodBoard/WoodBoardD.png");
	AddResourece(TextureTag::eWoodBoardN,		"../Assets/FBX Model/WoodBoard/WoodBoardN.jpg");

	AddResourece(MeshTag::eWoodBox,				"../Assets/FBX Model/WoodBox/WoodBox.model");
	AddResourece(MeshTag::eWoodBoard,			"../Assets/FBX Model/WoodBoard/WoodBoard.model");

	// =================== Stair =================== //
	AddResourece(TextureTag::eStairD,			"../Assets/FBX Model/Stairs/StairD.dds");

	AddResourece(MeshTag::eStair,				"../Assets/FBX Model/Stairs/Stair.model");
	AddResourece(MeshTag::eStair2,				"../Assets/FBX Model/Stairs/Stair2.model");

	// =================== Barrel =================== //
	AddResourece(MeshTag::eFireBarrel,			"../Assets/FBX Model/Barrel/FireBarrel.model");
	AddResourece(MeshTag::eOccupyBarrel,		"../Assets/FBX Model/Barrel/OccupyBarrel.model");

	AddResourece(TextureTag::eFireBarrelD,		"../Assets/FBX Model/Barrel/FireBarrelD.dds");
	AddResourece(TextureTag::eFireBarrelN,		"../Assets/FBX Model/Barrel/FireBarrelN.png");
	
	AddResourece(TextureTag::eOccupyBarrelD,	"../Assets/FBX Model/Barrel/OccupyBarrelD.jpg");
	AddResourece(TextureTag::eOccupyBarrelN,	"../Assets/FBX Model/Barrel/OccupyBarrelN.jpg");

	// =================== Lamp =================== //
	AddResourece(MeshTag::eStreetLamp,			"../Assets/FBX Model/StreetLamp/StreetLamp.model");
	AddResourece(TextureTag::eStreetLampD,		"../Assets/FBX Model/StreetLamp/StreetLampD.dds");
	AddResourece(TextureTag::eStreetLampN,		"../Assets/FBX Model/StreetLamp/StreetLampN.png");

	// Etc
	AddResourece(TextureTag::eGrassD,			"../Assets/FBX Model/Etc/GrassD.jpg");

	AddResourece(TextureTag::eBarricadeD,		"../Assets/FBX Model/Barricade/BarricadeD.jpg");
	AddResourece(MeshTag::eBarricade,			"../Assets/FBX Model/Barricade/streetprops_barricade3.model");


	AddResourece(TextureTag::eSideWalk1D,		"../Assets/FBX Model/SideWalk/5Diffuse.jpg");
	AddResourece(TextureTag::eSideWalk1ND,		"../Assets/FBX Model/SideWalk/5ND.dds");
	AddResourece(TextureTag::eSideWalk2D,		"../Assets/FBX Model/SideWalk/2Diffuse.jpg");
	AddResourece(TextureTag::eSideWalk2N,		"../Assets/FBX Model/SideWalk/2NorMap.png");

//	AddResourece(MeshTag::eTest,				"../Assets/FBX Model/Desk4.model");
//	AddResourece(TextureTag::eTest,				"../Assets/FBX Model/Office.png");

}

void CResourceManager::LoadCharacterData()
{
	cout << endl << " ----- Character Data -----" << endl;

//	AddResourece(TextureTag::eTerroristD,		"../Assets/FBX Model/Character/Terrorist/TerrorristD.dds");
	AddResourece(TextureTag::eTerroristD_Red,	"../Assets/FBX Model/Character/Terrorist/TerrorristD_R.jpg");
	AddResourece(TextureTag::eTerroristD_Blue,	"../Assets/FBX Model/Character/Terrorist/TerrorristD_B.jpg");
//	AddResourece(TextureTag::eTerroristD,		"../Assets/FBX Model/Character/Terrorist/test.jpg");
	AddResourece(TextureTag::eTerroristND,		"../Assets/FBX Model/Character/Terrorist/TerrorristNM_D.dds.dds");

	AddResourece(MeshTag::eTerrorist,			"../Assets/FBX Model/Character/Terrorist/Terrorist.model");
	AddResourece(MeshTag::eTerrorist_Arm,		"../Assets/FBX Model/Character/Terrorist/Terrorist(Arm).model");

//	AddResourece(TextureTag::ePoliceD,			"../Assets/FBX Model/Character/Police/AllTex.PNG");

	// Rifle
	AddResourece(TextureTag::eRifleD,			"../Assets/FBX Model/Weapon/Rifle/rifle.bmp");
	AddResourece(TextureTag::eRifleN,			"../Assets/FBX Model/Weapon/Rifle/RifleNM.png");
	AddResourece(MeshTag::eRifle,				"../Assets/FBX Model/Weapon/Rifle/rifle.model");

	// Sniper Rifle
	AddResourece(TextureTag::eSniperRifleD,		"../Assets/FBX Model/Weapon/SniperRifle/SniperRifleD.dds");
	AddResourece(MeshTag::eSniperRifle,			"../Assets/FBX Model/Weapon/SniperRifle/SniperRifle.model");
}

void CResourceManager::LoadEtcData()
{
	cout << endl << " ----- Etc Data -----" << endl;

	AddResourece(TextureTag::eStoneD,		"../Assets/Image/Miscellaneous/StonesD.dds");
	AddResourece(TextureTag::eStoneND,		"../Assets/Image/Miscellaneous/StoneND.dds");
	
	AddResourece(TextureTag::eStoneWallD,	"../Assets/FBX Model/StoneWall/stonewall_diff.jpg");
	AddResourece(TextureTag::eStoneWallN,	"../Assets/FBX Model/StoneWall/stonewall_norm.jpg");

	AddResourece(TextureTag::eBricksD,		"../Assets/Image/Miscellaneous/bricks.dds");
	AddResourece(TextureTag::eBricksN,		"../Assets/Image/Miscellaneous/bricks_nmap.dds");
	AddResourece(TextureTag::eWallD,		"../Assets/Image/Miscellaneous/wall.jpg");
	AddResourece(TextureTag::eWallND,		"../Assets/Image/Miscellaneous/wall_NM_height.dds");

	AddResourece(TextureTag::eDecal_Concrete,	"../Assets/Image/Decal/Decal_Concrete.dds");
	AddResourece(TextureTag::eDecal_White,		"../Assets/Image/Decal/white.dds");
}

void CResourceManager::AddResourece(const TextureTag& textureTag, const string& source)
{
	// 한 태그에 여러개 등록되었음
	assert(m_mapTexturePool.count(textureTag) < 1);

	cout << "  File Loading < " + source + " > ";
	
	HRESULT hResult;
	ID3D11ShaderResourceView *pSRVTexture = NULL;
	D3DX11CreateShaderResourceViewFromFile(STATEOBJ_MGR->g_pd3dDevice, s_to_ws(source).c_str(), NULL, NULL, &pSRVTexture, &hResult);

	if ((HRESULT)hResult >= 0)
		ShowTaskSuccess("\t Success!!");
	else
		ShowTaskFail("\t Error!! \t\t 파일 또는 경로를 확인하세요.");

	string str = "TextureTag : " + to_string(static_cast<int>(textureTag));
	DXUT_SetDebugName(pSRVTexture, str.c_str());

	m_mapTexturePool.insert(make_pair(textureTag, pSRVTexture));
}

void CResourceManager::AddResourece(const MeshTag& meshTag, const string& source)
{
	// 한 태그에 여러개 등록되었음
	assert(m_mapFbxMeshPool.count(meshTag) < 1);

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
}

void CResourceManager::AddResourecePath(const TextureTag& textureTag, const string& source)
{
	// 한 태그에 여러개 등록되었음
	assert(m_mapPathPool.count(textureTag) <= 1);

	m_mapPathPool.insert(make_pair(textureTag, source));
}

wstring CResourceManager::FindResourcePath(const TextureTag& textureTag)
{
	auto findResource = m_mapPathPool.find(textureTag);

	// Pool에 해당 데이터가 존재하지 않는다.
	if (findResource == m_mapPathPool.end())
		MessageBox(NULL, s_to_ws("Texture Tag : " + to_string(static_cast<int>(textureTag))).c_str(), L"Resource File Null", MB_OK);

	return s_to_ws((*findResource).second.c_str());
}

ID3D11ShaderResourceView* CResourceManager::CloneShaderResourceView(const TextureTag& textureTag)
{
	auto findResource = m_mapTexturePool.find(textureTag);

	// Pool에 해당 데이터가 존재하지 않는다.
	if(findResource == m_mapTexturePool.end())
		MessageBox(NULL, s_to_ws("Texture Tag : " + to_string(static_cast<int>(textureTag))).c_str(), L"Resource File Null", MB_OK);

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
