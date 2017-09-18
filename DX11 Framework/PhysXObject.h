#pragma once
#include "Object.h"

class CPhysXObject : public CGameObject
{
	enum class PxMeshType 
	{ 
		eNone,
		eTriangle, eCube, eConvex
	};

public:
	CPhysXObject();
	virtual ~CPhysXObject();

	virtual void CreatePhysX_TriangleMesh(string name, PxPhysics* pPxPhysics, PxScene* pPxScene, PxMaterial *pPxMaterial, PxCooking* pCooking, XMFLOAT3 vScale = XMFLOAT3(1.0f, 1.0f, 1.0f));
	virtual void CreatePhysX_CubeMesh(string name, PxPhysics* pPxPhysics, PxScene* pPxScene, PxMaterial *pPxMaterial, PxCooking* pCooking, XMFLOAT3 vScale = XMFLOAT3(1.0f, 1.0f, 1.0f));
	virtual void CreatePhysX_ConvexMesh(string name, PxPhysics* pPxPhysics, PxScene* pPxScene, PxMaterial *pPxMaterial, PxCooking* pCooking, XMFLOAT3 vScale = XMFLOAT3(1.0f, 1.0f, 1.0f));
	virtual void SetPosition(XMFLOAT3 d3dxvPosition, bool isLocal = false) override;
	virtual void SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal = false) override;
	virtual void SetRotate(XMFLOAT3 fAngle, bool isLocal = false) override;
	
protected:
	PxRigidStatic*  m_pPxActor = nullptr;
	PxMeshType		m_tagPxMesh = PxMeshType::eNone;

	PxVec3*	m_pPxVertexData = nullptr;
	PxU32* m_pPxIndexData = nullptr;
};

