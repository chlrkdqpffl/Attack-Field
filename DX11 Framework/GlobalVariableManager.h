#pragma once
#include "SingletonManager.h"
#include "LineObject.h"

struct TessOption
{
	float g_fMinDistance = 0.0f;
	float g_fMaxDistance = 0.0f;
	float g_fMinTessFactor = 0.0f;
	float g_fMaxTessFactor = 0.0f;

	float g_fMipLevelInterval = 0.0f;
	float g_fMaxMipLevel = 0.0f;
	float g_fHeightScale = 0.0f;
	float padding = 0.0f;
};

class CGlobalVariableManager : public CSingletonManager<CGlobalVariableManager>
{
public:
	CGlobalVariableManager();
	virtual ~CGlobalVariableManager();

	virtual void InitializeManager() override;
	void UpdateManager();
	virtual void ReleseManager() override;

	bool SetTimer(DWORD& timeVar, int time);

	// ------- Timer ------- //
	chrono::system_clock::time_point		g_startTime;
	chrono::milliseconds  					g_GapMilliseconds;
	chrono::microseconds					g_GapMicroseconds;
	chrono::nanoseconds						g_GapNanoseconds;


	// ----- Rendering Option ----- //
	static XMFLOAT4							g_vRenderOption;		// (x : Fog Render, y : BoundingBox Render)
	static ID3D11Buffer						*g_pd3dcbRenderOption;
	static ID3D11Buffer						*g_pd3dcbTessOption;

	static bool								g_bShowGBuffer;
	static bool								g_bShowWorldAxis;
	static bool								g_bShowWireFrame;
	bool									g_bShowLightVolume = false;
	bool									g_bEnablePostFX = false;

	vector<CLineObject*>					g_vecLineContainer;
};