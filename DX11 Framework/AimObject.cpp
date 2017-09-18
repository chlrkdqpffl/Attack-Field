#include "stdafx.h"
#include "AimObject.h"


CAimObject::CAimObject(UINT AimLength, XMFLOAT4 color)
	: m_nAimLength(AimLength), m_Color(color)
{
}

CAimObject::~CAimObject()
{
	SafeDelete(m_pLeftLine);
	SafeDelete(m_pRightLine);
	SafeDelete(m_pTopLine);
	SafeDelete(m_pBottomLine);
	SafeDelete(m_pCenterPoint);

	SafeDelete(m_pShader);
}

void CAimObject::Initialize(ID3D11Device* pDevice, POINT aimPosition, UINT width)
{
	m_ptAimPos = aimPosition;
	POINT offset = aimPosition;

	offset.x -= m_nAimLength;
	m_pLeftLine = new CAimMesh(pDevice, offset, aimPosition, width, m_Color);

	offset = aimPosition;
	offset.x += m_nAimLength;
	m_pRightLine = new CAimMesh(pDevice, aimPosition, offset, width, m_Color);

	offset = aimPosition;
	offset.y -= m_nAimLength;
	m_pTopLine = new CAimMesh(pDevice, offset, aimPosition, width, m_Color);
	
	offset = aimPosition;
	offset.y += m_nAimLength;
	m_pBottomLine = new CAimMesh(pDevice, aimPosition, offset, width, m_Color);

	m_pCenterPoint = new CAimMesh(pDevice, aimPosition, aimPosition, width, m_Color);

	// Shader
	m_pShader = new C2DLineShader();
	m_pShader->CreateShader(pDevice);
}

void CAimObject::SetAimSize(UINT size)
{
	m_nAimSize = size;
	POINT offset = m_ptAimPos;

	offset.x -= m_nAimSize;
	m_pLeftLine->SetEndPos(offset);
	offset.x -= m_nAimLength;
	m_pLeftLine->SetStartPos(offset);

	offset = m_ptAimPos;
	offset.x += m_nAimSize;
	m_pRightLine->SetStartPos(offset);
	offset.x += m_nAimLength;
	m_pRightLine->SetEndPos(offset);

	offset = m_ptAimPos;
	offset.y -= m_nAimSize;
	m_pTopLine->SetEndPos(offset);
	offset.y -= m_nAimLength;
	m_pTopLine->SetStartPos(offset);

	offset = m_ptAimPos;
	offset.y += m_nAimSize;
	m_pBottomLine->SetStartPos(offset);
	offset.y += m_nAimLength;
	m_pBottomLine->SetEndPos(offset);
}

void CAimObject::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (false == m_bIsActive)
		return;

	m_pShader->OnPrepareRender(pDeviceContext);

	m_pLeftLine->Render(pDeviceContext);
	m_pRightLine->Render(pDeviceContext);
	m_pTopLine->Render(pDeviceContext);
	m_pBottomLine->Render(pDeviceContext);
	m_pCenterPoint->Render(pDeviceContext);
}
