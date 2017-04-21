#pragma once
#include "Shader.h"

class CGBufferShader : public CShader
{
	GBufferType m_nType = GBufferType::eNone;
public:
	CGBufferShader(GBufferType type);
	virtual ~CGBufferShader();

	void CreateShader(ID3D11Device *pd3dDevice) override;
};