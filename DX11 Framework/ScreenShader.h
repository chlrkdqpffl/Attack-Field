#pragma once
#include "Shader.h"

class CScreenShader : public CShader
{
public:
	CScreenShader();
	virtual ~CScreenShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;
};