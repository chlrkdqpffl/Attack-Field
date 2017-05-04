#pragma once
#include "Mesh.h"

class CWaterGridMesh : public CHeightMapGridMesh
{
private:
	int							m_xStart;
	int							m_zStart;

	float						m_fK1;
	float						m_fK2;
	float						m_fK3;

	float						m_fTimeDelta;
	float						m_fSpatialDelta;
	float						m_fAccumulatedTime;

	XMFLOAT3					*m_pd3dxvPreviousPositions;
	XMFLOAT3					*m_pd3dxvCurrentPositions;
	XMFLOAT3					*m_pd3dxvNormalVectors;

public:
	CWaterGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, XMVECTOR d3dxvScale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), void *pContext = NULL, D3D11_USAGE d3dUsage = D3D11_USAGE_DEFAULT);
	virtual ~CWaterGridMesh();

	void SetDisturbingForce(int i, int j, float fMagnitude);
	void OnPrepareAnimation(float fTimeDelta, float fSpatialDelta, float fSpeed, float fDamping);

	virtual void Update(float fDeltaTime);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};