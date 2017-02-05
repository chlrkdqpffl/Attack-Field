#pragma once
#include "SingletonManager.h"

class CAntTweakBarManager : public CSingletonManager<CAntTweakBarManager>
{
public:
	TwBar *g_tweakBar;


	// Option
	bool g_isAnimBlending = true;
	
public:
	CAntTweakBarManager();
	virtual ~CAntTweakBarManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;
};