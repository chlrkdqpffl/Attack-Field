#pragma once
#include "Shader.h"

class CNormalDisplaceMapShader : public CShader
{
public:
	CNormalDisplaceMapShader() {};
	virtual ~CNormalDisplaceMapShader() {};

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext) override;
	virtual void OnPostRender(ID3D11DeviceContext *pd3dDeviceContext) override;
};

class CNormalDisplaceMapInstancedShader : public CShader
{
public:
	CNormalDisplaceMapInstancedShader() {};
	virtual ~CNormalDisplaceMapInstancedShader() {};

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext) override;
	virtual void OnPostRender(ID3D11DeviceContext *pd3dDeviceContext) override;
};