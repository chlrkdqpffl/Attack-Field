#pragma once
#include "Player.h"
#include "ThirdPersonCamera.h"
#include "HeightMapTerrain.h"

class CTerrainPlayer : public CPlayer
{
public:
	CTerrainPlayer(CCharacterObject* pCharacter = nullptr);
	virtual ~CTerrainPlayer();

	virtual void ChangeCamera(ID3D11Device *pd3dDevice, CameraTag cameraTag);

//	virtual void OnApplyGravity(float fDeltaTime) override;
	virtual void OnCameraUpdated(float fDeltaTime) override;
};