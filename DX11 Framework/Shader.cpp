//-----------------------------------------------------------------------------
// File: Shader.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Shader.h"
#include "BoundingBoxMesh.h"
#include "BoundingBoxShader.h"
#include "BoundingBoxInstancedShader.h"
#include "Scene.h"

CShader::CShader()
{
	m_pd3dVertexShader = NULL;
	m_pd3dVertexLayout = NULL;
	m_pd3dPixelShader = NULL;
	m_pd3dGeometryShader = NULL;

	m_nType = 0x00;
}

CShader::~CShader()
{
	ReleaseCOM(m_pd3dVertexLayout);
	ReleaseCOM(m_pd3dVertexShader);
	ReleaseCOM(m_pd3dPixelShader);
	ReleaseCOM(m_pd3dGeometryShader);
	ReleaseCOM(m_pd3dComputeShader);
	ReleaseCOM(m_pd3dHullShader);
	ReleaseCOM(m_pd3dDomainShader);

	if (m_pd3dInputElementDescs) delete[] m_pd3dInputElementDescs;
}

void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, const wstring& wstrFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputElements, UINT nElements, ID3D11InputLayout **ppd3dInputLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(wstrFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dVertexShaderBlob, &pd3dErrorBlob, NULL)))
	{
		HR(pd3dDevice->CreateVertexShader(pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader));
		HR(pd3dDevice->CreateInputLayout(pd3dInputElements, nElements, pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize(), ppd3dInputLayout));

		pd3dVertexShaderBlob->Release();

		DXUT_SetDebugName(*ppd3dInputLayout, pszShaderName);
		DXUT_SetDebugName(*ppd3dVertexShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", wstrFileName.c_str(), MB_OK);
	}
}

void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, const wstring& wstrFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(wstrFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dVertexShaderBlob, &pd3dErrorBlob, NULL)))
	{
		HR(pd3dDevice->CreateVertexShader(pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader));
		
		pd3dVertexShaderBlob->Release();

		DXUT_SetDebugName(*ppd3dVertexShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", wstrFileName.c_str(), MB_OK);
	}
}

void CShader::CreateLayoutFromFile(ID3D11Device *pd3dDevice, const wstring& wstrFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, D3D11_INPUT_ELEMENT_DESC *pd3dInputElements, UINT nElements, ID3D11InputLayout **ppd3dInputLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(wstrFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dVertexShaderBlob, &pd3dErrorBlob, NULL)))
	{
		HR(pd3dDevice->CreateInputLayout(pd3dInputElements, nElements, pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize(), ppd3dInputLayout));

		pd3dVertexShaderBlob->Release();

		DXUT_SetDebugName(*ppd3dInputLayout, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", wstrFileName.c_str(), MB_OK);
	}
}

void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, const wstring& wstrFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dPixelShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(wstrFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dPixelShaderBlob, &pd3dErrorBlob, NULL)))
	{
		HR(pd3dDevice->CreatePixelShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader));
		pd3dPixelShaderBlob->Release();
		DXUT_SetDebugName(*ppd3dPixelShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", wstrFileName.c_str(), MB_OK);
	}
}

void CShader::CreateGeometryShaderFromFile(ID3D11Device *pd3dDevice, const wstring& wstrFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dPixelShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(wstrFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dPixelShaderBlob, &pd3dErrorBlob, NULL)))
	{
		HR(pd3dDevice->CreateGeometryShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), NULL, ppd3dGeometryShader));
		pd3dPixelShaderBlob->Release();

		DXUT_SetDebugName(*ppd3dGeometryShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", wstrFileName.c_str(), MB_OK);
	}
}

void CShader::CreateComputeShaderFromFile(ID3D11Device *pd3dDevice, const wstring& wstrFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11ComputeShader **ppd3dComputeShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(wstrFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		HR(pd3dDevice->CreateComputeShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dComputeShader));
		pd3dShaderBlob->Release();

		DXUT_SetDebugName(*ppd3dComputeShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", wstrFileName.c_str(), MB_OK);
	}
}

void CShader::CreateHullShaderFromFile(ID3D11Device *pd3dDevice, const wstring& wstrFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11HullShader **ppd3dHullShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(wstrFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		HR(pd3dDevice->CreateHullShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dHullShader));
		pd3dShaderBlob->Release();
		DXUT_SetDebugName(*ppd3dHullShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", wstrFileName.c_str(), MB_OK);
	}
}

