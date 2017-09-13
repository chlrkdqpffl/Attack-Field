#pragma once

class CSSReflection
{
public:

	CSSReflection();
	~CSSReflection();

	void Initialize(ID3D11Device* pDevice, UINT width, UINT height);
	void Deinit();

	void SetParameters(float fViewAngleThreshold, float fEdgeDistThreshold, float fDepthBias, float fReflectionScale)
	{
		m_fViewAngleThreshold = fViewAngleThreshold;
		m_fEdgeDistThreshold = fEdgeDistThreshold;
		m_fDepthBias = fDepthBias;
		m_fReflectionScale = fReflectionScale;
	}

	// Prepare for the reflection calculation
	void PreRenderReflection(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pDiffuseSRV, ID3D11ShaderResourceView* pDepthSRV, ID3D11ShaderResourceView* pNormalsSRV, ID3D11DepthStencilView* ptDepthReadOnlyDSV);

	// Clean up after the reflection calculation
	void PostRenderReflection(ID3D11DeviceContext* pd3dImmediateContext);

	// Do the reflections blend with light accumulation
	void DoReflectionBlend(ID3D11DeviceContext* pd3dImmediateContext);

private:

	float m_fViewAngleThreshold;
	float m_fEdgeDistThreshold;
	float m_fDepthBias;
	float m_fReflectionScale;

	ID3D11VertexShader* m_pRefelctionVS = nullptr;
	ID3D11PixelShader* m_pRefelctionPS = nullptr;
	ID3D11VertexShader* m_pRefelctionBlendVS = nullptr;
	ID3D11PixelShader* m_pRefelctionBlendPS = nullptr;
	ID3D11Buffer* m_pRefelctionPixelShaderCB = nullptr;
	ID3D11InputLayout* m_pReflectVSLayout = nullptr;

	ID3D11Texture2D* m_pReflectTexture = nullptr;
	ID3D11RenderTargetView* m_ReflectRTV = nullptr;
	ID3D11ShaderResourceView* m_ReflectSRV = nullptr;

	ID3D11Buffer* m_pRefelctionVexterShaderCB = nullptr;
};