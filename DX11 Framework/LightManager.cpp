#include "stdafx.h"
#include "GBuffer.h"
#include "LightManager.h"

CLightManager::CLightManager() :
	m_pDirLightVertexShader(NULL), m_pDirLightPixelShader(NULL), m_pDirLightCB(NULL),
	m_pPointLightVertexShader(NULL), m_pPointLightHullShader(NULL), m_pPointLightDomainShader(NULL), m_pPointLightPixelShader(NULL),
	m_pPointLightDomainCB(NULL), m_pPointLightPixelCB(NULL),
	m_pSpotLightVertexShader(NULL), m_pSpotLightHullShader(NULL), m_pSpotLightDomainShader(NULL), m_pSpotLightPixelShader(NULL),
	m_pSpotLightDomainCB(NULL), m_pSpotLightPixelCB(NULL),
	m_pCapsuleLightVertexShader(NULL), m_pCapsuleLightHullShader(NULL), m_pCapsuleLightDomainShader(NULL), m_pCapsuleLightPixelShader(NULL),
	m_pCapsuleLightDomainCB(NULL), m_pCapsuleLightPixelCB(NULL),
	m_pDebugLightPixelShader(NULL),
	m_pNoDepthWriteLessStencilMaskState(NULL), m_pNoDepthWriteGreatherStencilMaskState(NULL),
	m_pAdditiveBlendState(NULL), m_pNoDepthClipFrontRS(NULL), m_pWireframeRS(NULL)
{
}

CLightManager::~CLightManager()
{
}

