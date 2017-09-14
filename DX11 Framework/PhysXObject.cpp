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

void CPhysXObject::SetPosition(XMFLOAT3 pos, bool isLocal)
{
	CGameObject::SetPosition(pos, isLocal);

	PxTransform _PxTransform = m_pPxActor->getGlobalPose();

	if (m_tagPxMesh == PxMeshType::eCube) {			// Cube에 대하여 Offset 적용
		pos.x += m_vecMeshContainer[0]->GetBoundingCube().Center.x;
		pos.y += m_vecMeshContainer[0]->GetBoundingCube().Center.y;
		pos.z += m_vecMeshContainer[0]->GetBoundingCube().Center.z;
	}
	_PxTransform.p = PxVec3(pos.x, pos.y, pos.z);

	m_pPxActor->setGlobalPose(_PxTransform);
}

void CPhysXObject::SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal)
{
	CGameObject::SetRotate(fPitch, fYaw, fRoll, isLocal);

	PxTransform _PxTransform = m_pPxActor->getGlobalPose();

	_PxTransform.q *= PxQuat(XMConvertToRadians(fPitch), PxVec3(1, 0, 0));
	_PxTransform.q *= PxQuat(XMConvertToRadians(fYaw), PxVec3(0, 1, 0));
	_PxTransform.q *= PxQuat(XMConvertToRadians(fRoll), PxVec3(0, 0, 1));

	m_pPxActor->setGlobalPose(_PxTransform);
}

void CPhysXObject::SetRotate(XMFLOAT3 fAngle, bool isLocal)
{
	CGameObject::SetRotate(fAngle.x, fAngle.y, fAngle.z, isLocal);

	PxTransform _PxTransform = m_pPxActor->getGlobalPose();
	
	_PxTransform.q *= PxQuat(XMConvertToRadians(fAngle.x), PxVec3(1, 0, 0));
	_PxTransform.q *= PxQuat(XMConvertToRadians(fAngle.y), PxVec3(0, 1, 0));
	_PxTransform.q *= PxQuat(XMConvertToRadians(fAngle.z), PxVec3(0, 0, 1));

	m_pPxActor->setGlobalPose(_PxTransform);
}

void CPhysXObject::CreatePhysX_TriangleMesh(string name, PxPhysics* pPxPhysics, PxScene* pPxScene, PxMaterial *pPxMaterial, PxCooking* pCooking, XMFLOAT3 vScale)
{
	m_tagPxMesh = PxMeshType::eTriangle;
	PxTriangleMeshDesc meshDesc;

	UINT nIndexCount = m_vecMeshContainer[0]->GetIndexCount();
	UINT nVertexCount = m_vecMeshContainer[0]->GetVertexCount();
	XMFLOAT3* pVertexData = m_vecMeshContainer[0]->GetVertexData();
	UINT* pIndexData = m_vecMeshContainer[0]->GetIndexData();

	PxVec3*	pPxVertexData = new PxVec3[nVertexCount];
	PxU32* pPxIndexData = new PxU32[nIndexCount];

	for (UINT count = 0; count < nVertexCount; ++count)	{
		pPxVertexData[count].x = pVertexData[count].x;
		pPxVertexData[count].y = pVertexData[count].y;
		pPxVertexData[count].z = pVertexData[count].z;
	}
	
	for (UINT count = 0; count < nIndexCount; ++count)
		pPxIndexData[count] = pIndexData[count];

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

	PxMeshScale PxScale;
	PxScale.scale = PxVec3(vScale.x, vScale.y, vScale.z);

	PxTriangleMeshGeometry meshGeom(triangleMesh, PxScale);

	PxTransform _PxTransform(0, 0, 0);
	m_pPxActor = PxCreateStatic(*pPxPhysics, _PxTransform, meshGeom, *pPxMaterial);
	m_pPxActor->setName(name.c_str());

	pPxScene->addActor(*m_pPxActor);
}

