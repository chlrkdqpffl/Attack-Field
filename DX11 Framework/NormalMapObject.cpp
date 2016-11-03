#include "stdafx.h"
#include "NormalMapObject.h"


CNormalMapObject::CNormalMapObject()
{
	

}


CNormalMapObject::~CNormalMapObject()
{
}


void CNormalMapObject::CreateTexture(ID3D11Device *pd3dDevice)
{
	CTexture *pNormalTexture = new CTexture(1, 1, PS_SLOT_TEXTURE_TERRAIN, PS_SLOT_SAMPLER_TERRAIN);

	ID3D11ShaderResourceView *pd3dsrvNormalTexture = NULL;
	HR(D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Assets/Image/Terrain/Base_Texture.jpg"), NULL, NULL, &pd3dsrvNormalTexture, NULL));
	pNormalTexture->SetTexture(0, pd3dsrvNormalTexture);
	pNormalTexture->SetSampler(0, STATEOBJ_MGR->m_pPointWarpSS);
	pd3dsrvNormalTexture->Release();


}
