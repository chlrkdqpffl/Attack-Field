#pragma once
#include "Object.h"

class CBulletObject : public CGameObject
{
	float			m_fSpeed = 0.0f;

public:
	CBulletObject();
	virtual ~CBulletObject();

	virtual void CreateMesh(ID3D11Device *pd3dDevice);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateMaterial();

	virtual void Update(float fTimeElapsed) override;

	// ----- Get, Setter ----- //
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }

	float GetSpeed() const { return m_fSpeed; }
};

