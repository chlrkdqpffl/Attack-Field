#include "stdafx.h"
#include "Material.h"

///////////////////////////////////////////////////////////////////////////

CMaterialColors::CMaterialColors()
{
	m_d3dxcAmbient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_d3dxcDiffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_d3dxcSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 32.0f);
	m_d3dxcEmissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

CMaterialColors::~CMaterialColors()
{
}

CMaterial::CMaterial(CMaterialColors *pColors)
{
	if (pColors == nullptr) 
		m_pColors = new CMaterialColors();
	else 
		m_pColors = pColors;
}

CMaterial::~CMaterial()
{
	SafeDelete(m_pColors);
//	if(m_pTexture) m_pTexture->ReleaseShaderVariables();			// 만약에 이걸로 오류가 나면 지워도 됨, 미확실
	SafeDelete(m_pTexture);
}

void CMaterial::SetTexture(CTexture *pTexture)
{
	SafeDelete(m_pTexture);
	m_tagTexture = pTexture->GetTextureTag();
	m_pTexture = pTexture;
}

void CMaterial::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pColors) CGameObject::UpdateConstantBuffer_Material(pd3dDeviceContext, m_pColors);
	if (m_pTexture) m_pTexture->UpdateShaderVariable(pd3dDeviceContext);
}

void CMaterial::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxTexture)
{
	if (m_pTexture) m_pTexture->UpdateShaderVariable(pd3dDeviceContext, pd3dxmtxTexture);
}

///////////////////////////////////////////////////////////////////////////
//
ID3D11Buffer *CTexture::m_pd3dcbTextureMatrix = NULL;

CTexture::CTexture(int nTextures, int nSamplers, int nTextureStartSlot, int nSamplerStartSlot)
{
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

void CTexture::SetTexture(int nIndex, TextureTag textuerTag)
{
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->Release();
	m_ppd3dsrvTextures[nIndex] = RESOURCE_MGR->CloneShaderResourceView(textuerTag);
	if(m_tagTexture == TextureTag::eNone)
		m_tagTexture = textuerTag;
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
	d3dBufferDesc.ByteWidth = sizeof(XMMATRIX);		// 이거 XMFLOAT4x4안해도 되는지 확인하기
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
