#pragma once
#include "Object.h"
#include "Shader.h"

class CSkyBox : public CGameObject
{
public:
	CSkyBox(ID3D11Device *pd3dDevice);
	virtual ~CSkyBox();

#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
	ID3D11Buffer				*m_pd3dcbTextureIndex;
#endif

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
};
