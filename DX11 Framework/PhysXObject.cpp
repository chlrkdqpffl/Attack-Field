#include "stdafx.h"
#include "PhysXObject.h"


CPhysXObject::CPhysXObject()
{
}

CPhysXObject::~CPhysXObject()
{
	if (m_pPxActor)
		m_pPxActor->release();
}

void CPhysXObject::SetPosition(float x, float y, float z, bool isLocal)
{
	CGameObject::SetPosition(x, y, z, isLocal);

	PxTransform _PxTransform = m_pPxActor->getGlobalPose();
	_PxTransform.p = PxVec3(x, y, z);
	
	m_pPxActor->setGlobalPose(_PxTransform);
}

void CPhysXObject::SetPosition(XMVECTOR vPosition, bool isLocal)
{
	CGameObject::SetPosition(vPosition, isLocal);

	XMFLOAT3 pos; XMStoreFloat3(&pos, vPosition);

	PxTransform _PxTransform = m_pPxActor->getGlobalPose();
	_PxTransform.p = PxVec3(pos.x, pos.y, pos.z);

	m_pPxActor->setGlobalPose(_PxTransform);
}

void CPhysXObject::SetPosition(XMFLOAT3 pos, bool isLocal)
{
	CGameObject::SetPosition(pos, isLocal);

	PxTransform _PxTransform = m_pPxActor->getGlobalPose();
	_PxTransform.p = PxVec3(pos.x, pos.y, pos.z);

	m_pPxActor->setGlobalPose(_PxTransform);
}

void CPhysXObject::SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal)
{
	CGameObject::SetRotate(fPitch, fYaw, fRoll, isLocal);

	PxTransform _PxTransform = m_pPxActor->getGlobalPose();

	_PxTransform.q *= PxQuat(fPitch, PxVec3(1, 0, 0));
	_PxTransform.q *= PxQuat(fRoll, PxVec3(0, 1, 0));
	_PxTransform.q *= PxQuat(fYaw, PxVec3(0, 0, 1));

	m_pPxActor->setGlobalPose(_PxTransform);
}

void CPhysXObject::SetRotate(XMFLOAT3 fAngle, bool isLocal)
{
	CGameObject::SetRotate(fAngle.x, fAngle.y, fAngle.z, isLocal);

	PxTransform _PxTransform = m_pPxActor->getGlobalPose();

	_PxTransform.q *= PxQuat(fAngle.x, PxVec3(1, 0, 0));
	_PxTransform.q *= PxQuat(fAngle.z, PxVec3(0, 1, 0));
	_PxTransform.q *= PxQuat(fAngle.y, PxVec3(0, 0, 1));

	m_pPxActor->setGlobalPose(_PxTransform);
}

void CPhysXObject::SetRotate(XMVECTOR *vAxis, float fAngle, bool isLocal)
{
	CGameObject::SetRotate(vAxis, isLocal);

	XMFLOAT3 rotate; XMStoreFloat3(&rotate, *vAxis);
	PxTransform _PxTransform = m_pPxActor->getGlobalPose();

	_PxTransform.q *= PxQuat(rotate.x, PxVec3(1, 0, 0));
	_PxTransform.q *= PxQuat(rotate.z, PxVec3(0, 1, 0));
	_PxTransform.q *= PxQuat(rotate.y, PxVec3(0, 0, 1));

	m_pPxActor->setGlobalPose(_PxTransform);
}

void CPhysXObject::CreatePhysXDaga(string name, PxPhysics* pPxPhysics, PxScene* pPxScene, PxMaterial *pPxMaterial, PxCooking* pCooking, XMFLOAT3 vScale)
{
	PxTriangleMeshDesc meshDesc;

	UINT nIndexCount = m_vecMeshContainer[0]->GetIndexCount();
	UINT nVertexCount = m_vecMeshContainer[0]->GetVertexCount();
	XMFLOAT3* pVertexData = m_vecMeshContainer[0]->GetVertexData();
	UINT* pIndexData = m_vecMeshContainer[0]->GetIndexData();

	PxVec3*	pPxVertexData = new PxVec3[nVertexCount];
	PxU32* pPxIndexData = new PxU32[nIndexCount];

	for (UINT count = 0; count < nVertexCount; ++count)
	{
		pPxVertexData[count].x = pVertexData[count].x;
		pPxVertexData[count].y = pVertexData[count].y;
		pPxVertexData[count].z = pVertexData[count].z;
	}
	
	for (UINT count = 0; count < nIndexCount; ++count)
	{
		pPxIndexData[count] = pIndexData[count];
	}


	meshDesc.points.count = nVertexCount;
	meshDesc.triangles.count = nIndexCount / 3;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.triangles.stride = sizeof(PxU32) * 3;
	meshDesc.points.data = &(*pPxVertexData);
	meshDesc.triangles.data = &(*pPxIndexData);
	meshDesc.flags = PxMeshFlags(0);

	PxDefaultMemoryOutputStream stream;
	bool ok = pCooking->cookTriangleMesh(meshDesc, stream);

	if (!ok)
		return;

	PxDefaultMemoryInputData rb(stream.getData(), stream.getSize());
	PxTriangleMesh* triangleMesh = pPxPhysics->createTriangleMesh(rb);

	// Scale
	PxMeshScale PxScale;
	PxScale.scale = PxVec3(vScale.x, vScale.y, vScale.z);

	PxTriangleMeshGeometry meshGeom(triangleMesh, PxScale);

	PxTransform _PxTransform(0, 0, 0);
	m_pPxActor = PxCreateStatic(*pPxPhysics, _PxTransform, meshGeom, *pPxMaterial);
	m_pPxActor->setName(name.c_str());

	pPxScene->addActor(*m_pPxActor);
}