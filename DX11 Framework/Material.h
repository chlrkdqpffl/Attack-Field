#pragma once

class CMaterialColors
{
public:
	CMaterialColors();
	virtual ~CMaterialColors();

public:
	XMFLOAT4						m_d3dxcAmbient;
	XMFLOAT4						m_d3dxcDiffuse;
	XMFLOAT4						m_d3dxcSpecular; //(r,g,b,a=power)
	XMFLOAT4						m_d3dxcEmissive;
};

class CTexture
{
public:
	CTexture(int nTextures = 1, int nSamplers = 1, int nTextureStartSlot = 0, int nSamplerStartSlot = 0);
	virtual ~CTexture();

private:
	TextureTag						m_tagTexture = TextureTag::eNone;
	XMFLOAT2						m_f2offset = XMFLOAT2(1.0f, 1.0f);

	int								m_nTextures = 0;
	ID3D11ShaderResourceView		**m_ppd3dsrvTextures = nullptr;
	int								m_nTextureStartSlot = 0;

	int								m_nSamplers = 0;
	ID3D11SamplerState				**m_ppd3dSamplerStates = nullptr;
	int								m_nSamplerStartSlot = 0;

public:

	void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext);

	void SetOffset(XMFLOAT2 offset) { m_f2offset = offset; }
	void SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture);
	void SetTexture(int nIndex, TextureTag);
	void SetSampler(int nIndex, ID3D11SamplerState *pd3dSamplerState);
	TextureTag GetTextureTag() const { return m_tagTexture; }

	//static ID3D11Buffer				*m_pd3dcbTextureMatrix;
	//static void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxTexture);
	static ID3D11Buffer				*m_pd3dcbTextureOffset;
	static XMFLOAT2					prevOffset;
	static void CreateShaderVariables(ID3D11Device *pd3dDevice);
	static void ReleaseShaderVariables();
	static void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, XMFLOAT2* textureOffset);
	static ID3D11ShaderResourceView *CreateTexture2DArraySRV(ID3D11Device *pd3dDevice, _TCHAR(*ppstrFilePaths)[128], UINT nTextures);
};

class CMaterial
{
public:
	CMaterial(CMaterialColors *pColors = nullptr);
	virtual ~CMaterial();

private:

	TextureTag						m_tagTexture = TextureTag::eNone;
	CMaterialColors					*m_pColors = nullptr;
	CTexture						*m_pTexture = nullptr;

public:
	void SetTexture(CTexture *pTexture);
	void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext);
	//void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxTexture);
	
	CTexture* GetTexture() const { return m_pTexture; }
	TextureTag GetTextureTag() const { return m_tagTexture; }
};
