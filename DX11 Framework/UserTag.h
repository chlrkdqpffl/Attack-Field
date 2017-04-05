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
	eMouseWheel		= 0x80,		// �� ���Ʒ��� ���� ����

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

enum class ShaderTag {
	eColor, eTexture, eNormalTexture, eNormalTangentTexture,
	eInstanceNormalTexture
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
		eNone, eIdle, eWalk, eRun, eStandingFire, eWalkingFire, eStandingReloading		//eDeath
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
	eBuilding21D, eBuilding50D,

	// BackGround Image
	eTitleBackGroundD, eLoadingBackGroundD,

	// Road
	eRoadD,

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

	// Building
	eBuilding21, eBuilding50,

	// Road
	eRoad,


	// Etc
	eTest, eTest2,
};

// �浹 ó���� ����� Layer
namespace Layer 
{

}