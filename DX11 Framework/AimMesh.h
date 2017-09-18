#pragma once
#include "Mesh.h"

class CAimMesh : public CMeshDiffused
{
	POINT	m_ptStartPos = POINT{ 0,0 };
	POINT	m_ptEndPos = POINT{ 0,0 };
	UINT	m_nWitdh = 0;

public:
	CAimMesh(ID3D11Device *pd3dDevice, POINT startPos, POINT endPos, UINT width, XMFLOAT4 color);
	virtual ~CAimMesh();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext) override;

	void SetStartPos(POINT pos) { m_ptStartPos = pos; }
	void SetEndPos(POINT pos) { m_ptEndPos = pos; }
};