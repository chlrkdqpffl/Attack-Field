//-----------------------------------------------------------------------------
// File: CGameObject.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Object.h"
#include "Shader.h"

///////////////////////////////////////////////////////////////////////////

CMaterialColors::CMaterialColors()
{
	m_nReferences = 0;
	m_d3dxcAmbient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_d3dxcDiffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	m_d3dxcEmissive = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

CMaterialColors::~CMaterialColors()
{
}

CMaterial::CMaterial(CMaterialColors *pColors)
{
	m_nReferences = 0;

	m_pColors = pColors;
	if (pColors) pColors->AddRef();

	m_pTexture = NULL;
}

CMaterial::~CMaterial()
{
	if (m_pColors) m_pColors->Release();
	if (m_pTexture) m_pTexture->Release();
}

void CMaterial::SetTexture(CTexture *pTexture)
{
	if (m_pTexture) m_pTexture->Release();
	m_pTexture = pTexture;
	if (m_pTexture) m_pTexture->AddRef();
}

void CMaterial::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pColors) CGameObject::UpdateConstantBuffer_Material(pd3dDeviceContext, m_pColors);
	if (m_pTexture) m_pTexture->UpdateShaderVariable(pd3dDeviceContext);
}

///////////////////////////////////////////////////////////////////////////
//
ID3D11Buffer *CTexture::m_pd3dcbTextureMatrix = NULL;

CTexture::CTexture(int nTextures, int nSamplers, int nTextureStartSlot, int nSamplerStartSlot)
{
	m_nReferences = 0;

	m_nTextures = nTextures;
	m_ppd3dsrvTextures = new ID3D11ShaderResourceView*[m_nTextures];
	for (int i = 0; i < m_nTextures; i++) m_ppd3dsrvTextures[i] = NULL;
	m_nTextureStartSlot = nTextureStartSlot;

	m_nSamplers = nSamplers;
	m_ppd3dSamplerStates = new ID3D11SamplerState*[m_nSamplers];
	for (int i = 0; i < m_nSamplers; i++) m_ppd3dSamplerStates[i] = NULL;
	m_nSamplerStartSlot = nSamplerStartSlot;
}

CTexture::~CTexture()
{
	for (int i = 0; i < m_nTextures; i++) 
		if (m_ppd3dsrvTextures[i]) 
			m_ppd3dsrvTextures[i]->Release();

	for (int i = 0; i < m_nSamplers; i++) 
		if (m_ppd3dSamplerStates[i]) 
			m_ppd3dSamplerStates[i]->Release();

	if (m_ppd3dsrvTextures) 
		delete[] m_ppd3dsrvTextures;
	if (m_ppd3dSamplerStates) 
		delete[] m_ppd3dSamplerStates;
}

void CTexture::SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture)
{
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->Release();
	m_ppd3dsrvTextures[nIndex] = pd3dsrvTexture;
	if (pd3dsrvTexture) pd3dsrvTexture->AddRef();
}

void CTexture::SetTexture(int nIndex, Resource_TextrueTag eTag)
{
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->Release();
	m_ppd3dsrvTextures[nIndex] = RESOURCE_MGR->FindResourceAndCreateSRV(eTag);
	m_Tag = eTag;
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->AddRef();
}

void CTexture::SetSampler(int nIndex, ID3D11SamplerState *pd3dSamplerState)
{
	if (m_ppd3dSamplerStates[nIndex]) m_ppd3dSamplerStates[nIndex]->Release();
	m_ppd3dSamplerStates[nIndex] = pd3dSamplerState;
	if (pd3dSamplerState) pd3dSamplerState->AddRef();
}

