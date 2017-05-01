#pragma once
#include "Object.h"

class CLineObject : CGameObject
{
public:
	CLineObject(XMFLOAT3 pos1, XMFLOAT3 pos2, DWORD lifeTime);
	virtual ~CLineObject();

	virtual void CreateObjectData(ID3D11Device *pd3dDevice) override;
	virtual void CreateMesh(ID3D11Device *pd3dDevice) override;
	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
private:
	DWORD				m_dwLifeTime = 0.0f;
	DWORD				m_dwAgeTime = 0.0f;
	CLineMesh*			m_pLineMesh;

	XMFLOAT3			m_StartPosition;
	XMFLOAT3			m_EndPosition;
};

