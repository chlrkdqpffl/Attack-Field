#pragma once

// ========================================================================================================================== //
// ====================================================== KeyInput ========================================================== //

enum class KeyInput
{
	eNone,

	// Direction & Moving
	eForward		= 0x01,			// W
	eBackward		= 0x02,			// S
	eLeft			= 0x04,			// A
	eRight			= 0x08,			// D

	eRun			= 0x10,			// Shift
	eReload			= 0x20,			// R
	eOccupy			= 0x40,			// E
	eJump			= 0x80,			// Space Bar

	eCrouch			= 0x100,		// Ctrl

	// Mouse
	eLeftMouse		= 0x200,
//	eRightMouse		= 0x400,		// Zoom In 기능 용도
//	eMouseWheel		= 0x800,		// 휠 위아래는 추후 구현
};

enum class CameraTag 
{
	eNone,

	eFirstPerson, eFreeCam, eThirdPerson,

	eDeadCam, 
};

enum class GameMode
{
	eNone,
	eDeathMatch, eOccupy
};

enum class TeamType
{
	eNone,
	eRedTeam, eBlueTeam
};

enum class SceneTag 
{
	eNone, eTitleScene, eLoadingScene, eMainScene, eWaitScene
};

enum class ShaderTag
{
	eNone,
	eColor, eNormal, eTexture, eNormalTexture, eNormalTangentTexture, eDisplacementMapping,

	// Instancing
	eInstanceNormal, eInstanceNormalTexture, eInstanceNormalTangentTexture, eInstanceDisplacementMapping
};

enum class ChracterBoundingBoxParts 
{
	eNone = -1,
	eBody, eHead,
	eLeftUpArm, eLeftDownArm, eRightUpArm, eRightDownArm,
	eLeftUpLeg, eLeftDownLeg, eRightUpLeg, eRightDownLeg,
	
	ePartsCount
};

enum class ParticleTag
{
	eNone,
	eFire,

	// ======================================= // 
	MaxPostProcessingParticle = 100,
	// ======================================= // 
	eRain,
	eSpark, eFireSpark,
	eBleeding, eCopiousBleeding,
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
		eCrouch
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

	eTerroristD_Red, eTerroristD_Blue, eTerroristND,
	ePoliceD,
	eRifleD, eRifleN,

	// Building
	eBuilding19D, eBuilding20D, eBuilding21D, eBuilding22D, eBuilding30D, eBuilding33D, eBuilding34D, eBuilding77D, eBuilding78D, eBuilding100D, eBuilding103D,
	eGrassD,

	// BackGround Image
	eTitleBackGroundD, eLoadingBackGroundD,

	// Road
	eRoad1D, eRoad2D, eCenterRoadD, eCrossRoadD,
	eRoad1N, eRoad2N, eCenterRoadN, eCrossRoadN,
	eSideWalk1D, eSideWalk2D,
	eSideWalk1ND, eSideWalk2N,

	// Container
	eContainerRedD, eContainerBlueD, eContainerYellowD, eContainerN,

	// Barrel
	eFireBarrelD, eFireBarrelN, eOccupyBarrelD, eOccupyBarrelN,

	// StreetLamp
	eStreetLampD, eStreetLampN,

	// WoodBox, Board
	eWoodBoxD, eWoodBoardD,

	// Stair
	eStairD,

	// UI
	eStartButtonOn, eStartButtonOff, eExitButtonOn, eExitButtonOff,
	eCaptureArea, eMagazineUI, eLifeUI, eScoreUI, eDamagedCharacterUI, eRespawnGageBar, eRespawnGageWhiteBar, eOccupyGageBar, eOccupyGageWhiteBar,
	eDamageDirection_Left, eDamageDirection_LeftTop, eDamageDirection_Top, eDamageDirection_RightTop, eDamageDirection_Right, eDamageDirection_Bottom,

	// Etc
	eBarricadeD, eStoneWallD, eStoneWallN,

	// Etc 2
	eStoneD, eStoneND, eBricksD, eBricksN,
	eWallD, eWallND,

	// UI2
	eID, ePassword, eIDClick, ePasswordClick, eLogin, eLoginon,eDeathOn, eOccupyOn, eDeathOff, eOccupyOff, eLoginBack,

	// Particle
	eFire, eRain, eSpark,
	eBlood1, eBlood2, eBlood3,

	// Sprite
	eExplosionSprite, eGunExplosionOtherPlayer, eGunExplosionPlayer,

	// Search
	eSearch1, eSearch2, eSearch3, eSearch4, eSearch5, eSearch6, eSearch7, eSearch8, eSearch9, eSearch10, eSearch11, eSearch12, eSearch13,
	eDeathSearch, eOccupySearch

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

	// Road
	eRoad, eCenterRoad, eCrossRoad,

	// Container
	eContainer1, eContainer2, eContainer3,

	// WoodBox, Board
	eWoodBox, eWoodBoard,

	// Stair
	eStair, eStair2,

	// Barrel
	eFireBarrel, eOccupyBarrel,

	// Etc
	eStreetLamp, eBarricade,

	// Etc 2
	eTest,
};

enum class ObjectTag
{
	eNone,

	// Road
	eRoad1, eRoad2, eCenterRoad, eCrossRoad,
	eSideWalk1, eSideWalk2,

	// Building
	eBuilding19, eBuilding20, eBuilding21, eBuilding22, eBuilding30, eBuilding33, eBuilding34, eBuilding77, eBuilding78, eBuilding100, eBuilding103, eBuilding104,
	eStoneWall,

	// Container
	eContainer1Red, eContainer2Red, eContainer3Red,
	eContainer1Blue, eContainer2Blue, eContainer3Blue,
	eContainer1Yellow, eContainer2Yellow, eContainer3Yellow,

	// WoodBox, Board
	eWoodBox, eWoodBoard,

	// Stair
	eStair, eStair2,

	// Barrel
	eFireBarrel, eOccupyBarrel,

	// Etc
	eGrass, eStreetLamp, eBarricade
};

// =========================================================================================================================== //
// ===================================================== Sound Data ======================================================== //
enum class SoundTag
{
	eNone = -1,

	// BGM
	eBGM_Rain,

	// Effect
	eGunFire, eReload, eWalk, eRun, eDeath, eShellsFall,

	// Environment
	eFire, eThunderStrike, eThunderStrike2, eThunderStrike3, eThunderStrike4,

	SoundCount
};

/*
enum SoundChannel
{
	eChannel_Bgm,
	eChannel_Player,
	eChannel_OtherPlayer,

	ChannelCount
};
*/


enum class GBufferType
{
	eNone,
	eDepth, eDiffuse, eNormal, eSpec
};
