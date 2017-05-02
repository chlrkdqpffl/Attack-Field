#pragma once
#include "SingletonManager.h"
#include "LineObject.h"

class CGlobalVariableManager : public CSingletonManager<CGlobalVariableManager>
{
public:
	CGlobalVariableManager();
	virtual ~CGlobalVariableManager();

	virtual void InitializeManager() override;
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

	static bool								g_bShowGBuffer;
	static bool								g_bShowWorldAxis;
	static bool								g_bShowWireFrame;

	vector<CLineObject*>					g_vecLineContainer;
};