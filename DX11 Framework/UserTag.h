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
//	eRightMouse		= 0x400,		// Zoom In
//	eMouseWheel		= 0x800,		// Zoom Ratio
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

enum class TeamTag
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
enum class WeaponTag
{
	eNone = -1,

	eRifle, eSniperRifle,
	eMaxWeaponCount
};

// ============================================================================================================================ //
// ===================================================== Animation Data ======================================================= //
enum class CharacterParts
{
	Defalut, UpperBody, LowerBody
};

enum class AnimationType
{
	eLoop, eInverseLoop, eOnce, ePingPong
};

enum class AnimationTag
{
	eNone,
	// --------------------------------------------------------- // 
	// Whole Body
	eIdle, eDeath, eDeath_Head, eRun,

	// --------------------------------------------------------- // 
	// Upper Body
	UpperBodyAnim = 100,
	eFire, eFireLoop, eReload, eHeadHit,
	// --------------------------------------------------------- // 
	// Lower Body
	LowerBodyAnim = 1000,

	eWalk_Left, eWalk_ForwardLeft, eWalk_Forward, eWalk_ForwardRight, eWalk_Right, eWalk_BackwardRight, eWalk_Backward, eWalk_BackwardLeft,
	eCrouch
};

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

	// Character
	eTerroristD_Red, eTerroristD_Blue, eTerroristND,
	ePoliceD,
	eRifleD, eRifleN,
	eSniperRifleD,
	eZoomScopeD,

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
	eWoodBoxD, eWoodBoxN, eWoodBoardD, eWoodBoardN,

	// Stair
	eStairD,

	// UI
	eStartButtonOn, eStartButtonOff, eExitButtonOn, eExitButtonOff,
	eMagazineUI, eLifeUI, eScoreUI, eDamagedCharacterUI, eRespawnGageBar, eRespawnGageWhiteBar, eOccupyGageBar, eOccupyGageWhiteBar,
	eDamageDirection_Left, eDamageDirection_LeftTop, eDamageDirection_Top, eDamageDirection_RightTop, eDamageDirection_Right, eDamageDirection_Bottom,
	eRifleKill, eRifleKill2, eRifleKill3, eRifleKill4, eRifleKill5, eRifleKill6,
	eSniperKill, eSniperKill2, eSniperKill3, eSniperKill4, eSniperKill5, eSniperKill6,

	// Etc
	eBarricadeD, eStoneWallD, eStoneWallN,

	// Etc 2
	eStoneD, eStoneND, eBricksD, eBricksN,
	eWallD, eWallND,

	// UI2
	eDeathOn, eOccupyOn, eDeathOff, eOccupyOff,

	// Particle
	eFire, eRain, eSpark,
	eBlood1, eBlood2, eBlood3,

	// Decal
	eDecal_Concrete, eDecal_White,

	// Sprite
	eExplosionSprite, eGunExplosionOtherPlayer, eGunExplosionPlayer,

	// Search
	eSearch1, eSearch2, eSearch3, eSearch4, eSearch5, eSearch6, eSearch7, eSearch8, eSearch9,
	eDeathSearch, eOccupySearch

};

// ========================================================================================================================= //
// ======================================================= Mesh Data ======================================================= //
enum class MeshTag
{
	eNone,

	// Skinned
	eTerrorist, eTerrorist_Arm,
	// --------------------------------------------------------- // 
	MaxSkinnedMesh = 100,
	// --------------------------------------------------------- // 

	ePolice,

	// Character
	eRifle, eSniperRifle,

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

	eTest
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
	eSniperRifleFire, eSniperShellsFall, eBeep,
	
	// Environment
	eFire, eThunderStrike, eThunderStrike2, eThunderStrike3, eThunderStrike4,

	SoundCount
};

enum class GBufferType
{
	eNone,
	eDepth, eDiffuse, eNormal, eSpec
};
