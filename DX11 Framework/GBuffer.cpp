#include "stdafx.h"
#include "GBuffer.h"

CGBuffer::CGBuffer()
{

}

CGBuffer::~CGBuffer()
{
	ReleaseCOM(m_pGBufferUnpackCB);

	ReleaseCOM(m_DepthStencilRT);
	ReleaseCOM(m_DiffuseSpecIntensityRT);
	ReleaseCOM(m_NormalRT);
	ReleaseCOM(m_SpecPowerRT);

	ReleaseCOM(m_DepthStencilDSV);
	ReleaseCOM(m_DepthStencilReadOnlyDSV);

	ReleaseCOM(m_DiffuseSpecIntensityRTV);
	ReleaseCOM(m_NormalRTV);
	ReleaseCOM(m_SpecPowerRTV);

	ReleaseCOM(m_DepthStencilSRV);
	ReleaseCOM(m_DiffuseSpecIntensitySRV);
	ReleaseCOM(m_NormalSRV);
	ReleaseCOM(m_SpecPowerSRV);

	SafeDelete(m_pDepthStencilMesh);
	SafeDelete(m_pDiffuseMesh);
	SafeDelete(m_pNormalMesh);
	SafeDelete(m_pSpecPowerMesh);

	SafeDelete(m_pDepthShader);
	SafeDelete(m_pDiffuseShader);
	SafeDelete(m_pNormalShader);
	SafeDelete(m_pSpecShader);

	ReleaseCOM(m_pDeferrdVertexShader);
	ReleaseCOM(m_pDeferrdPixelShader);
}

