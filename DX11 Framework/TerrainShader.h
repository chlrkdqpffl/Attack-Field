#pragma once
#include "TessellationShader.h"

class CTerrainShader :	public CTessellationShader
{
public:
	CTerrainShader();
	virtual ~CTerrainShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;
};