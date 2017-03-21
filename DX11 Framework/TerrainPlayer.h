#pragma once
#include "Player.h"
#include "ThirdPersonCamera.h"
#include "HeightMapTerrain.h"

class CTerrainPlayer : public CPlayer
{
public:
	CTerrainPlayer(CCharacterObject* pCharacter);
	virtual ~CTerrainPlayer();

	virtual void ChangeCamera(ID3D11Device *pd3dDevice, CameraTag cameraTag, float fTimeElapsed);

	virtual void OnPlayerUpdated(float fTimeElapsed);
	virtual void OnCameraUpdated(float fTimeElapsed);
};