void CTexture::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->DSSetShaderResources(m_nTextureStartSlot, m_nTextures, m_ppd3dsrvTextures);
	pd3dDeviceContext->DSSetSamplers(m_nSamplerStartSlot, m_nSamplers, m_ppd3dSamplerStates);

	pd3dDeviceContext->PSSetShaderResources(m_nTextureStartSlot, m_nTextures, m_ppd3dsrvTextures);
	pd3dDeviceContext->PSSetSamplers(m_nSamplerStartSlot, m_nSamplers, m_ppd3dSamplerStates);
}

void CTexture::UpdateTextureShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, int nIndex, int nSlot)
{
	pd3dDeviceContext->PSSetShaderResources(nSlot, 1, &m_ppd3dsrvTextures[nIndex]);
}

void CTexture::UpdateSamplerShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, int nIndex, int nSlot)
{
	pd3dDeviceContext->PSSetSamplers(nSlot, 1, &m_ppd3dSamplerStates[nIndex]);
}

void CTexture::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(XMMATRIX);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbTextureMatrix);

	DXUT_SetDebugName(m_pd3dcbTextureMatrix, "TextureMatrix");
}

void CTexture::ReleaseShaderVariables()
{
	if (m_pd3dcbTextureMatrix) m_pd3dcbTextureMatrix->Release();
}

void CTexture::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxTexture)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbTextureMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pcbWorldMatrix = (XMMATRIX *)d3dMappedResource.pData;
	*pcbWorldMatrix = XMMatrixTranspose(*pd3dxmtxTexture);
	pd3dDeviceContext->Unmap(m_pd3dcbTextureMatrix, 0);
	
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_TEXTURE_MATRIX, 1, &m_pd3dcbTextureMatrix);
}

