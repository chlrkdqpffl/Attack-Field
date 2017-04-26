#pragma once
#include "Camera.h"
#include "Player.h"

class CFirstPersonCamera : public CCamera
{
public:
	CFirstPersonCamera(CCamera *pCamera);
	virtual ~CFirstPersonCamera() { }
	
	virtual void Update(float fTimeElapsed) override;
};