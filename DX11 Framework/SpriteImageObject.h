#pragma once
#include "Object.h"
#include "SpriteImageShader.h"
#include "SpriteImageMesh.h"

class CSpriteImageObject : public CGameObject
{
public:
	CSpriteImageObject(TextureTag tag);
	virtual ~CSpriteImageObject();

	virtual void CreateMesh(ID3D11Device *pd3dDevice) override;
	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void CreateMaterial() override;

	virtual void Update(float fDeltaTime) override;
};