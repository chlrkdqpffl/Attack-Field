#pragma once


enum eSceneTag {
	eScene_Title, eScene_Loading, eScene_Main
};

enum eShaderTag {
	eShader_NormalMap
};


// eTexture
// RESOURCE_MGR->FindResourceAndCreateSRV(     )

enum Resource_TextrueTag {
	// Environment
	eTexture_Water, eTexture_WaterDetail, eTexture_SkyBox, 
	eTexture_TerrainDiffuse, eTexture_TerrainDetail, eTexture_TerrainNormal, eTexture_TerrainHeightMap, eTexture_TerrainHeightMapRaw,

	// Model
	eTexture_DarkFighterColor,

	// UI
	eTexture_Title,

	// etc
	eTexture_Stone, eTexture_StoneNormal, eTexture_Stone02, eTexture_Stone02Normal, eTexture_Bricks, eTexture_BricksNormal, eTexture_Wood,
	eTexture_WallDiffuse, eTexture_WallNormalHeight
};


// eMesh
enum Resource_MeshTag {
	eMesh_DarkFighter, eMesh_Siegetank
};