ID3D11ShaderResourceView *CTexture::CreateTexture2DArraySRV(ID3D11Device *pd3dDevice, _TCHAR(*ppstrFilePaths)[128], UINT nTextures)
{
	D3DX11_IMAGE_LOAD_INFO d3dxImageLoadInfo;
	d3dxImageLoadInfo.Width = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Height = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Depth = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.FirstMipLevel = 0;
	d3dxImageLoadInfo.MipLevels = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Usage = D3D11_USAGE_STAGING;
	d3dxImageLoadInfo.BindFlags = 0;
	d3dxImageLoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	d3dxImageLoadInfo.MiscFlags = 0;
	d3dxImageLoadInfo.Format = DXGI_FORMAT_FROM_FILE; //DXGI_FORMAT_R8G8B8A8_UNORM
	d3dxImageLoadInfo.Filter = D3DX11_FILTER_NONE;
	d3dxImageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;
	d3dxImageLoadInfo.pSrcInfo = 0;

	ID3D11Texture2D **ppd3dTextures = new ID3D11Texture2D*[nTextures];
	for (UINT i = 0; i < nTextures; i++) D3DX11CreateTextureFromFile(pd3dDevice, ppstrFilePaths[i], &d3dxImageLoadInfo, 0, (ID3D11Resource **)&ppd3dTextures[i], 0);

	D3D11_TEXTURE2D_DESC d3dTexure2DDesc;
	ppd3dTextures[0]->GetDesc(&d3dTexure2DDesc);

	D3D11_TEXTURE2D_DESC d3dTexture2DArrayDesc;
	d3dTexture2DArrayDesc.Width = d3dTexure2DDesc.Width;
	d3dTexture2DArrayDesc.Height = d3dTexure2DDesc.Height;
	d3dTexture2DArrayDesc.MipLevels = d3dTexure2DDesc.MipLevels;
	d3dTexture2DArrayDesc.ArraySize = nTextures;
	d3dTexture2DArrayDesc.Format = d3dTexure2DDesc.Format;
	d3dTexture2DArrayDesc.SampleDesc.Count = 1;
	d3dTexture2DArrayDesc.SampleDesc.Quality = 0;
	d3dTexture2DArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DArrayDesc.CPUAccessFlags = 0;
	d3dTexture2DArrayDesc.MiscFlags = 0;

	ID3D11Texture2D *pd3dTexture2DArray;
	pd3dDevice->CreateTexture2D(&d3dTexture2DArrayDesc, 0, &pd3dTexture2DArray);

	ID3D11DeviceContext *pd3dDeviceContext;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedTexture2D;
	for (UINT t = 0; t < nTextures; t++)
	{
		for (UINT m = 0; m < d3dTexure2DDesc.MipLevels; m++)
		{
			pd3dDeviceContext->Map(ppd3dTextures[t], m, D3D11_MAP_READ, 0, &d3dMappedTexture2D);
			pd3dDeviceContext->UpdateSubresource(pd3dTexture2DArray, D3D11CalcSubresource(m, t, d3dTexure2DDesc.MipLevels), 0, d3dMappedTexture2D.pData, d3dMappedTexture2D.RowPitch, d3dMappedTexture2D.DepthPitch);
			pd3dDeviceContext->Unmap(ppd3dTextures[t], m);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dTextureSRVDesc;
	ZeroMemory(&d3dTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dTextureSRVDesc.Format = d3dTexture2DArrayDesc.Format;
	d3dTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	d3dTextureSRVDesc.Texture2DArray.MostDetailedMip = 0;
	d3dTextureSRVDesc.Texture2DArray.MipLevels = d3dTexture2DArrayDesc.MipLevels;
	d3dTextureSRVDesc.Texture2DArray.FirstArraySlice = 0;
	d3dTextureSRVDesc.Texture2DArray.ArraySize = nTextures;

	ID3D11ShaderResourceView *pd3dsrvTextureArray;
	pd3dDevice->CreateShaderResourceView(pd3dTexture2DArray, &d3dTextureSRVDesc, &pd3dsrvTextureArray);

	if (pd3dTexture2DArray) pd3dTexture2DArray->Release();

	for (UINT i = 0; i < nTextures; i++) if (ppd3dTextures[i]) ppd3dTextures[i]->Release();
	delete[] ppd3dTextures;

	if (pd3dDeviceContext) pd3dDeviceContext->Release();

	return(pd3dsrvTextureArray);
}

///////////////////////////////////////////////////////////////////////////

ID3D11Buffer *CGameObject::m_pd3dcbWorldMatrix = NULL;
ID3D11Buffer *CGameObject::m_pd3dcbMaterialColors = NULL;

CGameObject::CGameObject(int nMeshes)
{
	XMStoreFloat4x4(&m_d3dxmtxLocal, XMMatrixIdentity());
	XMStoreFloat4x4(&m_d3dxmtxWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&m_d3dxmtxShadow, XMMatrixIdentity());
	
	m_pShader = NULL;
	m_pMaterial = NULL;

	m_nMeshes = nMeshes;
	m_ppMeshes = NULL;
	if (m_nMeshes > 0) m_ppMeshes = new CMesh*[m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++)	m_ppMeshes[i] = NULL;

	m_bcMeshBoundingCube.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bcMeshBoundingCube.Extents = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bcBoundingCube = BoundingBox();

	m_bActive = true;
	m_bIsVisible = true;

	m_nReferences = 0;

	m_pChild = m_pSibling = m_pParent = NULL;

	m_pd3dDepthStencilState = NULL;
	m_pd3dBlendState = NULL;
}

CGameObject::~CGameObject()
{
	if (m_pd3dDepthStencilState) m_pd3dDepthStencilState->Release();
	if (m_pd3dBlendState) m_pd3dBlendState->Release();

	if (m_pShader) m_pShader->Release();

	if (m_pMaterial) m_pMaterial->Release();

	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Release();
			m_ppMeshes[i] = NULL;
		}
		delete[] m_ppMeshes;
	}

	if (m_pSibling) delete m_pSibling;
	if (m_pChild) delete m_pChild;
}

void CGameObject::SetMesh(CMesh *pMesh, int nIndex)
{
	if (nIndex >= m_nMeshes)
	{
		CMesh **ppMeshes = new CMesh*[nIndex + 1];
		if (m_ppMeshes)
		{
			for (int i = 0; i < m_nMeshes; i++) ppMeshes[i] = m_ppMeshes[i];
			delete[] m_ppMeshes;
		}
		ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
		m_nMeshes = nIndex + 1;
		m_ppMeshes = ppMeshes;
	}
	else {
		if (m_ppMeshes)
		{
			if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
			m_ppMeshes[nIndex] = pMesh;
			if (pMesh) pMesh->AddRef();
		}
	}

	if (pMesh) {
		XMFLOAT3 xmMin, xmMax; // 기존값
		XMFLOAT3 input_xmMin, input_xmMax;		//새로 들어온거
		XMFLOAT3 fCenter, fExtern;

		fCenter = pMesh->GetBoundingCube().Center; 
		fExtern = pMesh->GetBoundingCube().Extents;

		XMStoreFloat3(&input_xmMin, XMVectorSubtract(XMLoadFloat3(&fCenter), XMLoadFloat3(&fExtern)));
		XMStoreFloat3(&input_xmMax, XMVectorAdd(XMLoadFloat3(&fCenter), XMLoadFloat3(&fExtern)));

		XMStoreFloat3(&xmMin, XMVectorSubtract(XMLoadFloat3(&m_bcMeshBoundingCube.Center), XMLoadFloat3(&m_bcMeshBoundingCube.Extents)));
		XMStoreFloat3(&xmMax, XMVectorAdd(XMLoadFloat3(&m_bcMeshBoundingCube.Center), XMLoadFloat3(&m_bcMeshBoundingCube.Extents)));
		
		if (input_xmMin.x < xmMin.x) xmMin.x = input_xmMin.x;
		if (input_xmMin.y < xmMin.y) xmMin.y = input_xmMin.y;
		if (input_xmMin.z < xmMin.z) xmMin.z = input_xmMin.z;
		if (input_xmMax.x > xmMax.x) xmMax.x = input_xmMax.x;
		if (input_xmMax.y > xmMax.y) xmMax.y = input_xmMax.y;
		if (input_xmMax.z > xmMax.z) xmMax.z = input_xmMax.z;
		
		XMStoreFloat3(&m_bcBoundingCube.Center, XMVectorSubtract(XMLoadFloat3(&input_xmMax), XMLoadFloat3(&input_xmMin)) * 0.5f);
		XMStoreFloat3(&m_bcBoundingCube.Extents, XMVectorAdd(XMLoadFloat3(&input_xmMax), XMLoadFloat3(&input_xmMin)) * 0.5f);

		BoundingBox::CreateMerged(m_bcMeshBoundingCube, m_bcMeshBoundingCube, pMesh->GetBoundingCube());
	}
}

void CGameObject::SetChild(CGameObject *pChild)
{
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}
	if (pChild) pChild->m_pParent = this;
}