void CShader::CreateDomainShaderFromFile(ID3D11Device *pd3dDevice, const wstring& wstrFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11DomainShader **ppd3dDomainShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(wstrFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		HR(pd3dDevice->CreateDomainShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dDomainShader));
		pd3dShaderBlob->Release();
		DXUT_SetDebugName(*ppd3dDomainShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", wstrFileName.c_str(), MB_OK);
	}
}

void CShader::CreateShader(ID3D11Device *pd3dDevice, UINT nType)
{
	m_nType |= nType;
	GetInputElementDesc(m_nType);
	LPCSTR pszVSShaderName = NULL, pszVSShaderModel = "vs_5_0", pszPSShaderName = NULL, pszPSShaderModel = "ps_5_0";
	GetShaderName(m_nType, &pszVSShaderName, &pszVSShaderModel, &pszPSShaderName, &pszPSShaderModel);

	CreateShader(pd3dDevice, NULL, 0, L"../Shader HLSL File/Effect.fx", pszVSShaderName, pszVSShaderModel, pszPSShaderName, pszPSShaderModel);
}

void CShader::CreateShader(ID3D11Device *pd3dDevice, D3D11_INPUT_ELEMENT_DESC *pd3dInputElementDesc, int nInputElements, WCHAR *pszFileName, LPCSTR pszVSShaderName, LPCSTR pszVSShaderModel, LPCSTR pszPSShaderName, LPCSTR pszPSShaderModel)
{
	CreateVertexShaderFromFile(pd3dDevice, pszFileName, pszVSShaderName, pszVSShaderModel, &m_pd3dVertexShader, (pd3dInputElementDesc) ? pd3dInputElementDesc : m_pd3dInputElementDescs, (pd3dInputElementDesc) ? nInputElements : m_nInputElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, pszFileName, pszPSShaderName, pszPSShaderModel, &m_pd3dPixelShader);
}

void CShader::CreateShader(ID3D11Device *pd3dDevice, ShaderTag shaderTag)
{
	LPCSTR pszVSShaderName = NULL, pszVSShaderModel = "vs_5_0", pszPSShaderName = NULL, pszPSShaderModel = "ps_5_0";
	m_tagShader = shaderTag;

	switch (shaderTag) {
	case ShaderTag::eColor:
		m_nType = VERTEX_POSITION_ELEMENT | VERTEX_COLOR_ELEMENT;
		break;
	case ShaderTag::eNormal:
		m_nType = VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT;
		break;
	case ShaderTag::eTexture:
		m_nType = VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0;
		break;
	case ShaderTag::eNormalTexture:
		m_nType = VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0;
		break;
	case ShaderTag::eNormalTangentTexture:
		m_nType = VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TANGENT_ELEMENT | VERTEX_TEXTURE_ELEMENT_0;
		break;
	case ShaderTag::eDisplacementMapping:
	{
		// NormalDisplaceMapShader에서 사용중
		assert("여기에 들어오면 안된다.");
		return;
	}
		break;
	case ShaderTag::eInstanceNormal:
		m_nType = VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_INSTANCING_ELEMENT;
		break;
	case ShaderTag::eInstanceNormalTexture:
		m_nType = VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_INSTANCING_ELEMENT;
		break;
	case ShaderTag::eInstanceNormalTangentTexture:
		m_nType = VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TANGENT_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_INSTANCING_ELEMENT;
		break;
	case ShaderTag::eInstanceDisplacementMapping:
	{
		// NormalDisplaceMapShader에서 사용중
		assert("여기에 들어오면 안된다.");
		return;
	}
		break;
	}

	GetInputElementDesc(m_nType);
	GetShaderName(m_nType, &pszVSShaderName, &pszVSShaderModel, &pszPSShaderName, &pszPSShaderModel);

#ifdef USE_DEFERRD_RENDER
		CreateShader(pd3dDevice, NULL, 0, L"../Shader HLSL File/DeferredShading.hlsli", pszVSShaderName, pszVSShaderModel, pszPSShaderName, pszPSShaderModel);
#else
		CreateShader(pd3dDevice, NULL, 0, L"../Shader HLSL File/Effect.fx", pszVSShaderName, pszVSShaderModel, pszPSShaderName, pszPSShaderModel);
#endif
}

void CShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
}

void CShader::ReleaseShaderVariables()
{
}

