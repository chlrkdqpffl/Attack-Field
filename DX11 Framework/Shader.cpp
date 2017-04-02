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
	m_nReferences = 0;

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

void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputElements, UINT nElements, ID3D11InputLayout **ppd3dInputLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dVertexShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreateVertexShader(pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		pd3dDevice->CreateInputLayout(pd3dInputElements, nElements, pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize(), ppd3dInputLayout);
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
			MessageBox(NULL, L"ShaderName is NULL!!", pszFileName, MB_OK);
	}
}

void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dPixelShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dPixelShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreatePixelShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dPixelShaderBlob->Release();
		DXUT_SetDebugName(*ppd3dPixelShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", pszFileName, MB_OK);
	}
}

void CShader::CreateGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dPixelShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dPixelShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreateGeometryShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), NULL, ppd3dGeometryShader);
		pd3dPixelShaderBlob->Release();

		DXUT_SetDebugName(*ppd3dGeometryShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", pszFileName, MB_OK);
	}
}

void CShader::CreateComputeShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11ComputeShader **ppd3dComputeShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreateComputeShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dComputeShader);
		pd3dShaderBlob->Release();

		DXUT_SetDebugName(*ppd3dComputeShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", pszFileName, MB_OK);
	}
}

void CShader::CreateHullShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11HullShader **ppd3dHullShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreateHullShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dHullShader);
		pd3dShaderBlob->Release();
		DXUT_SetDebugName(*ppd3dHullShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", pszFileName, MB_OK);
	}
}

void CShader::CreateDomainShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11DomainShader **ppd3dDomainShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreateDomainShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dDomainShader);
		pd3dShaderBlob->Release();
		DXUT_SetDebugName(*ppd3dDomainShader, pszShaderName);
	}
	else {
		if (pszShaderName) {
			WCHAR* wstr = ConverCtoWC(pszShaderName);
			MessageBox(NULL, wstr, L"Shader Error !", MB_OK);
		}
		else
			MessageBox(NULL, L"ShaderName is NULL!!", pszFileName, MB_OK);
	}
}

void CShader::CreateShader(ID3D11Device *pd3dDevice, UINT nType)
{
	m_nType |= nType;
	GetInputElementDesc(m_nType);
	LPCSTR pszVSShaderName = NULL, pszVSShaderModel = "vs_5_0", pszPSShaderName = NULL, pszPSShaderModel = "ps_5_0";
	GetShaderName(m_nType, &pszVSShaderName, &pszVSShaderModel, &pszPSShaderName, &pszPSShaderModel);
	CreateShader(pd3dDevice, NULL, 0, L"Shader HLSL File/Effect.fx", pszVSShaderName, pszVSShaderModel, pszPSShaderName, pszPSShaderModel);
}

void CShader::CreateShader(ID3D11Device *pd3dDevice, D3D11_INPUT_ELEMENT_DESC *pd3dInputElementDesc, int nInputElements, WCHAR *pszFileName, LPCSTR pszVSShaderName, LPCSTR pszVSShaderModel, LPCSTR pszPSShaderName, LPCSTR pszPSShaderModel)
{
	CreateVertexShaderFromFile(pd3dDevice, pszFileName, pszVSShaderName, pszVSShaderModel, &m_pd3dVertexShader, (pd3dInputElementDesc) ? pd3dInputElementDesc : m_pd3dInputElementDescs, (pd3dInputElementDesc) ? nInputElements : m_nInputElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, pszFileName, pszPSShaderName, pszPSShaderModel, &m_pd3dPixelShader);
}

void CShader::CreateShader(ID3D11Device *pd3dDevice, ShaderTag shaderTag)
{
	cout << "여기 들어오면 안됨" << endl;
	if (shaderTag == eShader_NormalMap) {
		// 노말맵 하고나서 구현해보기
	}
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
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_COLOR_ELEMENT)) { *ppszVSShaderName = "VSDiffusedColor", *ppszPSShaderName = "PSDiffusedColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT)) { *ppszVSShaderName = "VSLightingColor", *ppszPSShaderName = "PSLightingColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0)) { *ppszVSShaderName = "VSTexturedColor", *ppszPSShaderName = "PSTexturedColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_TEXTURE_ELEMENT_1)) { *ppszVSShaderName = "VSDetailTexturedColor", *ppszPSShaderName = "PSDetailTexturedColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0)) { *ppszVSShaderName = "VSTexturedLightingColor", *ppszPSShaderName = "PSTexturedLightingColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TANGENT_ELEMENT | VERTEX_TEXTURE_ELEMENT_0)) { *ppszVSShaderName = "VSTexturedLightingTangent", *ppszPSShaderName = "PSTexturedLightingTangent"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_TEXTURE_ELEMENT_1)) { *ppszVSShaderName = "VSDetailTexturedLightingColor", *ppszPSShaderName = "PSDetailTexturedLightingColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_BONE_ID_ELEMENT | VERTEX_BONE_WEIGHT_ELEMENT)) { *ppszVSShaderName = "VSSkinnedLightingColor", *ppszPSShaderName = "PSLightingColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_BONE_ID_ELEMENT | VERTEX_BONE_WEIGHT_ELEMENT)) { *ppszVSShaderName = "VSSkinnedTexturedLightingColor", *ppszPSShaderName = "PSSkinnedTexturedLightingColor"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_INSTANCING_ELEMENT)) { *ppszVSShaderName = "VSInstancedTexturedLightingColor", *ppszPSShaderName = "PSInstancedTexturedLightingColor"; }
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
CObjectsShader::CObjectsShader(int nObjects)
{
	m_vecObjectsContainer.reserve(nObjects);

	m_pMaterial = NULL;
	m_pContext = NULL;
}

CObjectsShader::~CObjectsShader()
{
}

void CObjectsShader::SetObject(int nIndex, CGameObject *pGameObject)
{
	if (m_vecObjectsContainer[nIndex]) m_vecObjectsContainer[nIndex]->Release();
	m_vecObjectsContainer[nIndex] = pGameObject;
	if (pGameObject) pGameObject->AddRef();
}

void CObjectsShader::AddObject(CGameObject *pGameObject)
{
	m_vecObjectsContainer.push_back(pGameObject);
	if (pGameObject) pGameObject->AddRef();
}

void CObjectsShader::SetMaterial(CMaterial *pMaterial)
{
	m_pMaterial = pMaterial;
	if (pMaterial) pMaterial->AddRef();
}

void CObjectsShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	m_pContext = pContext;
}