void CGameObject::SetShader(CShader *pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

void CGameObject::SetMaterial(CMaterial *pMaterial)
{
	if (m_pMaterial) m_pMaterial->Release();
	m_pMaterial = pMaterial;
	if (m_pMaterial) m_pMaterial->AddRef();
}

void CGameObject::SetShadowMatrix(XMVECTOR d3dxvLight, XMVECTOR d3dxPlane)
{
	XMStoreFloat4x4(&m_d3dxmtxShadow, XMMatrixShadow(d3dxPlane, d3dxvLight));
}

void CGameObject::GenerateRayForPicking(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxWorld, XMMATRIX *pd3dxmtxView, XMVECTOR *pd3dxvPickRayPosition, XMVECTOR *pd3dxvPickRayDirection)
{
	XMMATRIX d3dxmtxInverse;
	XMMATRIX d3dxmtxWorldView = *pd3dxmtxView;
	if (pd3dxmtxWorld) d3dxmtxWorldView = XMMatrixMultiply(*pd3dxmtxWorld, *pd3dxmtxView);
	d3dxmtxInverse = XMMatrixInverse(NULL, d3dxmtxWorldView);
	/*
	pd3dxvPickRayDirection->x = pd3dxvPickPosition->x * d3dxmtxInverse._11 + pd3dxvPickPosition->y * d3dxmtxInverse._21 + pd3dxvPickPosition->z * d3dxmtxInverse._31;
	pd3dxvPickRayDirection->y = pd3dxvPickPosition->x * d3dxmtxInverse._12 + pd3dxvPickPosition->y * d3dxmtxInverse._22 + pd3dxvPickPosition->z * d3dxmtxInverse._32;
	pd3dxvPickRayDirection->z = pd3dxvPickPosition->x * d3dxmtxInverse._13 + pd3dxvPickPosition->y * d3dxmtxInverse._23 + pd3dxvPickPosition->z * d3dxmtxInverse._33;
	pd3dxvPickRayPosition->x = d3dxmtxInverse._41;
	pd3dxvPickRayPosition->y = d3dxmtxInverse._42;
	pd3dxvPickRayPosition->z = d3dxmtxInverse._43;
	*/
	XMVECTOR d3dxvCameraOrigin = XMVectorZero();
	*pd3dxvPickRayPosition = XMVector3TransformCoord(d3dxvCameraOrigin, d3dxmtxInverse);
	*pd3dxvPickRayDirection = XMVector3TransformCoord(*pd3dxvPickPosition, d3dxmtxInverse);
	*pd3dxvPickRayDirection = *pd3dxvPickRayDirection - *pd3dxvPickRayPosition;
}

int CGameObject::PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo)
{
	XMVECTOR d3dxvPickRayPosition, d3dxvPickRayDirection;
	int nIntersected = 0;
	if (m_bActive && m_bIsVisible && m_ppMeshes)
	{
		GenerateRayForPicking(pd3dxvPickPosition, &XMLoadFloat4x4(&m_d3dxmtxWorld), pd3dxmtxView, &d3dxvPickRayPosition, &d3dxvPickRayDirection);
		for (int i = 0; i < m_nMeshes; i++)
		{
			//nIntersected = m_ppMeshes[i]->CheckRayIntersection(&d3dxvPickRayPosition, &d3dxvPickRayDirection, pd3dxIntersectInfo);
			if (nIntersected > 0) break;
		}
	}
	return(nIntersected);
}