void CShader::GetInputElementDesc(UINT nVertexElementType)
{
	m_nInputElements = 0;
	if (nVertexElementType & VERTEX_POSITION_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_COLOR_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_NORMAL_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_TANGENT_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_0) m_nInputElements++;
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_1) m_nInputElements++;
	if (nVertexElementType & VERTEX_BONE_ID_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_BONE_WEIGHT_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_INSTANCING_ELEMENT) m_nInputElements += 4;
	m_pd3dInputElementDescs = new D3D11_INPUT_ELEMENT_DESC[m_nInputElements];
	UINT nIndex = 0, nSlot = 0;
	if (nVertexElementType & VERTEX_POSITION_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_COLOR_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_NORMAL_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_TANGENT_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_0) m_pd3dInputElementDescs[nIndex++] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_1) m_pd3dInputElementDescs[nIndex++] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_BONE_ID_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_SINT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_BONE_WEIGHT_ELEMENT) m_pd3dInputElementDescs[nIndex++] = { "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_INSTANCING_ELEMENT)
	{
		m_pd3dInputElementDescs[nIndex++] = { "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		m_pd3dInputElementDescs[nIndex++] = { "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		m_pd3dInputElementDescs[nIndex++] = { "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		m_pd3dInputElementDescs[nIndex++] = { "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
	}
}

void CShader::GetShaderName(UINT nVertexElementType, LPCSTR *ppszVSShaderName, LPCSTR *ppszVSShaderModel, LPCSTR *ppszPSShaderName, LPCSTR *ppszPSShaderModel)
{
	int nInputElements = 0, nIndex = 0;
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_COLOR_ELEMENT)) {
		*ppszVSShaderName = "VSDiffusedColor", *ppszPSShaderName = "PSDiffusedColor";
	}
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT)) {
		*ppszVSShaderName = "VSLightingColor", *ppszPSShaderName = "PSLightingColor";
	}
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0)) { 
		*ppszVSShaderName = "VSTexturedColor", *ppszPSShaderName = "PSTexturedColor";
	}
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_TEXTURE_ELEMENT_1)) {
		*ppszVSShaderName = "VSDetailTexturedColor", *ppszPSShaderName = "PSDetailTexturedColor"; 
	}
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0)) { 
		*ppszVSShaderName = "VSTexturedLightingColor", *ppszPSShaderName = "PSTexturedLightingColor";
	}

	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TANGENT_ELEMENT | VERTEX_TEXTURE_ELEMENT_0)) { 
		*ppszVSShaderName = "VSTexturedLightingTangent", *ppszPSShaderName = "PSTexturedLightingTangent";
	}
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_TEXTURE_ELEMENT_1)) { 
		*ppszVSShaderName = "VSDetailTexturedLightingColor", *ppszPSShaderName = "PSDetailTexturedLightingColor"; 
	}
	// Skinned
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_BONE_ID_ELEMENT | VERTEX_BONE_WEIGHT_ELEMENT)) { 
		*ppszVSShaderName = "VSSkinnedLightingColor", *ppszPSShaderName = "PSLightingColor";
	}
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_BONE_ID_ELEMENT | VERTEX_BONE_WEIGHT_ELEMENT)) {
		*ppszVSShaderName = "VSSkinnedTexturedLightingColor", *ppszPSShaderName = "PSSkinnedTexturedLightingColor"; 
	}
	// Instancing
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_INSTANCING_ELEMENT)) {
		*ppszVSShaderName = "VSInstancedLightingColor", *ppszPSShaderName = "PSInstancedLightingColor";
	}
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_INSTANCING_ELEMENT)) { 
		*ppszVSShaderName = "VSInstancedTexturedLightingColor", *ppszPSShaderName = "PSInstancedTexturedLightingColor";
	}

	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TANGENT_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_INSTANCING_ELEMENT)) {
		*ppszVSShaderName = "VSInstancedTexturedTangentLighting", *ppszPSShaderName = "PSInstancedTexturedTangentLighting";
	}
}

void CShader::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, nullptr, 0);
	pd3dDeviceContext->GSSetShader(m_pd3dGeometryShader, nullptr, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, nullptr, 0);
}

void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender(pd3dDeviceContext);
}

void CShader::OnPostRender(ID3D11DeviceContext *pd3dDeviceContext)
{
}

