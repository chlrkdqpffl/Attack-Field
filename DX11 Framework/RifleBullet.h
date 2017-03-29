#pragma once
#include "BulletObject.h"

class CRifleBullet : public CBulletObject
{
public:
	CRifleBullet(float fRange, float fSpeed);
	virtual ~CRifleBullet();

	virtual void CreateMesh(ID3D11Device *pd3dDevice);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateMaterial();

	virtual void Update(float fTimeElapsed) override;
};