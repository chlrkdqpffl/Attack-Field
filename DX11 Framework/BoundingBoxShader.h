#pragma once
#include "Shader.h"

class CBoundingBoxShader : public CShader
{
protected:
	ID3D11PixelShader				*m_pd3dCollisionPixelShader = nullptr;

public:
	CBoundingBoxShader();
	virtual ~CBoundingBoxShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	void OnPrepareSetting(ID3D11DeviceContext *pd3dDeviceContext, bool isCollision);
};