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

	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Offset X", TW_TYPE_FLOAT, &g_xmf3Offset.x, "group = AnimationOption step=0.05");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Offset Y", TW_TYPE_FLOAT, &g_xmf3Offset.y, "group = AnimationOption step=0.05");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Offset Z", TW_TYPE_FLOAT, &g_xmf3Offset.z, "group = AnimationOption step=0.05");

	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Rotate X", TW_TYPE_FLOAT, &g_xmf3Rotate.x, "group = AnimationOption step=0.05");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Rotate Y", TW_TYPE_FLOAT, &g_xmf3Rotate.y, "group = AnimationOption step=0.05");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Rotate Z", TW_TYPE_FLOAT, &g_xmf3Rotate.z, "group = AnimationOption step=0.05");

	// Select Object
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Select Position X", TW_TYPE_FLOAT, &g_xmf3SelectObjectPosition.x, "group = SelectObject step=5");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Select Position Y", TW_TYPE_FLOAT, &g_xmf3SelectObjectPosition.y, "group = SelectObject step=5");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Select Position Z", TW_TYPE_FLOAT, &g_xmf3SelectObjectPosition.z, "group = SelectObject step=5");

	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Select Rotate X", TW_TYPE_FLOAT, &g_xmf3SelectObjectRotate.x, "group = SelectObject step=5");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Select Rotate Y", TW_TYPE_FLOAT, &g_xmf3SelectObjectRotate.y, "group = SelectObject step=5");
	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Select Rotate Z", TW_TYPE_FLOAT, &g_xmf3SelectObjectRotate.z, "group = SelectObject step=5");

	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Select Number", TW_TYPE_UINT32, &g_nSelect, "");
}

void CAntTweakBarManager::ReleseManager()
{
	TwTerminate();
}