#pragma once

// ========================================================================================================================== //
// ====================================================== KeyInput ========================================================== //

enum class KeyInput
{
	eNone,

	// Direction & Moving
	eForward		= 0x01,
	eBackward		= 0x02,
	eLeft			= 0x04,
	eRight			= 0x08,
	eRun			= 0x10,

	// Mouse
	eLeftMouse		= 0x20,
	eRightMouse		= 0x40,
	eMouseWheel		= 0x80,		// 휠 위아래는 추후 구현

};

enum class CameraTag 
{
	eNone,

	eFirstPerson, eSpaceShip, eThirdPerson,

	eDeadCam, eFreeCam
};

enum class TeamType
{
	eNone,
	eRedTeam, eBlueTeam
};

enum class SceneTag {
	eNone, eTitleScene, eLoadingScene, eMainScene
};

enum ShaderTag {
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

	// BackGround Image
	eTitleBackGroundD, eLoadingBackGroundD,

	// UI
	eStartButtonOn, eStartButtonOff, eExitButtonOn, eExitButtonOff,
	eCaptureArea,

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
	eTerrorist, 

	// --------------------------------------------------------- // 
	MaxSkinnedMesh = 100,
	// --------------------------------------------------------- // 
	ePolice,
	// Character
	eDarkFighter, eRifle,

	// Environment
	eRoad,

	// Building
	eBuilding18, eBuilding19,

	// Etc
	eTest, eTest2
};

// 충돌 처리시 사용할 Layer
namespace Layer 
{

}