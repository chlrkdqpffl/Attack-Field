#pragma once
#include "Object.h"
#include "LineMesh.h"

class CLineObject : public CGameObject
{
public:
	CLineObject(XMVECTOR pos1, XMVECTOR pos2, DWORD lifeTime, XMVECTOR color = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
	CLineObject(XMFLOAT3 pos1, XMFLOAT3 pos2, DWORD lifeTime, XMVECTOR color = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
	virtual ~CLineObject();

	virtual void CreateObjectData(ID3D11Device *pd3dDevice) override;
	virtual void CreateMesh(ID3D11Device *pd3dDevice) override;
	virtual void CreateShader(ID3D11Device *pd3dDevice) override;

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;
private:
	DWORD				m_dwLifeTime = 0.0f;
	DWORD				m_dwAgeTime = 0.0f;
	CLineMesh*			m_pLineMesh = nullptr;

	XMVECTOR			m_Color;
	XMFLOAT3			m_StartPosition;
	XMFLOAT3			m_EndPosition;
};

