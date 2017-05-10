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
	eReload			= 0x20,

	// Mouse
	eLeftMouse		= 0x100,
	eRightMouse		= 0x200,
	eMouseWheel		= 0x400,		// 휠 위아래는 추후 구현

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
	eNone,
	eColor, eNormal, eTexture, eNormalTexture, eNormalTangentTexture,
	eInstanceNormalTexture
};

enum class ChracterBoundingBoxParts 
{
	eNone = -1,
	eBody, eHead,
	eLeftUpArm, eLeftDownArm, eRightUpArm, eRightDownArm,
	eLeftUpLeg, eLeftDownLeg, eRightUpLeg, eRightDownLeg,
	
	ePartsCount
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
	enum class Parts
	{ 
		Defalut, UpperBody, LowerBody
	};

	enum class Type
	{
		eLoop, eInverseLoop, eOnce, ePingPong
	};

	enum class CharacterAnim
	{
		eNone,
		// --------------------------------------------------------- // 
		// Whole Body
		eIdle, eDeath, eDeath_Head, eRun,

		// --------------------------------------------------------- // 
		// Upper Body
		UpperBodyAnim = 100,
		eFire, eReload, eHeadHit,
		// --------------------------------------------------------- // 
		// Lower Body
		LowerBodyAnim = 1000,
	
		eWalk_Left, eWalk_ForwardLeft, eWalk_Forward, eWalk_ForwardRight, eWalk_Right, eWalk_BackwardRight, eWalk_Backward, eWalk_BackwardLeft,
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
	eBuilding16D, eBuilding19D, eBuilding20D, eBuilding21D, eBuilding22D, eBuilding26D, eBuilding29D, eBuilding30D, eBuilding33D, eBuilding34D, eBuilding50D,

	eBenchD, eGrassD,

	// BackGround Image
	eTitleBackGroundD, eLoadingBackGroundD,

	// Road
	eRoad1D, eRoad2D, eCenterRoadD, eCrossRoadD,

	// UI
	eStartButtonOn, eStartButtonOff, eExitButtonOn, eExitButtonOff,
	eCaptureArea, eAim,

	// Etc
	eBusStopD, eStreetLampD,

	// Etc 2
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
	eTest2,
	// --------------------------------------------------------- // 
	MaxSkinnedMesh = 100,
	// --------------------------------------------------------- // 

	ePolice,
	// Character
	eDarkFighter, eRifle,

	// Environment

	// Building
	eBuilding16, eBuilding19, eBuilding20, eBuilding21, eBuilding22, eBuilding26, eBuilding29, eBuilding30, eBuilding33, eBuilding34, eBuilding50,

	// Road
	eRoad, eCenterRoad, eCrossRoad,


	// Etc
	eBench, eBusStop, eStreetLamp,

	// Etc 2
	eTest,
	eBox100m
};

enum class ObjectTag
{
	eNone,

	// Road
	eRoad1, eRoad2, eCenterRoad, eCrossRoad,

	// Building
	eBuilding16, eBuilding19, eBuilding20, eBuilding21, eBuilding22, eBuilding26, eBuilding29, eBuilding30, eBuilding33, eBuilding34, eBuilding50,

	eBench, eGrass, eBusStop, eStreetLamp
};


// =========================================================================================================================== //
// ===================================================== Sound Data ======================================================== //
enum class SoundTag
{
	eNone = -1,

	// BGM
	eBGM_TitleScene,

	// Effect
	eFire, eReload, eWalk, eRun, eDeath, eShellsFall,

	SoundCount
};


enum class GBufferType
{
	eNone,
	eDepth, eDiffuse, eNormal, eSpec
};
