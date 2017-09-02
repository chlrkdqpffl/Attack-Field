#pragma once
#include "Object.h"

class CBillboardObject : public CGameObject
{

public:
	CBillboardObject();
	virtual ~CBillboardObject();

	void SetLookAt(XMFLOAT3 cameraPos);

	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;
};