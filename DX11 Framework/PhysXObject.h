#pragma once
#include "Object.h"

class CPhysXObject : public CGameObject
{
public:
	CPhysXObject();
	virtual ~CPhysXObject();

	virtual void CreatePhysXDaga(string name, PxPhysics* pPxPhysics, PxScene* pPxScene, PxMaterial *pPxMaterial, PxCooking* pCooking, XMFLOAT3 vScale = XMFLOAT3(1.0f, 1.0f, 1.0f));

	virtual void SetPosition(float x, float y, float z, bool isLocal = false) override;
	virtual void SetPosition(XMVECTOR d3dxvPosition, bool isLocal = false) override;
	virtual void SetPosition(XMFLOAT3 d3dxvPosition, bool isLocal = false) override;
	virtual void SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal = false) override;
	virtual void SetRotate(XMFLOAT3 fAngle, bool isLocal = false) override;
	virtual void SetRotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal = false) override;

protected:
	PxRigidStatic*  m_pPxActor = nullptr;
};

