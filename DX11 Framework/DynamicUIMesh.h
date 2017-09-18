#pragma once
#include "Mesh.h"

class CDynamicUIMesh : public CMeshTextured
{
	POINT m_ptStartPos = POINT{ 0,0 };
	POINT m_ptEndPos = POINT{ 0,0 };
	float m_fZPos = 0.0f;

public:
	CDynamicUIMesh(ID3D11Device *pd3dDevice, POINT startPos, POINT endPos, float zPos);
	virtual ~CDynamicUIMesh();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

	void SetStartPos(POINT pos) { m_ptStartPos = pos; }
	void SetEndPos(POINT pos) { m_ptEndPos = pos; }
};