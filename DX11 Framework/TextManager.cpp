#include "stdafx.h"
#include "TextManager.h"

CTextManager::CTextManager()
{
	m_tChar = new TCHAR[100];
}

CTextManager::~CTextManager()
{
	delete[] m_tChar;
}

void CTextManager::InitializeManager(ID3D11Device* pd3dDevice)
{
	AddFont();
	wstring fontName = L"Koverwatch";

	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1FontFactory)))
		MessageBox(NULL, L"Do not Create FontFactory", L"Error", MB_OK);

	if (FAILED(m_pFW1FontFactory->CreateFontWrapper(pd3dDevice, fontName.c_str(), &m_pFW1Font)))
		MessageBox(NULL, L"Font is None", L"Font Error", MB_OK);
}

void CTextManager::AddFont()
{
	AddFontResource(L"../Assets/Font/Koverwatch.ttf");
}

void CTextManager::ReleseManager()
{
	if (m_pFW1FontFactory) {
		m_pFW1FontFactory->Release();
		m_pFW1FontFactory = nullptr;
	}

	if (m_pFW1Font) {
		m_pFW1Font->Release();
		m_pFW1Font = nullptr;
	}
}

void CTextManager::RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::string text, float fontSize, float posX, float posY, UINT32 color, UINT pivot)
{
	if (!m_pFW1Font) return;

	m_pFW1Font->DrawString(pd3dDeviceContext, s_to_ws(text).c_str(), fontSize * m_fResizeRatioX, posX * m_fResizeRatioX, posY * m_fResizeRatioY, color, pivot);
}

void CTextManager::RenderText(ID3D11DeviceContext* pd3dDeviceContext, std::wstring text, float fontSize, float posX, float posY, UINT32 color, UINT pivot)
{
	if (!m_pFW1Font) return;
	
	m_pFW1Font->DrawString(pd3dDeviceContext, text.c_str(), fontSize * m_fResizeRatioX, posX * m_fResizeRatioX, posY * m_fResizeRatioY, color, pivot);
}