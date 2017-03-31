#pragma once
#include "SingletonManager.h"
#include "../Setting/FW1FontWrapper/FW1FontWrapper.h"

// 32��Ʈ ������
//#pragma comment(lib, "../Setting/FW1FontWrapper/x86/FW1FontWrapper.lib")
// �������α׷��� ����� �������� ���߽��ϴ�. (0xc000007b) ������ FW1FontWrapper.dll �� 32��Ʈ���� �ƴϹǷ� 32��Ʈ������ �����

// 64��Ʈ ������
#pragma comment(lib, "../Setting/FW1FontWrapper/x64/FW1FontWrapper.lib")
				
// �������α׷��� ����� �������� ���߽��ϴ�. (0xc000007b) ������ FW1FontWrapper.dll �� 64��Ʈ���� �ƴϹǷ� 64��Ʈ������ �����

enum TEXT
{
	PIVOT_TOP = FW1_TOP,
	PIVOT_LEFT = FW1_LEFT,
	PIVOT_CENTER = FW1_CENTER,
	PIVOT_RIGHT = FW1_RIGHT,
	PIVOT_VCENTER = FW1_VCENTER,
	PIVOT_BOTTOM = FW1_BOTTOM
};

class CTextManager : public CSingletonManager<CTextManager>
{
private:
	IFW1FontWrapper*		m_pFW1Font;
	IFW1TextRenderer*		m_pFWTextRender;
	IFW1Factory*			m_pFW1FontFactory;
	IFW1GlyphProvider*		m_pFW1Glyphrovider;
	TCHAR*					m_tChar;
	float					m_fResizeRatioX = 1.0f;
	float					m_fResizeRatioY = 1.0f;

public:
	CTextManager();
	virtual ~CTextManager();

	void RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string text, float fontSize,
		float posX, float posY, UINT32 color = 0xFF000000, UINT pivot = (PIVOT_LEFT | PIVOT_TOP));

	void RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::wstring text, float fontSize,
		float posX, float posY, UINT32 color = 0xFF000000, UINT pivot = (PIVOT_LEFT | PIVOT_TOP));

public:
	virtual void InitializeManager() {};
	void InitializeManager(ID3D11Device* pd3dDevice, std::wstring font = L"���� ���");
	virtual void ReleseManager() override;

	void SetResizeRatio(float x, float y) { m_fResizeRatioX = x, m_fResizeRatioY = y; }
	XMFLOAT2 GetResizeRatio() const { return XMFLOAT2(m_fResizeRatioX, m_fResizeRatioY); }
};