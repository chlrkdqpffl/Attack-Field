#pragma once
#include "Object.h"
#include "BillboardObject.h"
#include "TextureToScreenRectMesh.h"
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
	int			m_nFrame = 0;
	int			m_nSizeX = 0;
	int			m_nSizeY = 0;
	float		m_fLifeTime = 0.0f;
	float		m_fTotalTime = 0.0f;
};