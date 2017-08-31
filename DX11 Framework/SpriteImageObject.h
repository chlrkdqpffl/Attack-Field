#pragma once
#include "Object.h"
#include "BillboardObject.h"
#include "SpriteImageMesh.h"
//#include "TextureToScreenRectMesh.h"
#include "SpriteImageShader.h"

class CSpriteImageObject : public CBillboardObject
{
public:
	CSpriteImageObject(CPlayer* player, TextureTag tag);
	virtual ~CSpriteImageObject();

	virtual void CreateMesh(ID3D11Device *pd3dDevice) override;
	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void CreateMaterial() override;

	virtual void Update(float fDeltaTime) override;

private:
	XMFLOAT4X4	m_mtxTexture;
};