void CGBuffer::Initialize(ID3D11Device* pDevice, UINT width, UINT height)
{
	// Texture formats
	static const DXGI_FORMAT depthStencilTextureFormat = DXGI_FORMAT_R24G8_TYPELESS;
	static const DXGI_FORMAT basicDiffuseTextureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	static const DXGI_FORMAT normalTextureFormat = DXGI_FORMAT_R11G11B10_FLOAT;
	static const DXGI_FORMAT specPowTextureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Render view formats
	static const DXGI_FORMAT depthStencilRenderViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	static const DXGI_FORMAT basicDiffuseRenderViewFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	static const DXGI_FORMAT normalRenderViewFormat = DXGI_FORMAT_R11G11B10_FLOAT;
	static const DXGI_FORMAT specPowRenderViewFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Resource view formats
	static const DXGI_FORMAT depthStencilResourceViewFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	static const DXGI_FORMAT basicDiffuseResourceViewFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	static const DXGI_FORMAT normalResourceViewFormat = DXGI_FORMAT_R11G11B10_FLOAT;
	static const DXGI_FORMAT specPowResourceViewFormat = DXGI_FORMAT_R8G8B8A8_UNORM;


	// ========================= Create Texture 2D =============================== //
	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = width;
	d3dTexture2DDesc.Height = height;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	HR(pDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_DepthStencilRT));
	DXUT_SetDebugName(m_DepthStencilRT, "GBuffer - Depth Stencil");

	d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DDesc.Format = basicDiffuseTextureFormat;

	HR(pDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_DiffuseSpecIntensityRT));
	DXUT_SetDebugName(m_DiffuseSpecIntensityRT, "GBuffer - Diffuse Specular Intensity");

	d3dTexture2DDesc.Format = normalTextureFormat;
	HR(pDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_NormalRT));
	DXUT_SetDebugName(m_NormalRT, "GBuffer - Normal");

	d3dTexture2DDesc.Format = specPowTextureFormat;
	HR(pDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_SpecPowerRT));
	DXUT_SetDebugName(m_SpecPowerRT, "GBuffer - Specular Power");

	// ========================= Create RenderTarget View =============================== //
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDSVDesc = { depthStencilRenderViewFormat, D3D11_DSV_DIMENSION_TEXTURE2D, 0 };
	HR(pDevice->CreateDepthStencilView(m_DepthStencilRT, &d3dDSVDesc, &m_DepthStencilDSV));
	DXUT_SetDebugName(m_DepthStencilDSV, "GBuffer - Depth Stencil DSV");

	d3dDSVDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;
	HR(pDevice->CreateDepthStencilView(m_DepthStencilRT, &d3dDSVDesc, &m_DepthStencilReadOnlyDSV));
	DXUT_SetDebugName(m_DepthStencilReadOnlyDSV, "GBuffer - Depth Stencil Read Only DSV");


	D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
	d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	d3dRTVDesc.Texture2D.MipSlice = 0;
	d3dRTVDesc.Format = basicDiffuseRenderViewFormat;

	HR(pDevice->CreateRenderTargetView(m_DiffuseSpecIntensityRT, &d3dRTVDesc, &m_DiffuseSpecIntensityRTV));
	DXUT_SetDebugName(m_DiffuseSpecIntensityRTV, "GBuffer - Color Spec Intensity RTV");

	d3dRTVDesc.Format = normalRenderViewFormat;
	HR(pDevice->CreateRenderTargetView(m_NormalRT, &d3dRTVDesc, &m_NormalRTV));
	DXUT_SetDebugName(m_NormalRTV, "GBuffer - Normal RTV");

	d3dRTVDesc.Format = specPowRenderViewFormat;
	HR(pDevice->CreateRenderTargetView(m_SpecPowerRT, &d3dRTVDesc, &m_SpecPowerRTV));
	DXUT_SetDebugName(m_SpecPowerRTV, "GBuffer - Spec Power RTV");

	// =============================== Create Shader Resource View =============================== //
	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc =
	{
		depthStencilResourceViewFormat,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		0, 0
	};
	d3dSRVDesc.Texture2D.MipLevels = 1;
	HR(pDevice->CreateShaderResourceView(m_DepthStencilRT, &d3dSRVDesc, &m_DepthStencilSRV));
	DXUT_SetDebugName(m_DepthStencilSRV, "GBuffer - Depth SRV");

	d3dSRVDesc.Format = basicDiffuseResourceViewFormat;
	HR(pDevice->CreateShaderResourceView(m_DiffuseSpecIntensityRT, &d3dSRVDesc, &m_DiffuseSpecIntensitySRV));
	DXUT_SetDebugName(m_DiffuseSpecIntensitySRV, "GBuffer - Diffuse Spec Intensity SRV");

	d3dSRVDesc.Format = normalResourceViewFormat;
	HR(pDevice->CreateShaderResourceView(m_NormalRT, &d3dSRVDesc, &m_NormalSRV));
	DXUT_SetDebugName(m_NormalSRV, "GBuffer - Normal SRV");

	d3dSRVDesc.Format = specPowResourceViewFormat;
	HR(pDevice->CreateShaderResourceView(m_SpecPowerRT, &d3dSRVDesc, &m_SpecPowerSRV));
	DXUT_SetDebugName(m_SpecPowerSRV, "GBuffer - Spec Power SRV");

	// ================================= Create Constant Buffers ===================================== //
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.ByteWidth = sizeof(CB_GBUFFER_UNPACK);
	HR(pDevice->CreateBuffer(&cbDesc, nullptr, &m_pGBufferUnpackCB));
	DXUT_SetDebugName(m_pGBufferUnpackCB, "GBufferUnpack CB");

	// ================================= Create Texture Mesh ===================================== //
	m_pDepthStencilMesh = new CUIMesh(pDevice, POINT{ 50, 650 }, POINT{ 350, 840 }, 0.1f);
	m_pDiffuseMesh = new CUIMesh(pDevice, POINT{ 450, 650 }, POINT{ 750, 840 }, 0.1f);
	m_pNormalMesh = new CUIMesh(pDevice, POINT{ 850, 650 }, POINT{ 1150, 840 }, 0.1f);
	m_pSpecPowerMesh = new CUIMesh(pDevice, POINT{ 1250, 650 }, POINT{ 1550, 840 }, 0.1f);

	// =================================== Create Shader ========================================= //
	m_pDepthShader = new CGBufferShader(GBufferType::eDepth);
	m_pDepthShader->CreateShader(pDevice);

	m_pDiffuseShader = new CGBufferShader(GBufferType::eDiffuse);
	m_pDiffuseShader->CreateShader(pDevice);

	m_pNormalShader = new CGBufferShader(GBufferType::eNormal);
	m_pNormalShader->CreateShader(pDevice);

	m_pSpecShader = new CGBufferShader(GBufferType::eSpec);
	m_pSpecShader->CreateShader(pDevice);

	CShader::CreateVertexShaderFromFile(pDevice, L"../Shader HLSL File/PostProcessing.hlsli", "VSPostProcessing", "vs_5_0", &m_pDeferrdVertexShader);
	CShader::CreatePixelShaderFromFile(pDevice, L"../Shader HLSL File/PostProcessing.hlsli", "PSPostProcessing", "ps_5_0", &m_pDeferrdPixelShader);
}