void CPhysXObject::CreatePhysX_CubeMesh(string name, PxPhysics* pPxPhysics, PxScene* pPxScene, PxMaterial *pPxMaterial, PxCooking* pCooking, XMFLOAT3 vScale)
{
	/*
	XMFLOAT3 vMin = 
	vMin.x *= vScale.x;	 vMin.y *= vScale.y;  vMin.z *= vScale.z;

	XMFLOAT3 vMax = *(CMeshMgr::GetInstance()->Get_MeshMax(m_uiObjNum));
	vMax.x *= vScale.x;	 vMax.y *= vScale.y;  vMax.z *= vScale.z;


	XMFLOAT3 vExtents =
		XMFLOAT3((abs(vMin.x) + abs(vMax.x)) / 2, (abs(vMin.y) + abs(vMax.y)) / 2, (abs(vMin.z) + abs(vMax.z)) / 2);
		*/
	m_tagPxMesh = PxMeshType::eCube;
	XMFLOAT3 vExtents = m_vecMeshContainer[0]->GetBoundingCube().Extents;

	PxTransform _PxTransform(-m_vecMeshContainer[0]->GetBoundingCube().Center.x, -m_vecMeshContainer[0]->GetBoundingCube().Center.y, -m_vecMeshContainer[0]->GetBoundingCube().Center.z);

	PxBoxGeometry _PxBoxGeometry(vExtents.x, vExtents.y, vExtents.z);
	m_pPxActor = PxCreateStatic(*pPxPhysics, _PxTransform, _PxBoxGeometry, *pPxMaterial);
	m_pPxActor->setName(name.c_str());
	pPxScene->addActor(*m_pPxActor);
}

void CPhysXObject::CreatePhysX_ConvexMesh(string name, PxPhysics* pPxPhysics, PxScene* pPxScene, PxMaterial *pPxMaterial, PxCooking* pCooking, XMFLOAT3 vScale)
{
	m_tagPxMesh = PxMeshType::eConvex;

	PxConvexMeshDesc meshDesc;

	UINT nIndexCount = m_vecMeshContainer[0]->GetIndexCount();
	UINT nVertexCount = m_vecMeshContainer[0]->GetVertexCount();
	XMFLOAT3* pVertexData = m_vecMeshContainer[0]->GetVertexData();
	UINT* pIndexData = m_vecMeshContainer[0]->GetIndexData();

	PxVec3*	pPxVertexData = new PxVec3[nVertexCount];
	PxU32* pPxIndexData = new PxU32[nIndexCount];

	for (UINT count = 0; count < nVertexCount; ++count) {
		pPxVertexData[count].x = pVertexData[count].x;
		pPxVertexData[count].y = pVertexData[count].y;
		pPxVertexData[count].z = pVertexData[count].z;
	}

	for (UINT count = 0; count < nIndexCount; ++count)
		pPxIndexData[count] = pIndexData[count];

	meshDesc.points.count = nVertexCount;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = &(*pPxVertexData);
	meshDesc.flags = PxConvexFlags(PxConvexFlag::eCOMPUTE_CONVEX);

	PxDefaultMemoryOutputStream stream;
	bool ok = pCooking->cookConvexMesh(meshDesc, stream);

	if (!ok)
		return;

	PxDefaultMemoryInputData rb(stream.getData(), stream.getSize());
	PxConvexMesh* ConvexMesh = pPxPhysics->createConvexMesh(rb);

	PxMeshScale PxScale;
	PxScale.scale = PxVec3(vScale.x, vScale.y, vScale.z);;

	PxConvexMeshGeometry meshGeom(ConvexMesh, PxScale);

	PxTransform _PxTransform(0, 0, 0);
	m_pPxActor = PxCreateStatic(*pPxPhysics, _PxTransform, meshGeom, *pPxMaterial);
	m_pPxActor->setName(name.c_str());

	pPxScene->addActor(*m_pPxActor);
}