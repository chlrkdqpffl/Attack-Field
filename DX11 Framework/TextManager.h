#pragma once
#include "SingletonManager.h"
#include "../Setting/FW1FontWrapper/FW1FontWrapper.h"


#ifdef _M_IX86		// 32��Ʈ
#pragma comment(lib, "../Setting/FW1FontWrapper/x86/FW1FontWrapper.lib")
#elif _M_X64		// 64��Ʈ
#pragma comment(lib, "../Setting/FW1FontWrapper/x64/FW1FontWrapper.lib")
#endif

//#pragma comment(lib, "../Setting/FW1FontWrapper/x86/FW1FontWrapper.lib")
// �������α׷��� ����� �������� ���߽��ϴ�. (0xc000007b) ������ FW1FontWrapper.dll �� 32��Ʈ���� �ƴϹǷ� 32��Ʈ������ �����




// �������α׷��� ����� �������� ���߽��ϴ�. (0xc000007b) ������ FW1FontWrapper.dll �� 64��Ʈ���� �ƴϹǷ� 64��Ʈ������ �����

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