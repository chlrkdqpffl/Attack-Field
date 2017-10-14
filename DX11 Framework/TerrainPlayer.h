#pragma once
#include "Player.h"
#include "ThirdPersonCamera.h"

class CTerrainPlayer : public CPlayer
{
public:
	CTerrainPlayer(CCharacterPlayer* pCharacter = nullptr);
	virtual ~CTerrainPlayer();

	virtual void ChangeCamera(ID3D11Device *pd3dDevice, CameraTag cameraTag);

//	virtual void OnApplyGravity(float fDeltaTime) override;
	virtual void OnCameraUpdated(float fDeltaTime) override;
};