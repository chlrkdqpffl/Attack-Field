#pragma once
#include "SingletonManager.h"

class CGlobalVariableManager : public CSingletonManager<CGlobalVariableManager>
{
public:
	CGlobalVariableManager();
	virtual ~CGlobalVariableManager();

	virtual void InitializeManager() override;
	void UpdateManager();
	virtual void ReleseManager() override;


	// ------- Timer ------- //
	chrono::system_clock::time_point		g_startTime;
	chrono::milliseconds  					g_timeGap;


	// ----- Rendering Option ----- //
	static XMFLOAT4							g_vRenderOption;		// (x : Fog, y : BoundingBox )
	static ID3D11Buffer						*g_pd3dcbRenderOption;
	static bool								g_bShowWorldAxis;
};

