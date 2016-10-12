#pragma once
#include "Object.h"

class CRotatingObject : public CGameObject
{
public:
	CRotatingObject(int nMeshes = 1);
	virtual ~CRotatingObject();

private:
	XMFLOAT3					m_d3dxvRotationAxis;
	float						m_fRotationSpeed;

public:
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(XMVECTOR d3dxvRotationAxis) { XMStoreFloat3(&m_d3dxvRotationAxis, d3dxvRotationAxis); }

	virtual void Animate(float fTimeElapsed);
};