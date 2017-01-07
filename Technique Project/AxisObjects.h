#pragma once
#include "Object.h"
#include "LineMesh.h"

class CAxisObjects : public CGameObject
{
	CGameObject*	m_pPlayerWorldAxis		= nullptr;
//	CGameObject*	m_pPlayerLocalAxis		= nullptr;
	CGameObject*	m_pWorldCenterAxis		= nullptr;

public:
	CAxisObjects();
	virtual ~CAxisObjects();

	void CreateAxisObjects(ID3D11Device *pd3dDevice);

	virtual void Update(float fTimeElapsed) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;
};

