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
	// 현재 안쓰고 있음 - 서버에서 따로 생성하였기 때문
	eNone,
	eRedTeam, eBlueTeam
};

enum class SceneTag {
	eNone, eTitleScene, eLoadingScene, eMainScene
};

enum class ShaderTag {
	eNone,
	eColor, eNormal, eTexture, eNormalTexture, eNormalTangentTexture,

	// Instancing
	eInstanceNormal, eInstanceNormalTexture, eInstanceNormalTangentTexture
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
	eBuilding19D, eBuilding20D, eBuilding21D, eBuilding22D, eBuilding30D, eBuilding33D, eBuilding34D, eBuilding77D, eBuilding78D, eBuilding100D, eBuilding103D,
	eParkingLotD, eHotelD,
	eBenchD, eGrassD,

	// BackGround Image
	eTitleBackGroundD, eLoadingBackGroundD,

	// Road
	eRoad1D, eRoad2D, eCenterRoadD, eCrossRoadD,
	eRoad1N, eRoad2N, eCenterRoadN, eCrossRoadN,
	eSideWalk1D, eSideWalk2D,

	// UI
	eStartButtonOn, eStartButtonOff, eExitButtonOn, eExitButtonOff,
	eCaptureArea, eAim, eMagazineUI, eLifeUI, eScoreUI,

	// Etc
	eBusStopD, eStreetLampD, eBarricadeD,

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
	eTerrorist, eTerrorist_Arm,
	eTest2,
	// --------------------------------------------------------- // 
	MaxSkinnedMesh = 100,
	// --------------------------------------------------------- // 

	ePolice,
	// Character
	eDarkFighter, eRifle,

	// Environment

	// Building
	eBuilding19, eBuilding20, eBuilding21, eBuilding22, eBuilding30, eBuilding33, eBuilding34, eBuilding77, eBuilding78, eBuilding100, eBuilding103, eBuilding104,
	eParkingLot, eHotel,

	// Road
	eRoad, eCenterRoad, eCrossRoad,


	// Etc
	eBench, eBusStop, eStreetLamp, eBarricade,

	// Etc 2
	eTest,
	eBox100m
};

enum class ObjectTag
{
	eNone,

	// Road
	eRoad1, eRoad2, eCenterRoad, eCrossRoad,
	eSideWalk1, eSideWalk2,

	// Building
	eBuilding19, eBuilding20, eBuilding21, eBuilding22, eBuilding30, eBuilding33, eBuilding34, eBuilding77, eBuilding78, eBuilding100, eBuilding103, eBuilding104,
	eParkingLot, eHotel, 

	// Etc
	eBench, eGrass, eBusStop, eStreetLamp, eBarricade
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
