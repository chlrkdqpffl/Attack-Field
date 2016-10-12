#pragma once
#include "Player.h"
#include "HeightMapTerrain.h"

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer(int nMeshes = 1);
	virtual ~CAirplanePlayer();

	virtual void OnPrepareRender();

	virtual void OnPlayerUpdated(float fTimeElapsed);
	virtual void ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);
};
