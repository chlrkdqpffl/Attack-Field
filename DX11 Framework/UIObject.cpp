#include "stdafx.h"
#include "UIObject.h"
#include "MainScene.h"
#include "ScreenShader.h"

CUIObject::CUIObject(TextureTag tag) : m_tagTexture(tag)
{
	m_pMaterial = new CMaterial();

	CTexture *pTexture = new CTexture(1, 1, PS_TEXTURE_SLOT, PS_SAMPLER_SLOT);
	pTexture->SetTexture(0, tag);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWarpSS);

	m_pMaterial->SetTexture(pTexture);
}

CUIObject::~CUIObject()
{
	SafeDelete(m_pMesh);
	SafeDelete(m_pMaterial);
}

bool CUIObject::IsCollision(POINT mousePos)
{
	if (m_ptStartPos.x * TEXT_MGR->GetResizeRatio().x <= mousePos.x && mousePos.x <= m_ptEndPos.x * TEXT_MGR->GetResizeRatio().x) {
		if (m_ptStartPos.y * TEXT_MGR->GetResizeRatio().y <= mousePos.y && mousePos.y <= m_ptEndPos.y * TEXT_MGR->GetResizeRatio().y)
			return true;
	}
	return false;
}

void CUIObject::Initialize(ID3D11Device* pDevice, POINT startPos, POINT endPos, float zPos)
{
	m_ptStartPos = startPos;
	m_ptEndPos = endPos;

	m_pMesh = new CUIMesh(pDevice, m_ptStartPos, m_ptEndPos, zPos);
	if (m_pMesh) m_pMesh->AddRef();
}

void CUIObject::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pDeviceContext);

	m_pMesh->Render(pDeviceContext);
}

// ============================================================================================================================  //
// ======================================================== UI Manager ========================================================  //

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
	SafeDelete(m_pUIShader);
	SafeDelete(m_pBackGroundUI);

	for (auto& obj : m_vecUIObject)
		SafeDelete(obj);
}

CUIObject* CUIManager::GetUIObject(TextureTag tag)
{
	for (auto& obj : m_vecUIObject)
		if (obj->GetTag() == tag)
			return obj;
}

void CUIManager::Initialize(ID3D11Device* pDevice)
{
	m_pUIShader = new CScreenShader();
	m_pUIShader->CreateShader(pDevice);
}

TextureTag CUIManager::FindCollisionUIObject(POINT mousePos)
{
	for (auto& uiObj : m_vecUIObject) {
		if (uiObj->GetActive()) {
			if (uiObj->IsCollision(mousePos)) 
				return uiObj->GetTag();
		}
	}

	return TextureTag::eNone;
}

void CUIManager::RenderAll(ID3D11DeviceContext* pDeviceContext)
{
	m_pUIShader->OnPrepareRender(pDeviceContext);

	pDeviceContext->OMSetBlendState(STATEOBJ_MGR->g_pTransparentBS, NULL, 0xffffffff);

	m_pBackGroundUI->Render(pDeviceContext);

	for (auto& uiObj : m_vecUIObject) {
		if (uiObj->GetActive())
			uiObj->Render(pDeviceContext);
	}

	pDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}