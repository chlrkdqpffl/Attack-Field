#pragma once
#include "Object.h"
#include "WaterGridMesh.h"

class CTerrainWater : public CGameObject
{
public:
	CTerrainWater(ID3D11Device *pd3dDevice, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMVECTOR d3dxvScale);
	virtual ~CTerrainWater();

private:
	int							m_nWidth;
	int							m_nLength;

	XMFLOAT3					m_d3dxvScale;

	CWaterGridMesh				*m_pWaterGridMesh;
	XMFLOAT4X4					m_d3dxmtxTexture;
public:

	virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
};