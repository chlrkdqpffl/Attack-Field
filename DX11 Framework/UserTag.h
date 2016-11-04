#pragma once


enum eSceneTag {
	eScene_Title, eScene_Loading, eScene_Main
};


// eTexture
// RESOURCE_MGR->FindResourceFromMap(     )

enum Resource_TextrueTag {
	// Environment
	eTexture_Water, eTexture_WaterDetail, eTexture_SkyBox, eTexture_Terrain, eTexture_TerrainDetail, eTexture_HeightMap,

	// Model
	eTexture_DarkFighterColor,

	// etc
	eTexture_Stone, eTexture_Bricks, eTexture_Wood
};


// eMesh
enum Resource_MeshTag {
	eMesh_DarkFighter
};