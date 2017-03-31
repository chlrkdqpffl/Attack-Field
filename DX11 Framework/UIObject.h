#pragma once
#include "UIMesh.h"

class CUIObject
{
	CMesh					*m_pMesh = nullptr;
	CMaterial				*m_pMaterial = nullptr;

	POINT					m_ptStartPos;
	POINT					m_ptEndPos;
	bool					m_bActive = true;
	TextureTag				m_tagTexture;

public:
	CUIObject(TextureTag);
	virtual ~CUIObject();

	void Initialize(ID3D11Device* pDevice, POINT startPos, POINT endPos, float zPos);
	bool IsCollision(POINT mousePos);

	void Render(ID3D11DeviceContext* pDeviceContext);

	// ----- Get, Setter ----- //
	bool GetActive() const { return m_bActive; }
	TextureTag GetTag() const { return m_tagTexture; }

	void SetMaterial(CMaterial* pMaterial) { m_pMaterial = pMaterial; }
	void SetActive(bool set) { m_bActive = set; }
};

class CUIManager
{
	CShader					*m_pUIShader = nullptr;
	CUIObject*				m_pBackGroundUI = nullptr;
	vector<CUIObject*>		m_vecUIObject;

public:
	CUIManager();
	virtual ~CUIManager();

	void Initialize(ID3D11Device* pDevice);
	
	TextureTag FindCollisionUIObject(POINT mousePos);
	void AddUIObject(CUIObject* object) { m_vecUIObject.push_back(object); };

	void RenderAll(ID3D11DeviceContext* pDeviceContext);

	// ----- Get, Setter ----- //
	CUIObject* GetUIObject(TextureTag tag);

	void SetBackGroundUI(CUIObject* ui) { m_pBackGroundUI = ui; }
};