void CGameObject::SetPosition(float x, float y, float z)
{
//	m_d3dxmtxLocal._41 = x;
//	m_d3dxmtxLocal._42 = y;
//	m_d3dxmtxLocal._43 = z;
//	if (!m_pParent) { m_d3dxmtxWorld._41 = x; m_d3dxmtxWorld._42 = y; m_d3dxmtxWorld._43 = z; }
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y; 
	m_d3dxmtxWorld._43 = z;
}

void CGameObject::SetPosition(XMVECTOR d3dxvPosition)
{
	XMFLOAT4 f4vPosition;
	XMStoreFloat4(&f4vPosition, d3dxvPosition);
	SetPosition(f4vPosition.x, f4vPosition.y, f4vPosition.z);
}

void CGameObject::SetPosition(XMFLOAT3 pos)
{
	m_d3dxmtxWorld._41 = pos.x;
	m_d3dxmtxWorld._42 = pos.y;
	m_d3dxmtxWorld._43 = pos.z;
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMVECTOR d3dxvPosition = GetvPosition();
	XMVECTOR d3dxvRight = GetRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveUp(float fDistance)
{
	XMVECTOR d3dxvPosition = GetvPosition();
	XMVECTOR d3dxvUp = GetUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveForward(float fDistance)
{
	XMVECTOR d3dxvPosition = GetvPosition();
	XMVECTOR d3dxvLookAt = GetLook();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), 
		XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	XMStoreFloat4x4(&m_d3dxmtxWorld, mtxRotate * XMLoadFloat4x4(&m_d3dxmtxWorld));
}

void CGameObject::Rotate(XMVECTOR *pd3dxvAxis, float fAngle)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationAxis(*pd3dxvAxis, XMConvertToRadians(fAngle));
	XMStoreFloat4x4(&m_d3dxmtxWorld, mtxRotate * XMLoadFloat4x4(&m_d3dxmtxWorld));
}

