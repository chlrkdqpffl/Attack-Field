#include "stdafx.h"
#include "DecalManager.h"

const UINT CDecalManager::m_nVertStride = 8 * sizeof(float);
const UINT CDecalManager::m_nMaxDecalVerts = 256;

/*
void CDecalManager::InitializeManager()
{
	ID3D11Device* pd3dDevice = STATEOBJ_MGR->g_pd3dDevice;

	D3D11_BUFFER_DESC CBDesc;
	ZeroMemory(&CBDesc, sizeof(CBDesc));
	CBDesc.Usage = D3D11_USAGE_DYNAMIC;
	CBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CBDesc.ByteWidth = sizeof(D3DXMATRIX);
	HR(pd3dDevice->CreateBuffer(&CBDesc, NULL, &m_pDecalGenVSCB));
	DXUT_SetDebugName(m_pDecalGenVSCB, "Generate Decal VS CB");

	CBDesc.ByteWidth = sizeof(TDecalGenCB);
	HR(pd3dDevice->CreateBuffer(&CBDesc, NULL, &m_pDecalGenGSCB));
	DXUT_SetDebugName(m_pDecalGenGSCB, "Generate Decal GS CB");

	
	// --------------------- Create Shader -------------------------- //
	wstring strFileName;
	// Directional Light Shader
	strFileName = L"../Shader HLSL File/DecalGen.hlsli";
	CShader::CreateVertexShaderFromFile(pd3dDevice, strFileName, "DecalGenVS", "vs_5_0", &m_pDecalGenVS);
	CShader::CreateGeometryShaderFromFile(pd3dDevice, strFileName, "DecalGenGS", "gs_5_0", &m_pDecalGenGS);
	CreateSOGeometryShaderFromFile(pd3dDevice, strFileName, "DecalGenGS", "gs_5_0", &m_pDecalGenGS);
	
	strFileName = L"../Shader HLSL File/DeferredShading.hlsli";
	CShader::CreateVertexShaderFromFile(pd3dDevice, strFileName, "RenderSceneVS", "vs_5_0", &m_pDecalRenderVS);
	CShader::CreatePixelShaderFromFile(pd3dDevice, strFileName, "RenderDecalPS", "ps_5_0", &m_pDecalRenderPS);

	// Create the VB for the decals
	static const UINT nDecalBufferSize = m_nMaxDecalVerts * m_nVertStride;
	D3D11_BUFFER_DESC VBDesc =
	{
		nDecalBufferSize,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT,
		0,
		0
	};
	HR(pd3dDevice->CreateBuffer(&VBDesc, NULL, &m_pDecalVB));

	// Create the querys in order to keep track of what got drawn so far
	D3D11_QUERY_DESC StatsDesc;
	StatsDesc.Query = D3D11_QUERY_SO_STATISTICS;
	StatsDesc.MiscFlags = 0;
	HR(pd3dDevice->CreateQuery(&StatsDesc, &m_pStatsQuery));
}

void CDecalManager::ReleseManager()
{
	ReleaseCOM(m_pDecalGenVSCB);
	ReleaseCOM(m_pDecalGenGSCB);
	ReleaseCOM(m_pDecalGenVS);
	ReleaseCOM(m_pDecalGenGS);
	ReleaseCOM(m_pStatsQuery);
	ReleaseCOM(m_pDecalVB);
	ReleaseCOM(m_pDecalRenderCB);
	ReleaseCOM(m_pDecalRenderVS);
	ReleaseCOM(m_pDecalRenderPS);
}

void CDecalManager::CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, const wstring& pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;


	D3D11_SO_DECLARATION_ENTRY pStreamOutDecl[] =
	{
		// stream, name, index, start component, component count, output slot
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "NORMAL", 0, 0, 3, 0 },
		{ 0,"TEXCOORD", 0, 0, 2, 0 },
	};
	UINT arrBufferStride[1] = { m_nVertStride };
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;

	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName.c_str(), NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreateGeometryShaderWithStreamOutput(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(),
			pStreamOutDecl, 3, arrBufferStride, 1, 0, NULL, ppd3dGeometryShader);

		pd3dShaderBlob->Release();

		DXUT_SetDebugName(*ppd3dGeometryShader, "ParticleGSWithSO");
	}
	else
	{
		MessageBox(NULL, L"SOGSERROR", L"SOGSERROR", MB_OK);
		exit(0);
	}
}

void CDecalManager::AddDecal(const XMVECTOR& vHitPos, const XMVECTOR& vHitNorm, CMesh& mesh, UINT nMesh, UINT nSubMesh)
{
	// Check that this decal is not too close to a previous decal or pending decals
	bool bTooClose = false;
	for (std::list<TDecalEntry>::iterator itr = m_DecalList.begin(); itr != m_DecalList.end(); itr++) {
		XMVECTOR vAToB = XMLoadFloat3(&(*itr).vHitPos) - vHitPos;

		if(XMVectorGetX(XMVector3LengthSq(vAToB)) < m_fDecalSize * m_fDecalSize) {
			bTooClose = true;
			break;
		}
	}
	if (!bTooClose) {
		for (std::list<TDecalAddEntry>::iterator itr = m_DecalAddList.begin(); itr != m_DecalAddList.end(); itr++) {
			XMVECTOR vAToB = XMLoadFloat3(&(*itr).vHitPos) - vHitPos;
			if (XMVectorGetX(XMVector3LengthSq(vAToB)) < m_fDecalSize * m_fDecalSize) {
				bTooClose = true;
				break;
			}
		}
	}

	if (!bTooClose) {
		// Not too close - add to the pending list
		TDecalAddEntry decalAddEntry;
		XMStoreFloat3(&decalAddEntry.vHitPos, vHitPos);
		XMStoreFloat3(&decalAddEntry.vHitNorm, vHitNorm);
		decalAddEntry.pMesh = &mesh;
		decalAddEntry.nMesh = nMesh;
		decalAddEntry.nSubMesh = nSubMesh;
		decalAddEntry.bStarted = false;
		m_DecalAddList.push_back(decalAddEntry);
	}
}

void CDecalManager::RemoveDecalFromList()
{
	TDecalEntry& decalEntry = m_DecalList.front();
	m_nDecalStart1 += decalEntry.nVertCount;
	m_nTotalDecalVerts1 -= decalEntry.nVertCount;

	if (m_nTotalDecalVerts1 == 0)
	{
		m_nTotalDecalVerts1 = m_nTotalDecalVerts2;
		m_nDecalStart1 = 0;
		m_nTotalDecalVerts2 = 0;
	}

	m_DecalList.pop_front();
}

void CDecalManager::PreRender(ID3D11DeviceContext* pd3dImmediateContext)
{
	// Check if there is a hit pending to be added to the list
	if (!m_DecalAddList.empty())
	{
		TDecalAddEntry& decalAddEntry = m_DecalAddList.front();

		if (!decalAddEntry.bStarted)
		{
			PrepareGenConstBuffer(pd3dImmediateContext, decalAddEntry.vHitPos, decalAddEntry.vHitNorm);
			AddDecalToVB(pd3dImmediateContext, *decalAddEntry.pMesh, decalAddEntry.nMesh, decalAddEntry.nSubMesh, false);
			decalAddEntry.bStarted = true;
		}
		else
		{
			// See if the querys are ready (don't block)
			D3D11_QUERY_DATA_SO_STATISTICS soStates;
			if (pd3dImmediateContext->GetData(m_pStatsQuery, &soStates, sizeof(soStates), 0) == S_OK)
			{
				if (soStates.NumPrimitivesWritten < soStates.PrimitivesStorageNeeded)
				{
					// There wasnt enoug room for all the triangles in the last decal added
					// We get around this by adding the decal again at the beginning of the buffer

					// Make sure there is room for the new decal
					while (m_nTotalDecalVerts2 != 0 || (m_nTotalDecalVerts1 != 0 && m_nDecalStart1 < 3 * (UINT)soStates.PrimitivesStorageNeeded))
					{
						RemoveDecalFromList();
					}

					// Add the new decal again at the start of the buffer
					//PrepareGenConstBuffer(pd3dImmediateContext, decalEntry.vHitPos, decalEntry.vHitNorm);
					AddDecalToVB(pd3dImmediateContext, *decalAddEntry.pMesh, decalAddEntry.nMesh, decalAddEntry.nSubMesh, true);
				}
				else
				{
					// Add a new active decal entry only if anything was written
					if (soStates.NumPrimitivesWritten > 0)
					{
						// Keep track over the amount of triangles added
						TDecalEntry decalEntry;
						decalEntry.nVertCount = 3 * (UINT)soStates.NumPrimitivesWritten;
						if (m_nTotalDecalVerts2 > 0 || (m_nDecalStart1 + m_nTotalDecalVerts1 + decalEntry.nVertCount) > m_nMaxDecalVerts)
						{
							m_nTotalDecalVerts2 += decalEntry.nVertCount;
						}
						else
						{
							m_nTotalDecalVerts1 += decalEntry.nVertCount;
						}

						// Check if its time to remove some decals
						while (m_nTotalDecalVerts2 > m_nDecalStart1 || (m_nTotalDecalVerts1 + m_nTotalDecalVerts2) > m_nMaxDecalVerts)
						{
							RemoveDecalFromList();
						}

						decalEntry.vHitPos = decalAddEntry.vHitPos;
						m_DecalList.push_back(decalEntry);
					}

					// Remove the pending entry
					m_DecalAddList.pop_front();
				}
			}
		}
	}
}

void CDecalManager::Render(ID3D11DeviceContext* pd3dImmediateContext, bool bWireframe)
{
	// Render the decals if there is anything in the buffer
	if (m_nTotalDecalVerts1 > 0 || m_nTotalDecalVerts2 > 0) {
		ID3D11RasterizerState* pPrevRSState;
		pd3dImmediateContext->RSGetState(&pPrevRSState);
		pd3dImmediateContext->RSSetState(bWireframe ? STATEOBJ_MGR->g_pWireframeRS : STATEOBJ_MGR->g_pDefaultRS);
		pd3dImmediateContext->OMSetBlendState(STATEOBJ_MGR->g_pDecalBS, NULL, 0xffffffff);

		// Get the projection & view matrix from the camera class
		XMMATRIX mWorld; mWorld = XMMatrixIdentity();
		XMMATRIX mView = SCENE_MGR->g_pCamera->GetViewMatrix();
		XMMATRIX mProj = SCENE_MGR->g_pCamera->GetProjectionMatrix();
		XMMATRIX mWorldViewProjection = mWorld * mView * mProj;

		D3D11_MAPPED_SUBRESOURCE MappedResource;
		HR(pd3dImmediateContext->Map(m_pDecalRenderCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
		TDecalRenderCB* pDecalRenderCB = (TDecalRenderCB*)MappedResource.pData;
		D3DXMatrixTranspose(&pDecalRenderCB->matWorldViewProj, &mWorldViewProjection);
		D3DXMatrixTranspose(&pDecalRenderCB->matWorld, &mWorld);
		pd3dImmediateContext->Unmap(m_pDecalRenderCB, 0);
		ID3D11Buffer* arrConstBuffers[1] = { m_pDecalRenderCB };
		pd3dImmediateContext->VSSetConstantBuffers(0, 1, arrConstBuffers);

		// Set the texture
		pd3dImmediateContext->PSSetShaderResources(0, 1, bWireframe ? &m_pWhiteTexView : &m_pDecalTexView);

		// Set the Vertex Layout
		g_SceneManager.SetSceneInputLayout(pd3dImmediateContext);

		pd3dImmediateContext->VSSetShader(m_pDecalRenderVS, NULL, 0);
		pd3dImmediateContext->PSSetShader(m_pDecalRenderPS, NULL, 0);

		// Set the decal VB
		ID3D11Buffer* pVB[1] = { m_pDecalVB };
		UINT offset[1] = { 0 };
		UINT stride[1] = { m_nVertStride };
		pd3dImmediateContext->IASetVertexBuffers(0, 1, pVB, stride, offset);
		pd3dImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Draw all the triangles from the start of the buffer up to the last one added
		pd3dImmediateContext->DrawAuto();

		// If there are older triangles at the end of the buffer, render them too
		if (m_nTotalDecalVerts2 > 0)
		{
			pd3dImmediateContext->Draw(m_nTotalDecalVerts1, m_nDecalStart1);
		}

		pd3dImmediateContext->VSSetShader(NULL, NULL, 0);
		pd3dImmediateContext->PSSetShader(NULL, NULL, 0);

		pd3dImmediateContext->RSSetState(pPrevRSState);
		ReleaseCOM(pPrevRSState);
		pd3dImmediateContext->OMSetBlendState(NULL, NULL, 0xffffffff);
	}
}


void CDecalManager::UpdateManager()
{

}
*/