#pragma once
#include "Player.h"
#include "HeightMapTerrain.h"


class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer(CCharacterObject* pCharacter);
	virtual ~CAirplanePlayer();

	virtual void OnPrepareRender();

	virtual void OnPlayerUpdated(float fTimeElapsed);
	virtual void ChangeCamera(ID3D11Device *pd3dDevice, CameraTag nNewCameraTag, float fTimeElapsed);
};