ID3D11Buffer *CShader::CreateBuffer(ID3D11Device *pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = d3dUsage;
	d3dBufferDesc.ByteWidth = nStride * nElements;
	d3dBufferDesc.BindFlags = nBindFlags;
	d3dBufferDesc.CPUAccessFlags = nCPUAccessFlags;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pBufferData;

	ID3D11Buffer *pd3dInstanceBuffer = NULL;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, (pBufferData) ? &d3dBufferData : NULL, &pd3dInstanceBuffer);

	DXUT_SetDebugName(pd3dInstanceBuffer, "Instance");
	return(pd3dInstanceBuffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
CObjectsShader::CObjectsShader()
{
}

CObjectsShader::~CObjectsShader()
{
}

void CObjectsShader::AddObject(ShaderTag tag, CGameObject *pGameObject)
{
	auto findShader = m_mapObjectContainer.find(tag);
	if (findShader != m_mapObjectContainer.end()) {
		findShader->second.push_back(pGameObject);
		return;
	}

	// Not Found Shader - new ShaderContainer
	vector<CGameObject*> vecObjectContainer;
	vecObjectContainer.reserve(100);
	vecObjectContainer.push_back(pGameObject);
	pGameObject->SetShaderTag(tag);
	
	CShader *pShader;

	switch (tag) {
	case ShaderTag::eDisplacementMapping:
		pShader = new CNormalDisplaceMapShader();
		pShader->CreateShader(m_pd3dDevice);
		break;
		
	case ShaderTag::eInstanceDisplacementMapping:
		pShader = new CNormalDisplaceMapInstancedShader();
		pShader->CreateShader(m_pd3dDevice);
		break;
	default:
		pShader = new CShader();
		pShader->CreateShader(m_pd3dDevice, tag);
		break;
	}

	m_mapShaderContainer.insert(make_pair(tag, pShader));
	m_mapObjectContainer.insert(make_pair(tag, vecObjectContainer));
}

void CObjectsShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
}

void CObjectsShader::ReleaseObjects()
{
	for (auto& shaderTag : m_mapObjectContainer) {
		for (auto& vecObject : shaderTag.second)
			SafeDelete(vecObject);
	}

	for (auto& shader : m_mapShaderContainer)
		SafeDelete(shader.second);
}

void CObjectsShader::UpdateObjects(float fDeltaTime)
{
	for (auto shaderTag : m_mapObjectContainer) {
		for (auto vecObject : shaderTag.second) {
			if (vecObject->GetActive())
				vecObject->Update(fDeltaTime);
		}
	}
}

void CObjectsShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	for (auto shaderTag : m_mapObjectContainer) {
		auto findShader = m_mapShaderContainer.find(shaderTag.first);
		findShader->second->OnPrepareRender(pd3dDeviceContext);

		for (auto vecObject : shaderTag.second) {
			if (vecObject->IsVisible(pCamera))
				vecObject->Render(pd3dDeviceContext, pCamera);
		}
		findShader->second->OnPostRender(pd3dDeviceContext);
	}
}