void CGBuffer::OnPreRender(ID3D11DeviceContext* pd3dImmediateContext)
{
	pd3dImmediateContext->ClearDepthStencilView(m_DepthStencilDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	pd3dImmediateContext->ClearRenderTargetView(m_DiffuseSpecIntensityRTV, ClearColor);
	pd3dImmediateContext->ClearRenderTargetView(m_NormalRTV, ClearColor);
	pd3dImmediateContext->ClearRenderTargetView(m_SpecPowerRTV, ClearColor);

	ID3D11RenderTargetView* rt[3] = { m_DiffuseSpecIntensityRTV, m_NormalRTV, m_SpecPowerRTV };
	pd3dImmediateContext->OMSetRenderTargets(3, rt, m_DepthStencilDSV);
	pd3dImmediateContext->OMSetDepthStencilState(STATEOBJ_MGR->g_pGBufferDSS, 1);
}

void CGBuffer::OnPrepareForUnpack(ID3D11DeviceContext* pd3dImmediateContext)
{
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	HR(pd3dImmediateContext->Map(m_pGBufferUnpackCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_GBUFFER_UNPACK* pGBufferUnpackCB = (CB_GBUFFER_UNPACK*)MappedResource.pData;
	
	XMFLOAT4X4 pProj;
	XMStoreFloat4x4(&pProj, SCENE_MGR->g_pCamera->GetProjectionMatrix());
	pGBufferUnpackCB->m_vPerspectiveValues.x = 1.0f / pProj.m[0][0];
	pGBufferUnpackCB->m_vPerspectiveValues.y = 1.0f / pProj.m[1][1];
	pGBufferUnpackCB->m_vPerspectiveValues.z = pProj.m[3][2];
	pGBufferUnpackCB->m_vPerspectiveValues.w = -pProj.m[2][2];

	XMFLOAT4X4 matViewInv;
	XMStoreFloat4x4(&matViewInv, XMMatrixInverse(nullptr, SCENE_MGR->g_pCamera->GetViewMatrix()));
	XMStoreFloat4x4(&pGBufferUnpackCB->m_mtxViewInverse, XMMatrixTranspose(XMLoadFloat4x4(&matViewInv)));

	pd3dImmediateContext->Unmap(m_pGBufferUnpackCB, 0);

	pd3dImmediateContext->PSSetConstantBuffers(PS_CB_SLOT_GBUFFER_UNPACK, 1, &m_pGBufferUnpackCB);
}

void CGBuffer::OnPostRender(ID3D11DeviceContext* pd3dImmediateContext)
{
	ID3D11RenderTargetView* rt[3] = { NULL, NULL, NULL };
	pd3dImmediateContext->OMSetRenderTargets(3, rt, m_DepthStencilReadOnlyDSV);
}

void CGBuffer::Render(ID3D11DeviceContext* pd3dImmediateContext)
{
	ID3D11ShaderResourceView* arrViews[4] = { m_DepthStencilSRV, m_DiffuseSpecIntensitySRV, m_NormalSRV, m_SpecPowerSRV };
	pd3dImmediateContext->PSSetShaderResources(PS_TEXTRUE_SLOT_GBUFFER, 4, arrViews);
	pd3dImmediateContext->PSSetSamplers(0, 1, &STATEOBJ_MGR->g_pLinearClampSS);
	
	m_pDepthShader->OnPrepareRender(pd3dImmediateContext);
	m_pDepthStencilMesh->Render(pd3dImmediateContext);

	m_pDiffuseShader->OnPrepareRender(pd3dImmediateContext);
	m_pDiffuseMesh->Render(pd3dImmediateContext);

	m_pNormalShader->OnPrepareRender(pd3dImmediateContext);
	m_pNormalMesh->Render(pd3dImmediateContext);

	m_pSpecShader->OnPrepareRender(pd3dImmediateContext);
	m_pSpecPowerMesh->Render(pd3dImmediateContext);

	// Clear
	pd3dImmediateContext->VSSetShader(nullptr, nullptr, 0);
	pd3dImmediateContext->PSSetShader(nullptr, nullptr, 0);
	ZeroMemory(arrViews, sizeof(arrViews));
	pd3dImmediateContext->PSSetShaderResources(PS_TEXTRUE_SLOT_GBUFFER, 4, arrViews);
}

void CGBuffer::DeferredRender(ID3D11DeviceContext* pd3dImmediateContext)
{
	pd3dImmediateContext->IASetInputLayout(NULL);
	pd3dImmediateContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	pd3dImmediateContext->VSSetShader(m_pDeferrdVertexShader, NULL, 0);
	pd3dImmediateContext->PSSetShader(m_pDeferrdPixelShader, NULL, 0);

	ID3D11ShaderResourceView* arrViews[4] = { m_DepthStencilSRV, m_DiffuseSpecIntensitySRV, m_NormalSRV, m_SpecPowerSRV };
	pd3dImmediateContext->PSSetShaderResources(PS_TEXTRUE_SLOT_GBUFFER, 4, arrViews);

	pd3dImmediateContext->Draw(4, 0);
	
	// Clear
	ID3D11ShaderResourceView *arrRV[1] = { NULL };
	pd3dImmediateContext->PSSetShaderResources(4, 1, arrRV);
	pd3dImmediateContext->VSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->PSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ZeroMemory(arrViews, sizeof(arrViews));
	pd3dImmediateContext->PSSetShaderResources(0, 4, arrViews);
}