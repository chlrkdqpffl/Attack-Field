#include "stdafx.h"
#include "SSReflection.h"

#pragma pack(push,1)
struct CB_REFLECT_VS
{
	XMFLOAT4X4 mWorldViewProj;
	XMFLOAT4X4 mWorldView;
};

struct CB_REFLECT_PS
{
	XMFLOAT4X4 mProjMatrix;
	float fViewAngleThreshold;
	float fEdgeDistThreshold;
	float fDepthBias;
	float fReflectionScale;
	XMFLOAT4 vPerspectiveValues;
};
#pragma pack(pop)

CSSReflection::CSSReflection() 
	: m_fViewAngleThreshold(0.2f), m_fEdgeDistThreshold(0.2f), m_fDepthBias(0.0025f), m_fReflectionScale(2.0f)
{
}

CSSReflection::~CSSReflection()
{
}

void CSSReflection::Initialize(ID3D11Device* pDevice, UINT width, UINT height)
{
	// Create constant buffer
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory( &cbDesc, sizeof(cbDesc) );
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	cbDesc.ByteWidth = sizeof(CB_REFLECT_VS);
	HR(pDevice->CreateBuffer(&cbDesc, NULL, &m_pRefelctionVexterShaderCB));
	DXUT_SetDebugName(m_pRefelctionVexterShaderCB, "Reflection Vertex Shader CB");

	cbDesc.ByteWidth = sizeof( CB_REFLECT_PS );
	HR(pDevice->CreateBuffer( &cbDesc, NULL, &m_pRefelctionPixelShaderCB ) );
	DXUT_SetDebugName( m_pRefelctionPixelShaderCB, "Reflection Pixel Shader CB" );

	// Compile the shaders
	wstring strFileName;
	// Directional Light Shader
	strFileName = L"../Shader HLSL File/SSReflection.hlsli";

	// Create a layout for the object data
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CShader::CreateLayoutFromFile(pDevice, strFileName, "SSReflectionVS", "vs_5_0", layout, ARRAYSIZE(layout), &m_pReflectVSLayout);
	CShader::CreateVertexShaderFromFile(pDevice, strFileName, "SSReflectionVS", "vs_5_0", &m_pRefelctionVS);
	CShader::CreatePixelShaderFromFile(pDevice, strFileName, "SSReflectionPS", "ps_5_0", &m_pRefelctionPS);

	CShader::CreateVertexShaderFromFile(pDevice, strFileName, "ReflectionBlendVS", "vs_5_0", &m_pRefelctionBlendVS);
	CShader::CreatePixelShaderFromFile(pDevice, strFileName, "ReflectionBlendPS", "ps_5_0", &m_pRefelctionBlendPS);


	// Create the HDR render target
	D3D11_TEXTURE2D_DESC dtd = {
		width, //UINT Width;
		height, //UINT Height;
		1, //UINT MipLevels;
		1, //UINT ArraySize;
		DXGI_FORMAT_R16G16B16A16_TYPELESS, //DXGI_FORMAT Format;
		1, //DXGI_SAMPLE_DESC SampleDesc;
		0,
		D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
		D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE,//UINT BindFlags;
		0,//UINT CPUAccessFlags;
		0//UINT MiscFlags;    
	};
	HR(pDevice->CreateTexture2D( &dtd, NULL, &m_pReflectTexture ) );
	DXUT_SetDebugName( m_pReflectTexture, "Reflection Accumulation Texture" );

	D3D11_RENDER_TARGET_VIEW_DESC rtsvd = 
	{
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		D3D11_RTV_DIMENSION_TEXTURE2D
	};
	HR(pDevice->CreateRenderTargetView( m_pReflectTexture, &rtsvd, &m_ReflectRTV ) );
	DXUT_SetDebugName( m_ReflectRTV, "Reflection Accumulation RTV" );

	D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd = 
	{
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		0,
		0
	};
	dsrvd.Texture2D.MipLevels = 1;
	HR(pDevice->CreateShaderResourceView( m_pReflectTexture, &dsrvd, &m_ReflectSRV ) );
	DXUT_SetDebugName( m_ReflectSRV, "Reflection Accumulation SRV" );
}

void CSSReflection::Deinit()
{
	ReleaseCOM( m_pRefelctionVS );
	ReleaseCOM( m_pRefelctionPS );
	ReleaseCOM( m_pRefelctionBlendVS );
	ReleaseCOM( m_pRefelctionBlendPS );
	ReleaseCOM( m_pRefelctionPixelShaderCB );
	ReleaseCOM( m_pReflectVSLayout );
	ReleaseCOM( m_pReflectTexture );
	ReleaseCOM( m_ReflectRTV );
	ReleaseCOM( m_ReflectSRV );
	ReleaseCOM(m_pRefelctionVexterShaderCB);
}

