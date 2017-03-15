#pragma once
#include "SingletonManager.h"

class CAntTweakBarManager : public CSingletonManager<CAntTweakBarManager>
{
public:
	TwBar *g_tweakBar;


	// Option
	bool g_isAnimBlending = true;

	// Etc
	UINT g_nBoneIndex = 0;
	XMFLOAT3 g_xmf3Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 g_xmf3Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	
public:
	CAntTweakBarManager();
	virtual ~CAntTweakBarManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;
};