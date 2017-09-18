#pragma once
#include "SingletonManager.h"
#include "../Setting/FW1FontWrapper/FW1FontWrapper.h"


#ifdef _M_IX86		// 32비트
#pragma comment(lib, "../Setting/FW1FontWrapper/x86/FW1FontWrapper.lib")
#elif _M_X64		// 64비트
#pragma comment(lib, "../Setting/FW1FontWrapper/x64/FW1FontWrapper.lib")
#endif

//#pragma comment(lib, "../Setting/FW1FontWrapper/x86/FW1FontWrapper.lib")
// 응용프로그램을 제대로 시작하지 못했습니다. (0xc000007b) 오류는 FW1FontWrapper.dll 이 32비트용이 아니므로 32비트용으로 덮어쓰기




// 응용프로그램을 제대로 시작하지 못했습니다. (0xc000007b) 오류는 FW1FontWrapper.dll 이 64비트용이 아니므로 64비트용으로 덮어쓰기

class CTextManager : public CSingletonManager<CTextManager>
{
private:
	IFW1FontWrapper*		m_pFW1Font			= nullptr;
	IFW1TextRenderer*		m_pFWTextRender		= nullptr;
	IFW1Factory*			m_pFW1FontFactory	= nullptr;
	IFW1GlyphProvider*		m_pFW1Glyphrovider	= nullptr;
	TCHAR*					m_tChar;
	float					m_fResizeRatioX = 1.0f;
	float					m_fResizeRatioY = 1.0f;

public:
	CTextManager();
	virtual ~CTextManager();

	void RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string text, float fontSize,
		float posX, float posY, UINT32 color = 0xFF000000, UINT pivot = (FW1_LEFT | FW1_TOP));

	void RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::wstring text, float fontSize,
		float posX, float posY, UINT32 color = 0xFF000000, UINT pivot = (FW1_LEFT | FW1_TOP));

public:
	virtual void InitializeManager() {};
	void InitializeManager(ID3D11Device* pd3dDevice);
	virtual void ReleseManager() override;

	void AddFont();
	void SetResizeRatio(float x, float y) { m_fResizeRatioX = x, m_fResizeRatioY = y; }
	XMFLOAT2 GetResizeRatio() const { return XMFLOAT2(m_fResizeRatioX, m_fResizeRatioY); }
};