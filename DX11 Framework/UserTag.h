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
	eTexture_Water, eTexture_WaterDetail, eTexture_SkyBox, eTexture_Terrain, eTexture_TerrainDetail, eTexture_HeightMap, eTexture_TerrainNormal,

	// Model
	eTexture_DarkFighterColor,

	// etc
	eTexture_Stone, eTexture_StoneNormal, eTexture_Bricks, eTexture_BricksNormal, eTexture_Wood, 
};


// eMesh
enum Resource_MeshTag {
	eMesh_DarkFighter
};