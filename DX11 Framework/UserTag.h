#pragma once


enum eSceneTag {
	eScene_Title, eScene_Loading, eScene_Main
};

enum eShaderTag {
	eShader_NormalMap
};

// ========================================================================================================================== //
// ==================================================== Weapon Data ========================================================= //

namespace WeaponData
{
	enum class Type
	{
		eRifle, eSniperRifle
	};
}


// ============================================================================================================================ //
// ===================================================== Animation Data ======================================================= //

namespace AnimationData
{
	enum class Type
	{
		eLoop, eOnce, ePingPong
	};

	enum class CharacterAnim
	{
		eNone, eIdle, eWalk, eRun, eStanding_Fire, eWalking_Fire, eDeath
	};
}

// =========================================================================================================================== //
// ===================================================== Texture Data ======================================================== //
enum Resource_TextrueTag {
	// Environment
	eTexture_Water, eTexture_WaterDetail, eTexture_SkyBox, 
	eTexture_TerrainDiffuse, eTexture_TerrainDetail, eTexture_TerrainNormal, eTexture_TerrainHeightMap, eTexture_TerrainHeightMapRaw,

	// Model
	eTexture_DarkFighterColor, eTexture_DrayerDiffuse, 

	eTexture_TerroristDiffuse, eTexture_TerroristNormalDisplace,
	eTexture_PoliceDiffuse,
	eTexture_Rifle, eTexture_RifleNormal,

	// UI
	eTexture_Title,

	// Etc
	eTexture_Stone, eTexture_StoneNormal, eTexture_Stone02, eTexture_Stone02Normal, eTexture_Bricks, eTexture_BricksNormal, eTexture_Wood,
	eTexture_WallDiffuse, eTexture_WallNormalHeight
};

// ========================================================================================================================= //
// ======================================================= Mesh Data ======================================================= //
namespace MeshData
{
	enum Resource_MeshTag {
		// Character
		eMesh_DarkFighter, eMesh_Drayer, eMesh_Police, eMesh_Terrorist, eMesh_Rifle,

		// Object
		eMesh_Road,

		// Etc
		eMesh_Test
	};
}