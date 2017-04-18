#pragma once
#include "UIMesh.h"
#include "ScreenShader.h"

#pragma pack(push,1)
struct CB_GBUFFER_UNPACK
{
	XMFLOAT4	m_vPerspectiveValues;
	XMFLOAT4X4	m_mtxViewInverse;
};
#pragma pack(pop)

class CGBuffer
{
public:
	CGBuffer();
	virtual ~CGBuffer();

	void Initialize(ID3D11Device* pDevice);
	void OnPreRender(ID3D11DeviceContext* pd3dImmediateContext);
	void OnPrepareForUnpack(ID3D11DeviceContext* pContext);

	void Render(ID3D11DeviceContext* pContext);

private:
	ID3D11Buffer*	m_pGBufferUnpackCB					= nullptr;

	ID3D11Texture2D* m_DepthStencilRT					= nullptr;
	ID3D11Texture2D* m_DiffuseSpecIntensityRT			= nullptr;
	ID3D11Texture2D* m_NormalRT							= nullptr;
	ID3D11Texture2D* m_SpecPowerRT						= nullptr;

	ID3D11DepthStencilView* m_DepthStencilDSV			= nullptr;
	ID3D11DepthStencilView* m_DepthStencilReadOnlyDSV	= nullptr;
	ID3D11RenderTargetView* m_DiffuseSpecIntensityRTV	= nullptr;
	ID3D11RenderTargetView* m_NormalRTV					= nullptr;
	ID3D11RenderTargetView* m_SpecPowerRTV				= nullptr;

	ID3D11ShaderResourceView* m_DepthStencilSRV			= nullptr;
	ID3D11ShaderResourceView* m_DiffuseSpecIntensitySRV = nullptr;
	ID3D11ShaderResourceView* m_NormalSRV				= nullptr;
	ID3D11ShaderResourceView* m_SpecPowerSRV			= nullptr;

	ID3D11DepthStencilState *m_DepthStencilState		= nullptr;



	CScreenShader*	m_pScreenShader		= nullptr;
	CUIMesh* m_pDepthStencilMesh		= nullptr;
	CUIMesh* m_pDiffuseMesh				= nullptr;
	CUIMesh* m_pNormalMesh				= nullptr;
	CUIMesh* m_pSpecPowerMesh			= nullptr;
};