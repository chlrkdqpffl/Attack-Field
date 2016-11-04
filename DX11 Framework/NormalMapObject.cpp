#include "stdafx.h"
#include "NormalMapObject.h"


CNormalMapObject::CNormalMapObject()
{
	

}


CNormalMapObject::~CNormalMapObject()
{
}


void CNormalMapObject::CreateMaterial(ID3D11Device *pd3dDevice)
{
	CMaterial *pMaterial = new CMaterial();

	CTexture *pNormalTexture = new CTexture(2, 1, PS_SLOT_TEXTURE_TERRAIN, PS_SLOT_SAMPLER_TERRAIN);

	ID3D11ShaderResourceView *pd3dsrvDiffuseTexture = NULL;
	ID3D11ShaderResourceView *pd3dsrvNormalTexture = NULL;

	HR(D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Terrain/Base_Texture.jpg"), NULL, NULL, &pd3dsrvDiffuseTexture, NULL));
	pNormalTexture->SetTexture(0, pd3dsrvDiffuseTexture);
	pd3dsrvDiffuseTexture->Release();

	HR(D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Terrain/Base_Texture.jpg"), NULL, NULL, &pd3dsrvNormalTexture, NULL));
	pNormalTexture->SetTexture(1, pd3dsrvNormalTexture);

	pNormalTexture->SetSampler(0, STATEOBJ_MGR->m_pPointWarpSS);
	pd3dsrvNormalTexture->Release();

	pMaterial->SetTexture(pNormalTexture);
}