void CLightManager::InitializeManager()
{
	ID3D11Device* pd3dDevice = STATEOBJ_MGR->g_pd3dDevice;

	// Create constant buffers
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.ByteWidth = sizeof(CB_DIRECTIONAL);
	HR(pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pDirLightCB));
	DXUT_SetDebugName(m_pDirLightCB, "Directional Light CB");

	cbDesc.ByteWidth = sizeof(CB_POINT_LIGHT_DOMAIN);
	HR(pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pPointLightDomainCB));
	DXUT_SetDebugName(m_pPointLightDomainCB, "Point Light Domain CB");

	cbDesc.ByteWidth = sizeof(CB_POINT_LIGHT_PIXEL);
	HR(pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pPointLightPixelCB));
	DXUT_SetDebugName(m_pPointLightPixelCB, "Point Light Pixel CB");

	cbDesc.ByteWidth = sizeof(CB_SPOT_LIGHT_DOMAIN);
	HR(pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pSpotLightDomainCB));
	DXUT_SetDebugName(m_pSpotLightDomainCB, "Spot Light Domain CB");

	cbDesc.ByteWidth = sizeof(CB_SPOT_LIGHT_PIXEL);
	HR(pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pSpotLightPixelCB));
	DXUT_SetDebugName(m_pSpotLightPixelCB, "Spot Light Pixel CB");

	cbDesc.ByteWidth = sizeof(CB_CAPSULE_LIGHT_DOMAIN);
	HR(pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pCapsuleLightDomainCB));
	DXUT_SetDebugName(m_pCapsuleLightDomainCB, "Capsule Light Domain CB");

	cbDesc.ByteWidth = sizeof(CB_CAPSULE_LIGHT_PIXEL);
	HR(pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pCapsuleLightPixelCB));
	DXUT_SetDebugName(m_pCapsuleLightPixelCB, "Capsule Light Pixel CB");

	wstring strFileName;
	// Directional Light Shader
	strFileName = L"Shader HLSL File/DirLight.hlsli";
	CShader::CreateVertexShaderFromFile(pd3dDevice, strFileName, "DirLightVS", "vs_5_0", &m_pDirLightVertexShader);
	CShader::CreatePixelShaderFromFile(pd3dDevice, strFileName, "DirLightPS", "ps_5_0", &m_pDirLightPixelShader);
	// Point Light Shader
	strFileName = L"Shader HLSL File/PointLight.hlsli";
	CShader::CreateVertexShaderFromFile(pd3dDevice, strFileName, "PointLightVS", "vs_5_0", &m_pPointLightVertexShader);
	CShader::CreateHullShaderFromFile(pd3dDevice, strFileName, "PointLightHS", "hs_5_0", &m_pPointLightHullShader);
	CShader::CreateDomainShaderFromFile(pd3dDevice, strFileName, "PointLightDS", "ds_5_0", &m_pPointLightDomainShader);
	CShader::CreatePixelShaderFromFile(pd3dDevice, strFileName, "PointLightPS", "ps_5_0", &m_pPointLightPixelShader);

	// Spot Light Shader
	strFileName = L"Shader HLSL File/SpotLight.hlsli";
	CShader::CreateVertexShaderFromFile(pd3dDevice, strFileName, "SpotLightVS", "vs_5_0", &m_pSpotLightVertexShader);
	CShader::CreateHullShaderFromFile(pd3dDevice, strFileName, "SpotLightHS", "hs_5_0", &m_pSpotLightHullShader);
	CShader::CreateDomainShaderFromFile(pd3dDevice, strFileName, "SpotLightDS", "ds_5_0", &m_pSpotLightDomainShader);
	CShader::CreatePixelShaderFromFile(pd3dDevice, strFileName, "SpotLightPS", "ps_5_0", &m_pSpotLightPixelShader);

	// Capsule Light Shader
	strFileName = L"Shader HLSL File/CapsuleLight.hlsli";
	CShader::CreateVertexShaderFromFile(pd3dDevice, strFileName, "CapsuleLightVS", "vs_5_0", &m_pCapsuleLightVertexShader);
	CShader::CreateHullShaderFromFile(pd3dDevice, strFileName, "CapsuleLightHS", "hs_5_0", &m_pCapsuleLightHullShader);
	CShader::CreateDomainShaderFromFile(pd3dDevice, strFileName, "CapsuleLightDS", "ds_5_0", &m_pCapsuleLightDomainShader);
	CShader::CreatePixelShaderFromFile(pd3dDevice, strFileName, "CapsuleLightPS", "ps_5_0", &m_pCapsuleLightPixelShader);

	// Light Volume Debug Shader
	strFileName = L"Shader HLSL File/Common.hlsli";
	CShader::CreatePixelShaderFromFile(pd3dDevice, strFileName, "DebugLightPS", "ps_5_0", &m_pDebugLightPixelShader);

	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = TRUE;
	descDepth.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	descDepth.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC noSkyStencilOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_EQUAL };
	descDepth.FrontFace = noSkyStencilOp;
	descDepth.BackFace = noSkyStencilOp;
	HR(pd3dDevice->CreateDepthStencilState(&descDepth, &m_pNoDepthWriteLessStencilMaskState));
	DXUT_SetDebugName(m_pNoDepthWriteLessStencilMaskState, "Depth Test Less / No Write, Stencil Mask DS");

	descDepth.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	HR(pd3dDevice->CreateDepthStencilState(&descDepth, &m_pNoDepthWriteGreatherStencilMaskState));
	DXUT_SetDebugName(m_pNoDepthWriteGreatherStencilMaskState, "Depth Test Greather / No Write, Stencil Mask DS");

	// Create the additive blend state
	D3D11_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	const D3D11_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
	{
		TRUE,
		D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
		D3D11_COLOR_WRITE_ENABLE_ALL,
	};
	for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		descBlend.RenderTarget[i] = defaultRenderTargetBlendDesc;
	HR(pd3dDevice->CreateBlendState(&descBlend, &m_pAdditiveBlendState));
	DXUT_SetDebugName(m_pAdditiveBlendState, "Additive Alpha BS");

	D3D11_RASTERIZER_DESC descRast = {
		D3D11_FILL_SOLID,
		D3D11_CULL_FRONT,
		FALSE,
		D3D11_DEFAULT_DEPTH_BIAS,
		D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
		FALSE,
		FALSE,
		FALSE,
		FALSE
	};
	descRast.CullMode = D3D11_CULL_FRONT;
	HR(pd3dDevice->CreateRasterizerState(&descRast, &m_pNoDepthClipFrontRS));
	DXUT_SetDebugName(m_pNoDepthClipFrontRS, "No Depth Clip Front RS");

	descRast.DepthClipEnable = TRUE;
	descRast.CullMode = D3D11_CULL_BACK;
	//descRast.CullMode = D3D11_CULL_NONE;
	descRast.FillMode = D3D11_FILL_WIREFRAME;
	HR(pd3dDevice->CreateRasterizerState(&descRast, &m_pWireframeRS));
	DXUT_SetDebugName(m_pWireframeRS, "Wireframe RS");
}