CGameObject *CObjectsShader::PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, CollisionInfo *pd3dxIntersectInfo)
{
	int nIntersected = 0;
	float fNearHitDistance = FLT_MAX;
	CGameObject *pSelectedObject = NULL;
	CollisionInfo d3dxIntersectInfo;


	for (auto shaderTag : m_mapObjectContainer) {
		for (auto& vecObject : shaderTag.second) {

			nIntersected = vecObject->PickObjectByRayIntersection(pd3dxvPickPosition, pd3dxmtxView, &d3dxIntersectInfo);
			if ((nIntersected > 0) && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
			{
				fNearHitDistance = d3dxIntersectInfo.m_fDistance;
				pSelectedObject = vecObject;
				if (pd3dxIntersectInfo) *pd3dxIntersectInfo = d3dxIntersectInfo;
			}

		}
	}
	return(pSelectedObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

CInstancedObjectsShader::CInstancedObjectsShader(int capacity)
	: m_nCapacity(capacity)
{
	m_nInstanceBufferStride = sizeof(XMMATRIX);
	m_nInstanceBufferOffset = 0;
	m_pd3dInstanceBuffer = NULL;

	m_nType = VERTEX_INSTANCING_ELEMENT;
}

CInstancedObjectsShader::~CInstancedObjectsShader()
{
	SafeDelete(m_pMaterial);
}

void CInstancedObjectsShader::SetMesh(CMesh *pMesh)
{
	if(m_pMesh) 
		m_pMesh->Release();
	m_pMesh = pMesh;
	m_pMesh->AddRef();
}

void CInstancedObjectsShader::SetMaterial(int textureCount, ...)
{
	SafeDelete(m_pMaterial);
	CMaterial* pMaterial = new CMaterial();
	vector<TextureTag> vecTextureTag;

	va_list ap;
	va_start(ap, textureCount);

	for (int i = 0; i < textureCount; ++i)
		vecTextureTag.push_back(va_arg(ap, TextureTag));

	va_end(ap);

	CTexture* pTexture = new CTexture(textureCount, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT_ANISOTROPIC_WRAP);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pAnisotropicWrapSS);		// 제일 고품질 Sampler State 사용

	for (int i = 0; i < textureCount; ++i)
		pTexture->SetTexture(i, vecTextureTag[i]);

	pMaterial->SetTexture(pTexture);

	m_pMaterial = pMaterial;
}

void CInstancedObjectsShader::SetMaterial(XMFLOAT2 offset, int textureCount, ...)
{
	SafeDelete(m_pMaterial);
	CMaterial* pMaterial = new CMaterial();
	vector<TextureTag> vecTextureTag;

	va_list ap;
	va_start(ap, textureCount);

	for (int i = 0; i < textureCount; ++i)
		vecTextureTag.push_back(va_arg(ap, TextureTag));

	va_end(ap);

	CTexture* pTexture = new CTexture(textureCount, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWrapSS);
	pTexture->SetOffset(offset);

	for (int i = 0; i < textureCount; ++i)
		pTexture->SetTexture(i, vecTextureTag[i]);

	pMaterial->SetTexture(pTexture);

	m_pMaterial = pMaterial;
}

void CInstancedObjectsShader::CreateShader(ID3D11Device *pd3dDevice)
{
	//if (m_pMesh) CObjectsShader::CreateShader(pd3dDevice, m_pMesh->GetType());
	if (m_pMesh) 
		CShader::CreateShader(pd3dDevice);
}

void CInstancedObjectsShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	CObjectsShader::BuildObjects(pd3dDevice);

	m_pd3dInstanceBuffer = CreateBuffer(pd3dDevice, m_nInstanceBufferStride, m_nCapacity, NULL, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_pMesh->AssembleToVertexBuffer(1, &m_pd3dInstanceBuffer, &m_nInstanceBufferStride, &m_nInstanceBufferOffset);

	// Create Bounding Box
	m_pBoundingBoxMesh = new CBoundingBoxMesh(pd3dDevice, m_pMesh->GetBoundingCube());
	m_pBoundingBoxShader = new CBoundingBoxShader();
	m_pBoundingBoxShader->CreateShader(pd3dDevice);
}

void CInstancedObjectsShader::ReleaseObjects()
{
	CObjectsShader::ReleaseObjects();

	m_pMesh->Release();
	SafeDelete(m_pBoundingBoxMesh);
	SafeDelete(m_pBoundingBoxShader);
	ReleaseCOM(m_pd3dInstanceBuffer);
}

void CInstancedObjectsShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	for (auto shaderTag : m_mapObjectContainer) {
		auto findShader = m_mapShaderContainer.find(shaderTag.first);

		if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pd3dDeviceContext);
		findShader->second->OnPrepareRender(pd3dDeviceContext);

		int nInstances = 0;
		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
		pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
		XMMATRIX *pMtxInstances = (XMMATRIX *)d3dMappedResource.pData;

		for (auto object : shaderTag.second) {
			if (object->IsVisible(pCamera))
				pMtxInstances[nInstances++] = XMMatrixTranspose(object->m_mtxWorld);
		}
		pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

		m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);

		findShader->second->OnPostRender(pd3dDeviceContext);
	}

	// Bounding Box Rendering
	if (GLOBAL_MGR->g_vRenderOption.y) {
		pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pWireframeRS);

		for (auto shaderTag : m_mapObjectContainer) {
			for (auto object : shaderTag.second) {
				if (object->IsVisible(pCamera)) {
					m_pBoundingBoxShader->OnPrepareSetting(pd3dDeviceContext, object->GetCollisionCheck());

					XMMATRIX mtxBoundingBoxWorld = object->m_mtxWorld;
					BoundingOrientedBox bcObbox = object->GetBoundingOBox();
					
					mtxBoundingBoxWorld = XMMatrixRotationQuaternion(XMLoadFloat4(&bcObbox.Orientation)) *
						XMMatrixTranslation(bcObbox.Center.x, bcObbox.Center.y, bcObbox.Center.z);

					CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &mtxBoundingBoxWorld);

					m_pBoundingBoxMesh->Render(pd3dDeviceContext);
				}
			}
		}
		pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);
	}
}