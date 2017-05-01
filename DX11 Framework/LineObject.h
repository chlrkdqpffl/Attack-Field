#pragma once
#include "Object.h"

class CLineObject : CGameObject
{
public:
	CLineObject();
	virtual ~CLineObject();

private:
	DWORD				m_fLifeTime = 0.0f;
	CLineMesh*			m_pLineMesh;

};

