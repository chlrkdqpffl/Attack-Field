#pragma once
#include "Camera.h"
#include "Player.h"

class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera(CCamera *pCamera);
	virtual ~CThirdPersonCamera() { }

	virtual void Update(float fDeltaTime) override;
};