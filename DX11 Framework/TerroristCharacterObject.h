#pragma once
#include "CharacterObject.h" 
#include "RifleGunWeapon.h"
#include "BoundingBoxMesh.h"

class CTerroristCharacterObject : public CCharacterObject
{
public:
	CTerroristCharacterObject(TeamType team);
	virtual ~CTerroristCharacterObject();

	virtual void CreateMaterial() override;
	virtual void CreateMesh(ID3D11Device *pd3dDevice) override;
	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void CreateAnimation() override;
	virtual void CreateWeapon(ID3D11Device *pd3dDevice) override;
	virtual void CreateBoundingBox(ID3D11Device *pd3dDevice) override;
};