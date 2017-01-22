#pragma once
#include "Object.h"
#include "ModelSkinnedMesh.h"

class CSkinnedObject :	public CGameObject
{
protected:
	CSkinnedMesh*		m_pSkinnedMesh;


public:
	CSkinnedObject();
	virtual ~CSkinnedObject();

	virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	void SetMesh(CSkinnedMesh* mesh) { m_pSkinnedMesh = mesh; }
};

