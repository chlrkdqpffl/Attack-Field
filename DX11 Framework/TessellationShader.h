#pragma once
#include "Shader.h"

class CTessellationShader :	public CShader
{
protected:
	ID3D11HullShader				*m_pd3dHullShader	= nullptr;
	ID3D11DomainShader				*m_pd3dDomainShader	= nullptr;

public:
	CTessellationShader();
	virtual ~CTessellationShader();

	void CreateHullShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11HullShader **ppd3dHullShader);
	void CreateDomainShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11DomainShader **ppd3dDomainShader);
};

