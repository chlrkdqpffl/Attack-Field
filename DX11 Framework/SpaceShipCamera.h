#pragma once
#include "Camera.h"
#include "Player.h"

class CSpaceShipCamera : public CCamera
{
public:
	CSpaceShipCamera(CCamera *pCamera);
	virtual ~CSpaceShipCamera();

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) override;
	virtual void Update(float fDeltaTime) override;
};