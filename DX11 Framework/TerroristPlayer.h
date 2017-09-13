#pragma once
#include "CharacterPlayer.h"

class CTerroristPlayer : public CCharacterPlayer
{
public:
	CTerroristPlayer(TeamType team);
	virtual ~CTerroristPlayer();

	virtual void CreateMaterial() override;
	virtual void CreateMesh(ID3D11Device *pd3dDevice) override;
	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void CreateAnimation() override;
	virtual void CreateWeapon(ID3D11Device *pd3dDevice) override;
	virtual void CreateBoundingBox(ID3D11Device *pd3dDevice) override;
};

