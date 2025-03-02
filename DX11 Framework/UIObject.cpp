#include "stdafx.h"
#include "UIObject.h"
#include "MainScene.h"
#include "UIShader.h"

CUIObject::CUIObject(TextureTag tag) : m_tagTexture(tag)
{
	m_pMaterial = new CMaterial();

	CTexture *pTexture = new CTexture(1, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);
	pTexture->SetTexture(0, tag);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWrapSS);

	m_pMaterial->SetTexture(pTexture);
}

CUIObject::~CUIObject()
{
	SafeDelete(m_pMesh);
	SafeDelete(m_pDynamicMesh);
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

void CUIObject::Initialize(ID3D11Device* pDevice, POINT startPos, POINT endPos, float zPos, bool bIsDynamic)
{
	m_ptStartPos = startPos;
	m_ptEndPos = endPos;
	m_bIsDynamic = bIsDynamic;

	if(bIsDynamic)
		m_pDynamicMesh = new CDynamicUIMesh(pDevice, m_ptStartPos, m_ptEndPos, zPos);
	else
		m_pMesh = new CUIMesh(pDevice, m_ptStartPos, m_ptEndPos, zPos);
}

void CUIObject::AddOpacity(float set)
{
	m_fOpacity += set;
	m_fOpacity = clamp(m_fOpacity, 0.0f, 1.0f);
}

void CUIObject::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pDeviceContext);

	if (m_bIsDynamic)
		m_pDynamicMesh->Render(pDeviceContext);
	else
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
	m_pUIShader = new CUIShader();
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
	pDeviceContext->RSSetState(STATEOBJ_MGR->g_pNoCullRS);

	ID3D11DepthStencilState* prevDSS; UINT prevStencil;
	pDeviceContext->OMGetDepthStencilState(&prevDSS, &prevStencil);
	pDeviceContext->OMSetDepthStencilState(STATEOBJ_MGR->g_pDisableDepthDSS, 0);

	if(m_pBackGroundUI) 
		m_pBackGroundUI->Render(pDeviceContext);

	for (auto& uiObj : m_vecUIObject) {
		if (uiObj->GetActive()) { 
			// Opacity Update
			if (uiObj->GetOpacity() != m_fSettingOpacity) {
				m_fSettingOpacity = uiObj->GetOpacity();

				GLOBAL_MGR->g_vRenderOption.w = m_fSettingOpacity;

				D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
				STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Map(GLOBAL_MGR->g_pd3dcbRenderOption, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
				XMFLOAT4 *pcbRenderOption = (XMFLOAT4 *)d3dMappedResource.pData;
				*pcbRenderOption = GLOBAL_MGR->g_vRenderOption;
				STATEOBJ_MGR->g_pd3dImmediateDeviceContext->Unmap(GLOBAL_MGR->g_pd3dcbRenderOption, 0);
			}
			uiObj->Render(pDeviceContext);
		}
	}

	pDeviceContext->OMSetDepthStencilState(prevDSS, prevStencil);
	pDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);
	pDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}