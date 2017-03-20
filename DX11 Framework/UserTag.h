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
		eNone, eRifle, eSniperRifle
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

// D - Diffuse Map
// N - Normal Map
// DP - Displacement Map
// ND - Normal + Displacement Map
// S - Specular
// H - Height Map

enum class TextureTag
{
	eNone, eTest,

	// Environment
	eWaterD, eWaterDetailD, eSkyBoxD,
	eTerrainD, eTerrainDetailD, eTerrainN, eTerrainH,

	// Model
	eDarkFighterD, eDarkFighterN,

	eTerroristD, eTerroristND,
	ePoliceD,
	eRifleD, eRifleN,

	// Building
	eBuilding18,

	// UI
	eTitleD,

	// Etc
	eStoneD, eStoneN, eStone02D, eStone02N, eBricksD, eBricksN,
	eWallD, eWallND
};


// ========================================================================================================================= //
// ======================================================= Mesh Data ======================================================= //

enum class MeshTag
{
	eNone,

	// Skinned
	eTerrorist, eTest,

	// --------------------------------------------------------- // 
	MaxSkinnedMesh = 100,
	// --------------------------------------------------------- // 
	ePolice,
	// Character
	eDarkFighter, eRifle,

	// Environment
	eRoad,

	// Building
	eBuilding18, eBuilding19

	// Etc

};

// 충돌 처리시 사용할 Layer
namespace Layer 
{

}