XMVECTOR CGameObject::GetvPosition(bool bIsLocal) const
{
	return((bIsLocal) ? XMVectorSet(m_d3dxmtxLocal._41, m_d3dxmtxLocal._42, m_d3dxmtxLocal._43, 0.0f) : 
		XMVectorSet(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43, 0.0f));

	return XMVectorSet(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43, 0.0f);
}

XMFLOAT3 CGameObject::GetPosition() const
{
	return XMFLOAT3(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43);
}

XMVECTOR CGameObject::GetRight(bool bIsLocal)
{
	XMVECTOR d3dxvRight = (bIsLocal) ? XMVectorSet(m_d3dxmtxLocal._11, m_d3dxmtxLocal._12, m_d3dxmtxLocal._13, 0.0f) : 
		XMVectorSet(m_d3dxmtxWorld._11, m_d3dxmtxWorld._12, m_d3dxmtxWorld._13, 0.0f);
	d3dxvRight = XMVector3Normalize(d3dxvRight);
	return(d3dxvRight);
}

XMVECTOR CGameObject::GetUp(bool bIsLocal)
{
	XMVECTOR d3dxvUp = (bIsLocal) ? XMVectorSet(m_d3dxmtxLocal._21, m_d3dxmtxLocal._22, m_d3dxmtxLocal._23, 0.0f) : 
		XMVectorSet(m_d3dxmtxWorld._21, m_d3dxmtxWorld._22, m_d3dxmtxWorld._23, 0.0f);
	d3dxvUp = XMVector3Normalize(d3dxvUp);
	return(d3dxvUp);
}

XMVECTOR CGameObject::GetLook(bool bIsLocal)
{
	XMVECTOR d3dxvLookAt = (bIsLocal) ? XMVectorSet(m_d3dxmtxLocal._31, m_d3dxmtxLocal._32, m_d3dxmtxLocal._33, 0.0f) : 
		XMVectorSet(m_d3dxmtxWorld._31, m_d3dxmtxWorld._32, m_d3dxmtxWorld._33, 0.0f);
	d3dxvLookAt = XMVector3Normalize(d3dxvLookAt);
	return(d3dxvLookAt);
}

void CGameObject::CreateConstantBuffers(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbWorldMatrix);

	d3dBufferDesc.ByteWidth = sizeof(XMVECTOR) * 4;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterialColors);

	DXUT_SetDebugName(m_pd3dcbWorldMatrix, "WorldMatrix");
	DXUT_SetDebugName(m_pd3dcbMaterialColors, "MaterialColors");
}

void CGameObject::ReleaseConstantBuffers()
{
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
	if (m_pd3dcbMaterialColors) m_pd3dcbMaterialColors->Release();
}

void CGameObject::UpdateConstantBuffer_WorldMtx(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxWorld)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	XMStoreFloat4x4(&pcbWorldMatrix->m_d3dxmtxWorld, XMMatrixTranspose(*pd3dxmtxWorld));
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}