void CLightManager::ReleseManager()
{
	ReleaseCOM(m_pDirLightVertexShader);
	ReleaseCOM(m_pDirLightPixelShader);
	ReleaseCOM(m_pDirLightCB);
	ReleaseCOM(m_pPointLightVertexShader);
	ReleaseCOM(m_pPointLightHullShader);
	ReleaseCOM(m_pPointLightDomainShader);
	ReleaseCOM(m_pPointLightPixelShader);
	ReleaseCOM(m_pPointLightDomainCB);
	ReleaseCOM(m_pPointLightPixelCB);
	ReleaseCOM(m_pSpotLightVertexShader);
	ReleaseCOM(m_pSpotLightHullShader);
	ReleaseCOM(m_pSpotLightDomainShader);
	ReleaseCOM(m_pSpotLightPixelShader);
	ReleaseCOM(m_pSpotLightDomainCB);
	ReleaseCOM(m_pSpotLightPixelCB);
	ReleaseCOM(m_pCapsuleLightVertexShader);
	ReleaseCOM(m_pCapsuleLightHullShader);
	ReleaseCOM(m_pCapsuleLightDomainShader);
	ReleaseCOM(m_pCapsuleLightPixelShader);
	ReleaseCOM(m_pCapsuleLightDomainCB);
	ReleaseCOM(m_pCapsuleLightPixelCB);
	ReleaseCOM(m_pDebugLightPixelShader);
	ReleaseCOM(m_pNoDepthWriteLessStencilMaskState);
	ReleaseCOM(m_pNoDepthWriteGreatherStencilMaskState);
	ReleaseCOM(m_pAdditiveBlendState);
	ReleaseCOM(m_pNoDepthClipFrontRS);
	ReleaseCOM(m_pWireframeRS);
}

void CLightManager::DoLighting(ID3D11DeviceContext* pd3dImmediateContext, CCamera *pCamera)
{
	// Store the previous depth state
	ID3D11DepthStencilState* pPrevDepthState;
	UINT nPrevStencil;
	pd3dImmediateContext->OMGetDepthStencilState(&pPrevDepthState, &nPrevStencil);

	// Set the depth state for the directional light
	pd3dImmediateContext->OMSetDepthStencilState(m_pNoDepthWriteLessStencilMaskState, 1);

	// Set the GBuffer views
	ID3D11ShaderResourceView* arrViews[4] = { m_pGBuffer->GetDepthView(), m_pGBuffer->GetDiffuseView(), m_pGBuffer->GetNormalView(), m_pGBuffer->GetSpecPowerView() };
	pd3dImmediateContext->PSSetShaderResources(PS_TEXTRUE_SLOT_GBUFFER, 4, arrViews);

	// Do the directional light
	DirectionalLight(pd3dImmediateContext);

	// Once we are done with the directional light, turn on the blending
	ID3D11BlendState* pPrevBlendState;
	FLOAT prevBlendFactor[4];
	UINT prevSampleMask;
	pd3dImmediateContext->OMGetBlendState(&pPrevBlendState, prevBlendFactor, &prevSampleMask);
	pd3dImmediateContext->OMSetBlendState(m_pAdditiveBlendState, prevBlendFactor, prevSampleMask);

	// Set the depth state for the rest of the lights
	pd3dImmediateContext->OMSetDepthStencilState(m_pNoDepthWriteGreatherStencilMaskState, 1);

	ID3D11RasterizerState* pPrevRSState;
	pd3dImmediateContext->RSGetState(&pPrevRSState);
	pd3dImmediateContext->RSSetState(m_pNoDepthClipFrontRS);

	//int count = 0;
	// 절두체 컬링이 스피어라서 제대로 안됨
	for (auto light : m_arrLights) {
		if (!pCamera->IsInFrustum(&light.m_bsBoundingSphere))
			continue;
	//	count++;
		if (light.eLightType == TYPE_POINT)
			PointLight(pd3dImmediateContext, light.vPosition, light.fRange, light.vColor, false);
		else if (light.eLightType == TYPE_SPOT)
			SpotLight(pd3dImmediateContext, light.vPosition, light.vDirection, light.fRange, light.fInnerAngle, light.fOuterAngle, light.vColor, false);
		else if (light.eLightType == TYPE_CAPSULE)
			CapsuleLight(pd3dImmediateContext, light.vPosition, light.vDirection, light.fRange, light.fLength, light.vColor, false);
	}

	//cout << "현재 보이는 조명 갯수 : " << count << endl;

	pd3dImmediateContext->OMSetBlendState(pPrevBlendState, prevBlendFactor, prevSampleMask);
	ReleaseCOM(pPrevBlendState);
	pd3dImmediateContext->RSSetState(pPrevRSState);
	ReleaseCOM(pPrevRSState);
	pd3dImmediateContext->OMSetDepthStencilState(pPrevDepthState, nPrevStencil);
	ReleaseCOM(pPrevDepthState);

	// Cleanup
	pd3dImmediateContext->VSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->HSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->DSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->PSSetShader(NULL, NULL, 0);

	// Cleanup
	ZeroMemory(arrViews, sizeof(arrViews));
	pd3dImmediateContext->PSSetShaderResources(PS_TEXTRUE_SLOT_GBUFFER, 4, arrViews);
}