void CObjectsShader::ReleaseObjects()
{
	ReleaseCOM(m_pMaterial);

	for (auto& object : m_vecObjectsContainer)
		ReleaseCOM(object);

	m_vecObjectsContainer.clear();
}

void CObjectsShader::UpdateObjects(float fTimeElapsed)
{
	for (auto object : m_vecObjectsContainer) {
		if(object->GetActive())
			object->Update(fTimeElapsed);
	}
}

void CObjectsShader::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	CShader::OnPrepareRender(pd3dDeviceContext);
}

void CObjectsShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	for (auto object : m_vecObjectsContainer) {
		if (object->IsVisible(pCamera)) {
			CShader::OnPrepareRender(pd3dDeviceContext);

			if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pd3dDeviceContext);
			object->Render(pd3dDeviceContext, pCamera);

			CShader::OnPostRender(pd3dDeviceContext);
		}
	}
}

CGameObject *CObjectsShader::PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo)
{
	int nIntersected = 0;
	float fNearHitDistance = FLT_MAX;
	CGameObject *pSelectedObject = NULL;
	MESHINTERSECTINFO d3dxIntersectInfo;

	for (auto object : m_vecObjectsContainer) {

		nIntersected = object->PickObjectByRayIntersection(pd3dxvPickPosition, pd3dxmtxView, &d3dxIntersectInfo);
		if ((nIntersected > 0) && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pSelectedObject = object;
			if (pd3dxIntersectInfo) *pd3dxIntersectInfo = d3dxIntersectInfo;
		}

	}

	return(pSelectedObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

CInstancedObjectsShader::CInstancedObjectsShader(int nObjects) : CObjectsShader(nObjects)
{
	m_nInstanceBufferStride = sizeof(XMMATRIX);
	m_nInstanceBufferOffset = 0;
	m_pd3dInstanceBuffer = NULL;

	m_pMesh = NULL;

	m_nType = VERTEX_INSTANCING_ELEMENT;
}

CInstancedObjectsShader::~CInstancedObjectsShader()
{
}

void CInstancedObjectsShader::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void CInstancedObjectsShader::CreateShader(ID3D11Device *pd3dDevice)
{
	if (m_pMesh) CreateShader(pd3dDevice, m_pMesh->GetType());
}

void CInstancedObjectsShader::CreateShader(ID3D11Device *pd3dDevice, UINT nType)
{
	CObjectsShader::CreateShader(pd3dDevice, nType);
}

void CInstancedObjectsShader::BuildObjects(ID3D11Device *pd3dDevice, void *pContext)
{
	CObjectsShader::BuildObjects(pd3dDevice, pContext);

	m_pd3dInstanceBuffer = CreateBuffer(pd3dDevice, m_nInstanceBufferStride, (int)m_vecObjectsContainer.capacity(), NULL, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_pMesh->AssembleToVertexBuffer(1, &m_pd3dInstanceBuffer, &m_nInstanceBufferStride, &m_nInstanceBufferOffset);


	m_pBoundingBoxMesh = new CBoundingBoxMesh(pd3dDevice, m_pMesh->GetBoundingCube());
	m_pBoundingBoxShader = new CBoundingBoxShader();
//	m_pBoundingBoxShader = new CBoundingBoxInstancedShader();
	m_pBoundingBoxShader->CreateShader(pd3dDevice);
//	m_pBoundingBoxMesh->AssembleToVertexBuffer(1, &m_pd3dInstanceBuffer, &m_nInstanceBufferStride, &m_nInstanceBufferOffset);
}

void CInstancedObjectsShader::ReleaseObjects()
{
	CObjectsShader::ReleaseObjects();

	ReleaseCOM(m_pMesh);
	SafeDelete(m_pBoundingBoxMesh);
	SafeDelete(m_pBoundingBoxShader);
	ReleaseCOM(m_pd3dInstanceBuffer);
}

void CInstancedObjectsShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender(pd3dDeviceContext);
	
	if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pd3dDeviceContext);

	int nInstances = 0;
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMMATRIX *pd3dxmtxInstances = (XMMATRIX *)d3dMappedResource.pData;

	for (auto object : m_vecObjectsContainer) {
		if (object->IsVisible(pCamera))
			pd3dxmtxInstances[nInstances++] = XMMatrixTranspose(object->m_mtxWorld);
	}

	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstances, 0);


	// Bounding Box Rendering
	if (GLOBAL_MGR->g_vRenderOption.y) {
		pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pWireframeRS);

		for (auto object : m_vecObjectsContainer) {
			if (object->IsVisible(pCamera)) {
				m_pBoundingBoxShader->OnPrepareSetting(pd3dDeviceContext,object->GetCollisionCheck());
				CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &object->m_mtxWorld);

				m_pBoundingBoxMesh->Render(pd3dDeviceContext);
			}
		}
		pd3dDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);
	}
	
	CShader::OnPostRender(pd3dDeviceContext);
}