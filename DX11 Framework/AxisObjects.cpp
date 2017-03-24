#include "stdafx.h"
#include "AxisObjects.h"

CAxisObjects::CAxisObjects(CGameObject* pOwner) : m_pOwnerObject(pOwner)
{
}

CAxisObjects::~CAxisObjects()
{
	SafeDelete(m_pWorldAxisObject);
}

void CAxisObjects::CreateAxis(ID3D11Device *pd3dDevice)
{
	if (m_pOwnerObject) {
		// Owner
		XMFLOAT3 size = m_pOwnerObject->GetMesh()->GetBoundingCube().Extents;

		CMesh* pOwnerWorldXAxisMesh = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(size.x + 15, 0, 0), XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));
		CMesh* pOwnerWorldYAxisMesh = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, size.y + 15, 0), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		CMesh* pOwnerWorldZAxisMesh = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, size.z + 15), XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f));

		m_pWorldAxisObject = new CGameObject;
		m_pWorldAxisObject->SetMesh(pOwnerWorldXAxisMesh, 0);
		m_pWorldAxisObject->SetMesh(pOwnerWorldYAxisMesh, 1);
		m_pWorldAxisObject->SetMesh(pOwnerWorldZAxisMesh, 2);
	}
	else {
		// World Center Axis
		CMesh* pWorldCenterX = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(1000, 0, 0), XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));
		CMesh* pWorldCenterY = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1000, 0), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		CMesh* pWorldCenterZ = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 1000), XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f));

		m_pWorldAxisObject = new CGameObject;
		m_pWorldAxisObject->SetMesh(pWorldCenterX, 0);
		m_pWorldAxisObject->SetMesh(pWorldCenterY, 1);
		m_pWorldAxisObject->SetMesh(pWorldCenterZ, 2);
	}
	m_pShader = new CShader();
	m_pShader->CreateShader(pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_COLOR_ELEMENT);
}

void CAxisObjects::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_pShader->Render(pd3dDeviceContext, pCamera);

	if (m_pOwnerObject)
		m_pWorldAxisObject->UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &XMLoadFloat4x4(&m_pOwnerObject->m_d3dxmtxWorld));
	else
		m_pWorldAxisObject->UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &XMMatrixIdentity());

	m_pWorldAxisObject->RenderMesh(pd3dDeviceContext, pCamera);
}