void CLightManager::DrawLightVolume(ID3D11DeviceContext* pd3dImmediateContext)
{
	// Set the wireframe mode
	ID3D11RasterizerState* pPrevRSState;
	pd3dImmediateContext->RSGetState(&pPrevRSState);
	pd3dImmediateContext->RSSetState(m_pWireframeRS);

	for (auto light : m_arrLights) {
		if (light.eLightType == TYPE_POINT)
			PointLight(pd3dImmediateContext, light.vPosition, light.fRange, light.vColor, true);
		else if (light.eLightType == TYPE_SPOT)
			SpotLight(pd3dImmediateContext, light.vPosition, light.vDirection, light.fRange, light.fInnerAngle, light.fOuterAngle, light.vColor, true);
		else if (light.eLightType == TYPE_CAPSULE)
			CapsuleLight(pd3dImmediateContext, light.vPosition, light.vDirection, light.fRange, light.fLength, light.vColor, true);
	}

	// Cleanup
	pd3dImmediateContext->VSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->HSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->DSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->PSSetShader(NULL, NULL, 0);

	// Restore the states
	pd3dImmediateContext->RSSetState(pPrevRSState);
	ReleaseCOM(pPrevRSState);
}

void CLightManager::DirectionalLight(ID3D11DeviceContext* pd3dImmediateContext)
{
	// Fill the directional and ambient values constant buffer
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	HR(pd3dImmediateContext->Map(m_pDirLightCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_DIRECTIONAL* pDirectionalValuesCB = (CB_DIRECTIONAL*)MappedResource.pData;

	XMFLOAT3 ambientUpperColor = GammaToLinear(m_vAmbientUpperColor);
	XMFLOAT3 ambientLowerColor = GammaToLinear(m_vAmbientLowerColor);
	pDirectionalValuesCB->vAmbientLower = ambientLowerColor;
	pDirectionalValuesCB->vAmbientRange.x = ambientUpperColor.x - ambientLowerColor.x;
	pDirectionalValuesCB->vAmbientRange.y = ambientUpperColor.y - ambientLowerColor.y;
	pDirectionalValuesCB->vAmbientRange.z = ambientUpperColor.z - ambientLowerColor.z;
	pDirectionalValuesCB->vDirToLight.x = -m_vDirectionalDir.x;
	pDirectionalValuesCB->vDirToLight.y = -m_vDirectionalDir.y;
	pDirectionalValuesCB->vDirToLight.z = -m_vDirectionalDir.z;
	pDirectionalValuesCB->vDirectionalColor = GammaToLinear(m_vDirectionalColor);
	pd3dImmediateContext->Unmap(m_pDirLightCB, 0);
	pd3dImmediateContext->PSSetConstantBuffers(PS_CB_SLOT_LIGHT, 1, &m_pDirLightCB);

	// Primitive settings
	pd3dImmediateContext->IASetInputLayout(NULL);
	pd3dImmediateContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Set the shaders
	pd3dImmediateContext->VSSetShader(m_pDirLightVertexShader, NULL, 0);
	pd3dImmediateContext->GSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->PSSetShader(m_pDirLightPixelShader, NULL, 0);

	pd3dImmediateContext->Draw(4, 0);

	// Cleanup
	ID3D11ShaderResourceView *arrRV[1] = { NULL };
//	pd3dImmediateContext->PSSetShaderResources(4, 1, arrRV);
	pd3dImmediateContext->VSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->PSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CLightManager::PointLight(ID3D11DeviceContext* pd3dImmediateContext, const XMFLOAT3& vPos, float fRange, const XMFLOAT3& vColor, bool bWireframe)
{
	XMMATRIX mLightWorldScale = XMMatrixScaling(fRange, fRange, fRange);
	XMMATRIX mLightWorldTrans = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);

	XMMATRIX mView = SCENE_MGR->g_pCamera->GetViewMatrix();
	XMMATRIX mProj = SCENE_MGR->g_pCamera->GetProjectionMatrix();
	XMMATRIX mWorldViewProjection = mLightWorldScale * mLightWorldTrans * mView * mProj;

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	HR(pd3dImmediateContext->Map(m_pPointLightDomainCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_POINT_LIGHT_DOMAIN* pPointLightDomainCB = (CB_POINT_LIGHT_DOMAIN*)MappedResource.pData;
	XMStoreFloat4x4(&pPointLightDomainCB->WolrdViewProj, XMMatrixTranspose(mWorldViewProjection));
	pd3dImmediateContext->Unmap(m_pPointLightDomainCB, 0);
	pd3dImmediateContext->DSSetConstantBuffers(DS_CB_SLOT_LIGHT, 1, &m_pPointLightDomainCB);

	HR(pd3dImmediateContext->Map(m_pPointLightPixelCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_POINT_LIGHT_PIXEL* pPointLightPixelCB = (CB_POINT_LIGHT_PIXEL*)MappedResource.pData;
	pPointLightPixelCB->PointLightPos = vPos;
	pPointLightPixelCB->PointLightRangeRcp = 1.0f / fRange;
	pPointLightPixelCB->PointColor = GammaToLinear(vColor);
	pd3dImmediateContext->Unmap(m_pPointLightPixelCB, 0);
	pd3dImmediateContext->PSSetConstantBuffers(PS_CB_SLOT_LIGHT, 1, &m_pPointLightPixelCB);

	pd3dImmediateContext->IASetInputLayout(NULL);
	pd3dImmediateContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);

	// Set the shaders
	pd3dImmediateContext->VSSetShader(m_pPointLightVertexShader, NULL, 0);
	pd3dImmediateContext->HSSetShader(m_pPointLightHullShader, NULL, 0);
	pd3dImmediateContext->DSSetShader(m_pPointLightDomainShader, NULL, 0);
	pd3dImmediateContext->GSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->PSSetShader(bWireframe ? m_pDebugLightPixelShader : m_pPointLightPixelShader, NULL, 0);

	pd3dImmediateContext->Draw(2, 0);
}

void CLightManager::SpotLight(ID3D11DeviceContext* pd3dImmediateContext, const XMFLOAT3& vPos, const XMFLOAT3& vDir, float fRange, float fInnerAngle, float fOuterAngle, const XMFLOAT3& vColor, bool bWireframe)
{
	// Convert angle in radians to sin/cos values
	float fCosInnerAngle = cosf(fInnerAngle);
	float fSinOuterAngle = sinf(fOuterAngle);
	float fCosOuterAngle = cosf(fOuterAngle);

	// Scale matrix from the cone local space to the world angles and range
	XMMATRIX mLightWorldScale = XMMatrixScaling(fRange, fRange, fRange);

	// Rotate and translate matrix from cone local space to lights world space
	XMVECTOR vUp = (vDir.y > 0.9 || vDir.y < -0.9) ? XMVectorSet(0.0f, 0.0f, vDir.y, 0.0f) : XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR vRight = XMVector3Cross(vUp, XMLoadFloat3(&vDir));
	vRight = XMVector3NormalizeEst(vRight);

	vUp = XMVector3Cross(XMLoadFloat3(&vDir), vRight);
	vUp = XMVector3NormalizeEst(vUp);
	XMVECTOR vAt = XMLoadFloat3(&vPos) + XMLoadFloat3(&vDir) * fRange;

	XMFLOAT3 f3Right; XMStoreFloat3(&f3Right, vRight);
	XMFLOAT3 f3Up; XMStoreFloat3(&f3Up, vUp);

	XMMATRIX m_LightWorldTransRotate;
	m_LightWorldTransRotate = XMMatrixSet(f3Right.x, f3Right.y, f3Right.z, 0.0f,
		f3Up.x, f3Up.y, f3Up.z, 0.0f,
		vDir.x, vDir.y, vDir.z, 0.0f,
		vPos.x, vPos.y, vPos.z, 1.0f);

	// Prepare the combined local to projected space matrix
	XMMATRIX mView = SCENE_MGR->g_pCamera->GetViewMatrix();
	XMMATRIX mProj = SCENE_MGR->g_pCamera->GetProjectionMatrix();
	XMMATRIX mWorldViewProjection = mLightWorldScale * m_LightWorldTransRotate * mView * mProj;

	// Write the matrix to the domain shader constant buffer
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	HR(pd3dImmediateContext->Map(m_pSpotLightDomainCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_SPOT_LIGHT_DOMAIN* pSpotLightDomainCB = (CB_SPOT_LIGHT_DOMAIN*)MappedResource.pData;
	XMStoreFloat4x4(&pSpotLightDomainCB->WolrdViewProj, XMMatrixTranspose(mWorldViewProjection));
	pSpotLightDomainCB->fSinAngle = fSinOuterAngle;
	pSpotLightDomainCB->fCosAngle = fCosOuterAngle;
	pd3dImmediateContext->Unmap(m_pSpotLightDomainCB, 0);
	pd3dImmediateContext->DSSetConstantBuffers(DS_CB_SLOT_LIGHT, 1, &m_pSpotLightDomainCB);

	HR(pd3dImmediateContext->Map(m_pSpotLightPixelCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_SPOT_LIGHT_PIXEL* pSpotLightPixelCB = (CB_SPOT_LIGHT_PIXEL*)MappedResource.pData;
	pSpotLightPixelCB->SpotLightPos = vPos;
	pSpotLightPixelCB->SpotLightRangeRcp = 1.0f / fRange;
	pSpotLightPixelCB->vDirToLight.x = -vDir.x;
	pSpotLightPixelCB->vDirToLight.y = -vDir.y;
	pSpotLightPixelCB->vDirToLight.z = -vDir.z;
	pSpotLightPixelCB->SpotCosOuterCone = fCosOuterAngle;
	pSpotLightPixelCB->SpotColor = GammaToLinear(vColor);
	pSpotLightPixelCB->SpotCosConeAttRange = fCosInnerAngle - fCosOuterAngle;
	pd3dImmediateContext->Unmap(m_pSpotLightPixelCB, 0);
	pd3dImmediateContext->PSSetConstantBuffers(PS_CB_SLOT_LIGHT, 1, &m_pSpotLightPixelCB);

	pd3dImmediateContext->IASetInputLayout(NULL);
	pd3dImmediateContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);

	// Set the shaders
	pd3dImmediateContext->VSSetShader(m_pSpotLightVertexShader, NULL, 0);
	pd3dImmediateContext->HSSetShader(m_pSpotLightHullShader, NULL, 0);
	pd3dImmediateContext->DSSetShader(m_pSpotLightDomainShader, NULL, 0);
	pd3dImmediateContext->GSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->PSSetShader(bWireframe ? m_pDebugLightPixelShader : m_pSpotLightPixelShader, NULL, 0);

	pd3dImmediateContext->Draw(1, 0);
}

void CLightManager::CapsuleLight(ID3D11DeviceContext* pd3dImmediateContext, const XMFLOAT3& vPos, const XMFLOAT3& vDir, float fRange, float fLen, const XMFLOAT3& vColor, bool bWireframe)
{
	// Rotate and translate matrix from capsule local space to lights world space
	XMVECTOR vUp = (vDir.y > 0.9 || vDir.y < -0.9) ? XMVectorSet(0.0f, 0.0f, vDir.y, 0.0f) : XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR vRight = XMVector3Cross(vUp, XMLoadFloat3(&vDir));
	vRight = XMVector3NormalizeEst(vRight);
	vUp = XMVector3Cross(XMLoadFloat3(&vDir), vRight);
	vUp = XMVector3NormalizeEst(vUp);

	XMVECTOR vCenterPos = XMLoadFloat3(&vPos) + 0.5f * XMLoadFloat3(&vDir) * fLen;
	XMVECTOR vAt = vCenterPos + XMLoadFloat3(&vDir) * fLen;

	XMFLOAT3 f3Right; XMStoreFloat3(&f3Right, vRight);
	XMFLOAT3 f3Up; XMStoreFloat3(&f3Up, vUp);
	XMFLOAT3 f3CenterPos; XMStoreFloat3(&f3CenterPos, vCenterPos);

	XMMATRIX m_LightWorldTransRotate;
	m_LightWorldTransRotate = XMMatrixSet(f3Right.x, f3Right.y, f3Right.z, 0.0f,
		f3Up.x, f3Up.y, f3Up.z, 0.0f,
		vDir.x, vDir.y, vDir.z, 0.0f,
		f3CenterPos.x, f3CenterPos.y, f3CenterPos.z, 1.0f);

	XMMATRIX mView = SCENE_MGR->g_pCamera->GetViewMatrix();
	XMMATRIX mProj = SCENE_MGR->g_pCamera->GetProjectionMatrix();
	XMMATRIX mWorldViewProjection = m_LightWorldTransRotate * mView * mProj;

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	HR(pd3dImmediateContext->Map(m_pCapsuleLightDomainCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_CAPSULE_LIGHT_DOMAIN* pCapsuleLightDomainCB = (CB_CAPSULE_LIGHT_DOMAIN*)MappedResource.pData;
	XMStoreFloat4x4(&pCapsuleLightDomainCB->WolrdViewProj, XMMatrixTranspose(mWorldViewProjection));
	pCapsuleLightDomainCB->HalfCapsuleLen = 0.5f * fLen;
	pCapsuleLightDomainCB->CapsuleRange = fRange;
	pd3dImmediateContext->Unmap(m_pCapsuleLightDomainCB, 0);
	pd3dImmediateContext->DSSetConstantBuffers(DS_CB_SLOT_LIGHT, 1, &m_pCapsuleLightDomainCB);

	HR(pd3dImmediateContext->Map(m_pCapsuleLightPixelCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_CAPSULE_LIGHT_PIXEL* pCapsuleLightPixelCB = (CB_CAPSULE_LIGHT_PIXEL*)MappedResource.pData;
	pCapsuleLightPixelCB->CapsuleLightPos = vPos;
	pCapsuleLightPixelCB->CapsuleLightRangeRcp = 1.0f / fRange;
	pCapsuleLightPixelCB->CapsuleDir = vDir;
	pCapsuleLightPixelCB->CapsuleLen = fLen;
	pCapsuleLightPixelCB->CapsuleColor = GammaToLinear(vColor);
	pd3dImmediateContext->Unmap(m_pCapsuleLightPixelCB, 0);
	pd3dImmediateContext->PSSetConstantBuffers(PS_CB_SLOT_LIGHT, 1, &m_pCapsuleLightPixelCB);

	pd3dImmediateContext->IASetInputLayout(NULL);
	pd3dImmediateContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);

	// Set the shaders
	pd3dImmediateContext->VSSetShader(m_pCapsuleLightVertexShader, NULL, 0);
	pd3dImmediateContext->HSSetShader(m_pCapsuleLightHullShader, NULL, 0);
	pd3dImmediateContext->DSSetShader(m_pCapsuleLightDomainShader, NULL, 0);
	pd3dImmediateContext->GSSetShader(NULL, NULL, 0);
	pd3dImmediateContext->PSSetShader(bWireframe ? m_pDebugLightPixelShader : m_pCapsuleLightPixelShader, NULL, 0);

	pd3dImmediateContext->Draw(2, 0);
}
