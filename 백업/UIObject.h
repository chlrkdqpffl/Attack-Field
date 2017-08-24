#pragma once
#include "UIMesh.h"
#include "DynamicUIMesh.h"

class CUIObject
{
	CUIMesh					*m_pMesh = nullptr;
	CDynamicUIMesh			*m_pDynamicMesh = nullptr;
	CMaterial				*m_pMaterial = nullptr;

	POINT					m_ptStartPos;
	POINT					m_ptEndPos;
	bool					m_bActive = true;
	TextureTag				m_tagTexture;
	float					m_fOpacity = 1.0f;
	bool					m_bIsDynamic = false;

public:
	CUIObject(TextureTag);
	virtual ~CUIObject();

	void Initialize(ID3D11Device* pDevice, POINT startPos, POINT endPos, float zPos, bool bIsDynamic = false);
	bool IsCollision(POINT mousePos);

	void Render(ID3D11DeviceContext* pDeviceContext);

	// ----- Get, Setter ----- //
	bool GetActive() const { return m_bActive; }
	TextureTag GetTag() const { return m_tagTexture; }

	void SetMaterial(CMaterial* pMaterial) { m_pMaterial = pMaterial; }
	void SetActive(bool set) { m_bActive = set; }
	void AddOpacity(float set);
	float GetOpacity() const { return m_fOpacity; }

	void SetStartPos(POINT pos) { m_pDynamicMesh->SetStartPos(pos); }
	void SetEndPos(POINT pos) { m_pDynamicMesh->SetEndPos(pos); }
};

class CUIManager
{
	CShader					*m_pUIShader = nullptr;
	CUIObject*				m_pBackGroundUI = nullptr;
	CUIObject*				m_ptempBackGroundUI = nullptr;
	vector<CUIObject*>		m_vecUIObject;
	float					m_fSettingOpacity = 1.0f;

public:
	CUIManager();
	virtual ~CUIManager();

	void Initialize(ID3D11Device* pDevice);

	TextureTag FindCollisionUIObject(POINT mousePos);
	void AddUIObject(CUIObject* object) { m_vecUIObject.push_back(object); };
	void Render(ID3D11DeviceContext* pDeviceContext, TextureTag tag);
	void RenderAll(ID3D11DeviceContext* pDeviceContext);

	// ----- Get, Setter ----- //
	CUIObject* GetUIObject(TextureTag tag);

	void SetBackGroundUI(CUIObject* ui) { m_pBackGroundUI = ui; }
	void SettempBackGroundUI(CUIObject* ui) { m_ptempBackGroundUI = ui; }
};