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

	TwAddVarRW(TWBAR_MGR->g_tweakBar, "Animation Blending", TW_TYPE_BOOLCPP, &g_isAnimBlending, "group = Option");
}

void CAntTweakBarManager::ReleseManager()
{
	TwTerminate();
}