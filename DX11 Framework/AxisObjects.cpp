#include "stdafx.h"
#include "AxisObjects.h"

CAxisObjects::CAxisObjects()
{
}

CAxisObjects::~CAxisObjects()
{
	if (m_pPlayerWorldAxis) {
		m_pPlayerWorldAxis->Release();
		delete m_pPlayerWorldAxis;
	}
	if (m_pWorldCenterAxis) {
		m_pWorldCenterAxis->Release();
		delete m_pWorldCenterAxis;
	}
}

void CAxisObjects::CreateAxisObjects(ID3D11Device *pd3dDevice)
{
	// World Axis
	CMesh* pWorldXAxisMesh = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(15, 0, 0), XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));
	CMesh* pWorldYAxisMesh = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 15, 0), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
	CMesh* pWorldZAxisMesh = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 15),XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f));

	m_pPlayerWorldAxis = new CGameObject;
	m_pPlayerWorldAxis->SetMesh(pWorldXAxisMesh, 0);
	m_pPlayerWorldAxis->SetMesh(pWorldYAxisMesh, 1);
	m_pPlayerWorldAxis->SetMesh(pWorldZAxisMesh, 2);

	/*
	// Local Axis
	CMesh* pLocalXAxisMesh = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(10, 0, 0), XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));
	CMesh* pLocalYAxisMesh = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 10, 0), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
	CMesh* pLocalZAxisMesh = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 10), XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f));

	m_pPlayerLocalAxis = new CGameObject;
	m_pPlayerLocalAxis->SetMesh(pLocalXAxisMesh, 0);
	m_pPlayerLocalAxis->SetMesh(pLocalYAxisMesh, 1);
	m_pPlayerLocalAxis->SetMesh(pLocalZAxisMesh, 2);
	*/

	// World Center Axis
	CMesh* pWorldCenterX = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(1000, 0, 0), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	CMesh* pWorldCenterY = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1000, 0), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	CMesh* pWorldCenterZ = new CLineMesh(pd3dDevice, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 1000), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	m_pWorldCenterAxis = new CGameObject;
	m_pWorldCenterAxis->SetMesh(pWorldCenterX, 0);
	m_pWorldCenterAxis->SetMesh(pWorldCenterY, 1);
	m_pWorldCenterAxis->SetMesh(pWorldCenterZ, 2);


	m_pShader = new CShader();
	m_pShader->CreateShader(pd3dDevice, VERTEX_POSITION_ELEMENT | VERTEX_COLOR_ELEMENT);
}

void CAxisObjects::Update(float fTimeElapsed)
{
	if (m_pPlayerWorldAxis)	m_pPlayerWorldAxis->SetPosition(SCENE_MGR->m_pPlayer->GetPosition());
}

void CAxisObjects::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);

	if (m_pWorldCenterAxis) {
		// Position ( 0, 0, 0 ) °íÁ¤
		m_pWorldCenterAxis->RenderMesh(pd3dDeviceContext, pCamera);
	}

	if (m_pPlayerWorldAxis) {
		m_pPlayerWorldAxis->UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &XMLoadFloat4x4(&m_pPlayerWorldAxis->m_d3dxmtxWorld));
		m_pPlayerWorldAxis->RenderMesh(pd3dDeviceContext, pCamera);
	}
}