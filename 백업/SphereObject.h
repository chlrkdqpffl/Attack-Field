#pragma once
#include "Object.h"
#include "Mesh.h"

class CSphereObject : public CGameObject
{
	UINT m_nSizeType = 0;

public:
	CSphereObject();
	virtual ~CSphereObject();

	virtual void CreateObjectData(ID3D11Device *pd3dDevice) override;
	virtual void CreateMesh(ID3D11Device *pd3dDevice) override;
	virtual void CreateShader(ID3D11Device *pd3dDevice) override;

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

	// Setter //
	void SetSizeType(UINT nType) { 
		assert(nType < 3);
		m_nSizeType = nType;
	}
};