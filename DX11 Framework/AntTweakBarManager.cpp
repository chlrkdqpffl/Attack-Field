#include "stdafx.h"
#include "AntTweakBarManager.h"

CAntTweakBarManager::CAntTweakBarManager()
{
}


CAntTweakBarManager::~CAntTweakBarManager()
{
}

void CAntTweakBarManager::InitializeManager()
{
	TwInit(TW_DIRECT3D11, STATEOBJ_MGR->g_pd3dDevice);

	g_tweakBar = TwNewBar("TweakBar");

	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	TwDefine(" TweakBar size = '300 300'");
	TwDefine(" TweakBar alpha = 230");   // transparent bar
	TwDefine(" TweakBar position = '30 100'");

	// ------ Option ----- //
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Animation Blending", TW_TYPE_BOOLCPP, &g_isAnimBlending, "group = Option");


	// ------ Animation Option ----- //
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Bone Index", TW_TYPE_UINT32, &g_nBoneIndex, "group = AnimationOption");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Animation Speed", TW_TYPE_FLOAT, &g_fAnimationSpeed, "group = AnimationOption step = 0.1");

	TwAddVarRW(TWBAR_MGR->g_tweakBar, "OffsetX", TW_TYPE_FLOAT, &g_xmf3Offset.x, "group = AnimationOption step=0.1");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "OffsetY", TW_TYPE_FLOAT, &g_xmf3Offset.y, "group = AnimationOption step=0.1");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "OffsetZ", TW_TYPE_FLOAT, &g_xmf3Offset.z, "group = AnimationOption step=0.1");

	TwAddVarRW(TWBAR_MGR->g_tweakBar, "RotateX", TW_TYPE_FLOAT, &g_xmf3Rotate.x, "group = AnimationOption step=5");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "RotateY", TW_TYPE_FLOAT, &g_xmf3Rotate.y, "group = AnimationOption step=5");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "RotateZ", TW_TYPE_FLOAT, &g_xmf3Rotate.z, "group = AnimationOption step=5");
}

void CAntTweakBarManager::ReleseManager()
{
	TwTerminate();
}