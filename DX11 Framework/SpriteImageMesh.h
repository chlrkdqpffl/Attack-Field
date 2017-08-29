#pragma once
#include "Mesh.h"

class CSpriteImageMesh : public CMeshTextured
{
public:
	CSpriteImageMesh(ID3D11Device *pd3dDevice, TextureTag tag, int frame, POINT perSize, float lifeTime, float totalTime);
	virtual ~CSpriteImageMesh();

	void Update(const float& fDeltaTime);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext)	override;

private:
	TextureTag	m_TagTexture	= TextureTag::eNone;
	UINT		m_nFrame		= 0;
	POINT		m_nPerSize		= POINT{ 0,0 };

	float		m_fLifeTime		= 0.0f;
	float		m_fTotalTime	= 0.0f;
};