void CSSReflection::PreRenderReflection(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pDiffuseSRV, ID3D11ShaderResourceView* pDepthSRV,ID3D11ShaderResourceView* pNormalsSRV, ID3D11DepthStencilView* ptDepthReadOnlyDSV)
{
	pd3dImmediateContext->OMSetDepthStencilState(STATEOBJ_MGR->g_pNoDepthEqualWritesDSS, 0);
	
	// Clear to black
	float ClearColor[4] = { 0.0f, 0.0, 0.0, 0.0f };
	pd3dImmediateContext->ClearRenderTargetView( m_ReflectRTV, ClearColor );
	pd3dImmediateContext->OMSetRenderTargets( 1, &m_ReflectRTV, ptDepthReadOnlyDSV );

	const XMMATRIX mProj = SCENE_MGR->g_pCamera->GetProjectionMatrix();

	XMFLOAT4X4 mtxProj;
	XMStoreFloat4x4(&mtxProj, mProj);
	const XMMATRIX mWorldViewProj = SCENE_MGR->g_pCamera->GetViewMatrix() * mProj;

	// Fill the constant buffers
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	HR(pd3dImmediateContext->Map(m_pRefelctionVexterShaderCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_REFLECT_VS* pReflectVSCB = (CB_REFLECT_VS*)MappedResource.pData;
	XMStoreFloat4x4(&pReflectVSCB->mWorldViewProj, XMMatrixTranspose(mWorldViewProj));
	XMStoreFloat4x4(&pReflectVSCB->mWorldView, XMMatrixTranspose(SCENE_MGR->g_pCamera->GetViewMatrix()));
	
	pd3dImmediateContext->Unmap(m_pRefelctionVexterShaderCB, 0);
	pd3dImmediateContext->VSSetConstantBuffers(VS_CB_SLOT_REFLECTION, 1, &m_pRefelctionVexterShaderCB);


//	D3D11_MAPPED_SUBRESOURCE MappedResource;
	HR(pd3dImmediateContext->Map( m_pRefelctionPixelShaderCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ));
	CB_REFLECT_PS* pReflectCB = ( CB_REFLECT_PS* )MappedResource.pData;
	XMStoreFloat4x4(&pReflectCB->mProjMatrix, XMMatrixTranspose(mProj));
	pReflectCB->fViewAngleThreshold = m_fViewAngleThreshold;
	pReflectCB->fEdgeDistThreshold = m_fEdgeDistThreshold;
	pReflectCB->fDepthBias = m_fDepthBias;
	pReflectCB->fReflectionScale = m_fReflectionScale;
	pReflectCB->vPerspectiveValues.x = 1.0f / mtxProj.m[0][0];
	pReflectCB->vPerspectiveValues.y = 1.0f / mtxProj.m[1][1];
	pReflectCB->vPerspectiveValues.z = mtxProj.m[3][2];
	pReflectCB->vPerspectiveValues.w = -mtxProj.m[2][2];
	pd3dImmediateContext->Unmap( m_pRefelctionPixelShaderCB, 0 );
	pd3dImmediateContext->PSSetConstantBuffers(PS_CB_SLOT_REFLECT, 1, &m_pRefelctionPixelShaderCB );

	ID3D11ShaderResourceView* arrViews[3] = { pDiffuseSRV, pDepthSRV, pNormalsSRV };
	pd3dImmediateContext->PSSetShaderResources(PS_TEXTURE_SLOT_REFLECTION, 3, arrViews);

	pd3dImmediateContext->IASetInputLayout( m_pReflectVSLayout );

	// Set the shaders
	pd3dImmediateContext->VSSetShader(m_pRefelctionVS, nullptr, 0);
	pd3dImmediateContext->PSSetShader(m_pRefelctionPS, nullptr, 0);
}

void CSSReflection::PostRenderReflection(ID3D11DeviceContext* pd3dImmediateContext)
{
	ID3D11ShaderResourceView* arrViews[3] = { nullptr, nullptr, nullptr };
	pd3dImmediateContext->PSSetShaderResources(PS_TEXTURE_SLOT_REFLECTION, 3, arrViews);
}

void CSSReflection::DoReflectionBlend(ID3D11DeviceContext* pd3dImmediateContext)
{
	ID3D11BlendState* pPrevBlendState;
	FLOAT prevBlendFactor[ 4 ];
	UINT prevSampleMask;
	pd3dImmediateContext->OMGetBlendState(&pPrevBlendState, prevBlendFactor, &prevSampleMask);
//	pd3dImmediateContext->OMSetBlendState(STATEOBJ_MGR->g_pAddativeBS, prevBlendFactor, prevSampleMask);
	pd3dImmediateContext->OMSetBlendState(STATEOBJ_MGR->g_pFireBS, prevBlendFactor, prevSampleMask);

	pd3dImmediateContext->PSSetShaderResources(PS_TEXTURE_SLOT_REFLECTION, 1, &m_ReflectSRV);

	// Primitive settings
	pd3dImmediateContext->IASetInputLayout(nullptr);
	pd3dImmediateContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	pd3dImmediateContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// Set the shaders
	pd3dImmediateContext->VSSetShader(m_pRefelctionBlendVS, nullptr, 0);
	pd3dImmediateContext->PSSetShader(m_pRefelctionBlendPS, nullptr, 0);

	pd3dImmediateContext->Draw(4, 0);

	// Cleanup
	ID3D11ShaderResourceView *arrRV[1] = { nullptr };
	pd3dImmediateContext->PSSetShaderResources(PS_TEXTURE_SLOT_REFLECTION, 1, arrRV);
	pd3dImmediateContext->VSSetShader(nullptr, nullptr, 0);
	pd3dImmediateContext->PSSetShader(nullptr, nullptr, 0);
	pd3dImmediateContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	pd3dImmediateContext->OMSetBlendState(pPrevBlendState, prevBlendFactor, prevSampleMask);
	ReleaseCOM( pPrevBlendState );
}