void CGameObject::UpdateConstantBuffer_Material(ID3D11DeviceContext *pd3dDeviceContext, CMaterialColors *pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterialColors, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMFLOAT4 *pcbMaterial = (XMFLOAT4 *)d3dMappedResource.pData;
	memcpy(pcbMaterial, &pMaterial->m_d3dxcAmbient, sizeof(XMFLOAT4));
	memcpy(pcbMaterial + 1, &pMaterial->m_d3dxcDiffuse, sizeof(XMFLOAT4));
	memcpy(pcbMaterial + 2, &pMaterial->m_d3dxcSpecular, sizeof(XMFLOAT4));
	memcpy(pcbMaterial + 3, &pMaterial->m_d3dxcEmissive, sizeof(XMFLOAT4));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterialColors, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_MATERIAL, 1, &m_pd3dcbMaterialColors);
}

bool CGameObject::IsVisible(CCamera *pCamera)
{
	OnPrepareRender();
	Update(NULL);

	m_bIsVisible = (m_bActive) ? true : false;
#ifdef _WITH_FRUSTUM_CULLING_BY_OBJECT
	if (m_bActive) {
		BoundingBox bcBoundingCube;
		m_bcMeshBoundingCube.Transform(bcBoundingCube, XMLoadFloat4x4(&m_d3dxmtxWorld));
		if (pCamera) m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);

		if (m_pSibling)	m_bIsVisible |= m_pSibling->IsVisible(pCamera);
		if (m_pChild) m_bIsVisible |= m_pChild->IsVisible(pCamera);
	}
#endif

	return(m_bIsVisible);
}

void CGameObject::Update(float fTimeElapsed)
{
	if (m_pSibling) m_pSibling->Update(fTimeElapsed);
	if (m_pChild) m_pChild->Update(fTimeElapsed);
}

void CGameObject::Update(float fTimeElapsed, XMMATRIX *pd3dxmtxParent)
{
	Update(fTimeElapsed);
	Animate(pd3dxmtxParent);
}

void CGameObject::Animate(XMMATRIX *pd3dxmtxParent)
{
//	m_d3dxmtxWorld = m_d3dxmtxLocal;
	if (pd3dxmtxParent) XMStoreFloat4x4(&m_d3dxmtxWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_d3dxmtxLocal), *pd3dxmtxParent));

	if (m_pSibling) m_pSibling->Animate(pd3dxmtxParent);
	if (m_pChild) m_pChild->Animate(&XMLoadFloat4x4(&m_d3dxmtxWorld));
}

void CGameObject::RenderMesh(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if (m_ppMeshes) {
		for (int i = 0; i < m_nMeshes; i++)	{
			if (m_ppMeshes[i]) {
				bool bIsVisible = true;
				#ifdef _WITH_FRUSTUM_CULLING_BY_SUBMESH
					if (pCamera) {
					#if _AABB_
						AABB bcBoundingCube = m_ppMeshes[i]->GetBoundingCube();
						bcBoundingCube.Update(&XMLoadFloat4x4(&m_d3dxmtxWorld));
						bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
					#else
						BoundingBox bcBoundingCube = m_ppMeshes[i]->GetBoundingCube();
						bcBoundingCube.Transform(bcBoundingCube, XMLoadFloat4x4(&m_d3dxmtxWorld));
						m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
					#endif
				}
				#endif
				if (bIsVisible) m_ppMeshes[i]->Render(pd3dDeviceContext);
			}
		}
	}
}

void CGameObject::OnPrepareRender()
{
}

void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender();
	Update(NULL);

	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);

	CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &XMLoadFloat4x4(&m_d3dxmtxWorld));
	if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pd3dDeviceContext);

//	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 0);
	if (m_pd3dBlendState) pd3dDeviceContext->OMSetBlendState(m_pd3dBlendState, NULL, 0xffffffff);

	RenderMesh(pd3dDeviceContext, pCamera);

	if (m_pSibling) m_pSibling->Render(pd3dDeviceContext, pCamera);
	if (m_pChild) m_pChild->Render(pd3dDeviceContext, pCamera);


	if (m_pShader) m_pShader->OnPostRender(pd3dDeviceContext);
	if (m_pd3dDepthStencilState) pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
	if (m_pd3dBlendState) pd3dDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}
