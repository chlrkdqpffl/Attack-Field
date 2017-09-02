#pragma once
#include "Object.h"
#include "BillboardObject.h"
#include "TextureToScreenRectMesh.h"
#include "SpriteImageShader.h"
#include "SpriteImageManager.h"

class CSpriteImageObject : public CBillboardObject
{
public:
	CSpriteImageObject(TextureTag tag, bool bIsInfinity = false);
	virtual ~CSpriteImageObject();

	virtual void CreateMesh(ID3D11Device *pd3dDevice) override;
	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	virtual void CreateMaterial() override;

	static void CreateConstantBuffers();
	static void UpdateConstantBuffers(int frame, SpriteInfo info);
	static void ReleaseConstantBuffers();

	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;

private:
	SpriteInfo	m_infoSprite;
	int			m_nFrame = 0;
	float		m_fTotalTime = 0.0f;
	bool		m_bIsInfinity = false;

	static ID3D11Buffer				*m_pSpriteInfoCB;
};