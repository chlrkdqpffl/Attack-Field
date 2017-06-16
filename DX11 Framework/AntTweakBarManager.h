#pragma once
#include "SingletonManager.h"

class CAntTweakBarManager : public CSingletonManager<CAntTweakBarManager>
{
public:
	TwBar *g_tweakBar;


	// Option

	// Etc
	int g_nSelect = 0;
	UINT g_nBoneIndex = 0;
	float g_fAnimationSpeed = 1.0f;
	XMFLOAT3 g_xmf3Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 g_xmf3Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT4 g_xmf3Quaternion = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Select Object
	XMFLOAT3 g_xmf3SelectObjectPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 g_xmf3SelectObjectRotate = XMFLOAT3(0.0f, 0.0f, 0.0f);

	TessOption	g_tessOption;

	// HDR Option
	float g_fMiddleGrey = 0.0f;
	float g_fWhite = 0.0f;
	float g_fAdaptation = 0.0f;
	float g_fBloomThreshold = 0.0f;
	float g_fBloomScale = 0.0f;

public:
	CAntTweakBarManager();
	virtual ~CAntTweakBarManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	void CreateHDROption();
};