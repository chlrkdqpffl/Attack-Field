#pragma once
#include "Shader.h"
#include "TextureToScreenRectMesh.h"

class CScreenShader : public CShader
{
	CTextureToScreenRectMesh	*m_pScreenMesh = nullptr;
	CTexture					*m_pTexture = nullptr;

public:
	CScreenShader();
	virtual ~CScreenShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	void CreateMesh(ID3D11Device *pd3dDevice, ID3D11ShaderResourceView *pd3dsrvTexture);
	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = nullptr) override;
};