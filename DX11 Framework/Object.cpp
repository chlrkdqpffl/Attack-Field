//-----------------------------------------------------------------------------
// File: CGameObject.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Object.h"
#include "Shader.h"
#include "BoundingBoxMesh.h"
#include "BoundingBoxShader.h"
#include "AxisObjects.h"

///////////////////////////////////////////////////////////////////////////

ID3D11Buffer*	CGameObject::m_pd3dcbWorldMatrix = NULL;
ID3D11Buffer*	CGameObject::m_pd3dcbMaterialColors = NULL;
UINT			CGameObject::g_nObjectId = 0;

CGameObject::CGameObject(int nMeshes)
{
//	m_nMeshes = nMeshes;
//	if (m_nMeshes > 0) m_ppMeshes = new CMesh*[m_nMeshes];
//	for (int i = 0; i < m_nMeshes; i++)	m_ppMeshes[i] = NULL;

	m_bcMeshBoundingBox.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bcMeshBoundingBox.Extents = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_nObjectId = ++g_nObjectId;
}

CGameObject::~CGameObject()
{
	ReleaseCOM(m_pd3dDepthStencilState);
	ReleaseCOM(m_pd3dBlendState);
	
	SafeDelete(m_pShader);
	SafeDelete(m_pMaterial);

	for (auto& mesh : m_vecMeshContainer)
		mesh->Release();

	m_vecMeshContainer.clear();
	SafeDelete(m_pBoundingBoxMesh);
	SafeDelete(m_pAxisObject);
}

void CGameObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CreateMesh(pd3dDevice);
	CreateShader(pd3dDevice);
	CreateMaterial();
	CreateBoundingBox(pd3dDevice);
}

void CGameObject::CreateBoundingBox(ID3D11Device *pd3dDevice)
{
	m_pBoundingBoxMesh = new CBoundingBoxMesh(pd3dDevice, m_bcMeshBoundingBox);
}

void CGameObject::CreateBoundingBox(ID3D11Device *pd3dDevice, CMesh* pMesh)
{
	if (pMesh) {
		BoundingBox::CreateMerged(m_bcMeshBoundingBox, m_bcMeshBoundingBox, pMesh->GetBoundingCube());
		BoundingSphere::CreateFromBoundingBox(m_bsMeshBoundingSphere, m_bcMeshBoundingBox);
		BoundingOrientedBox::CreateFromBoundingBox(m_bcMeshBoundingOBox, m_bcMeshBoundingBox);
	}
	m_pBoundingBoxMesh = new CBoundingBoxMesh(pd3dDevice, m_bcMeshBoundingBox);
}

void CGameObject::CreateAxisObject(ID3D11Device *pd3dDevice)
{
	m_pAxisObject = new CAxisObjects(this);
	m_pAxisObject->CreateAxis(pd3dDevice);
}

void CGameObject::SetMesh(CMesh *pMesh, int nIndex)
{
	pMesh->AddRef();

	if (nIndex == 0)
		m_tagMesh = pMesh->GetMeshTag();

	m_vecMeshContainer.push_back(pMesh);

	BoundingBox::CreateMerged(m_bcMeshBoundingBox, m_bcMeshBoundingBox, pMesh->GetBoundingCube());
	BoundingSphere::CreateFromBoundingBox(m_bsMeshBoundingSphere, m_bcMeshBoundingBox);
	BoundingOrientedBox::CreateFromBoundingBox(m_bcMeshBoundingOBox, m_bcMeshBoundingBox);
}

void CGameObject::SetShader(CShader *pShader)
{
	SafeDelete(m_pShader)
	m_tagShader = pShader->GetShaderTag();
	m_pShader = pShader;
	
}

void CGameObject::SetShader(ID3D11Device *pd3dDevice, ShaderTag tag)
{
	SafeDelete(m_pShader)
	CShader* pShader = new CShader();
	pShader->CreateShader(pd3dDevice, tag);
	m_tagShader = tag;
	m_pShader = pShader;
	
}

void CGameObject::SetMaterial(CMaterial *pMaterial)
{
	SafeDelete(m_pMaterial);
	m_tagTexture = pMaterial->GetTextureTag();
	m_pMaterial = pMaterial;
	
}

void CGameObject::SetMaterial(int textureCount, ...)
{
	SafeDelete(m_pMaterial);
	CMaterial* pMaterial = new CMaterial();
	vector<TextureTag> vecTextureTag;

	va_list ap;
	va_start(ap, textureCount);

	for (int i = 0; i < textureCount; ++i)
		vecTextureTag.push_back(va_arg(ap, TextureTag));

	va_end(ap);

	m_tagTexture = vecTextureTag[0];

	CTexture* pTexture = new CTexture(textureCount, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);
	//	pTexture->SetSampler(0, STATEOBJ_MGR->g_pPointWrapSS);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWrapSS);

	for (int i = 0; i < textureCount; ++i)
		pTexture->SetTexture(i, vecTextureTag[i]);

	pMaterial->SetTexture(pTexture);

	m_pMaterial = pMaterial;
}

void CGameObject::SetMaterial(XMFLOAT2 offset, int textureCount, ...)
{
	SafeDelete(m_pMaterial);
	CMaterial* pMaterial = new CMaterial();
	vector<TextureTag> vecTextureTag;

	va_list ap;
	va_start(ap, textureCount);

	for (int i = 0; i < textureCount; ++i)
		vecTextureTag.push_back(va_arg(ap, TextureTag));

	va_end(ap);

	m_tagTexture = vecTextureTag[0];

	CTexture* pTexture = new CTexture(textureCount, 1, PS_TEXTURE_SLOT_DIFFUSE, PS_SAMPLER_SLOT);
	pTexture->SetSampler(0, STATEOBJ_MGR->g_pLinearWrapSS);
	pTexture->SetOffset(offset);

	for(int i = 0; i < textureCount; ++i) 
		pTexture->SetTexture(i, vecTextureTag[i]);

	pMaterial->SetTexture(pTexture);

	m_pMaterial = pMaterial;
}

void CGameObject::GenerateRayForPicking(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxWorld, XMMATRIX *pd3dxmtxView, XMVECTOR *pd3dxvPickRayPosition, XMVECTOR *pd3dxvPickRayDirection)
{
	XMMATRIX d3dxmtxInverse;
	XMMATRIX d3dxmtxWorldView = *pd3dxmtxView;
	if (pd3dxmtxWorld) 
		d3dxmtxWorldView = XMMatrixMultiply(*pd3dxmtxWorld, *pd3dxmtxView);
	d3dxmtxInverse = XMMatrixInverse(NULL, d3dxmtxWorldView);
	
	XMVECTOR d3dxvCameraOrigin = XMVectorZero();
	*pd3dxvPickRayPosition = XMVector3TransformCoord(d3dxvCameraOrigin, d3dxmtxInverse);
	*pd3dxvPickRayDirection = XMVector3TransformCoord(*pd3dxvPickPosition, d3dxmtxInverse);
	*pd3dxvPickRayDirection = *pd3dxvPickRayDirection - *pd3dxvPickRayPosition;
}

int CGameObject::PickObjectByRayIntersection(XMVECTOR *pd3dxvPickPosition, XMMATRIX *pd3dxmtxView, CollisionInfo *pd3dxIntersectInfo)
{
	XMVECTOR d3dxvPickRayPosition, d3dxvPickRayDirection;
	int nIntersected = 0;
	if (m_bActive && m_bIsVisible)
	{
		GenerateRayForPicking(pd3dxvPickPosition, &m_mtxWorld, pd3dxmtxView, &d3dxvPickRayPosition, &d3dxvPickRayDirection);

		for(auto& mesh : m_vecMeshContainer) {
			nIntersected = mesh->CheckRayIntersection(&d3dxvPickRayPosition, &d3dxvPickRayDirection, pd3dxIntersectInfo);
			if (nIntersected > 0) break;
		}
	}
	return(nIntersected);
}

void CGameObject::SetPosition(float x, float y, float z, bool isLocal)
{
	XMFLOAT4X4 mtx; 
	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._41 = x;  mtx._42 = y; 	mtx._43 = z;
		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._41 = x;  mtx._42 = y; 	mtx._43 = z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::SetPosition(XMVECTOR vPosition, bool isLocal)
{
	XMFLOAT3 f3vPosition;
	XMStoreFloat3(&f3vPosition, vPosition);
	SetPosition(f3vPosition, isLocal);
}

void CGameObject::SetPosition(XMFLOAT3 vPosition, bool isLocal)
{
	XMFLOAT4X4 mtx;
	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._41 = vPosition.x; mtx._42 = vPosition.y; mtx._43 = vPosition.z;
		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._41 = vPosition.x; mtx._42 = vPosition.y; mtx._43 = vPosition.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMVECTOR d3dxvPosition = GetvPosition();
	XMVECTOR d3dxvRight = GetvRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveUp(float fDistance)
{
	XMVECTOR d3dxvPosition = GetvPosition();
	XMVECTOR d3dxvUp = GetvUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveForward(float fDistance)
{
	XMVECTOR d3dxvPosition = GetvPosition();
	XMVECTOR d3dxvLookAt = GetvLook();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::Move(XMFLOAT3 vPos, bool isLocal)
{
	if (isLocal) {
		XMVECTOR d3dxvPosition = GetvPosition(true) + XMLoadFloat3(&vPos);
		CGameObject::SetPosition(d3dxvPosition, true);
	}
	else {
		XMVECTOR d3dxvPosition = GetvPosition() + XMLoadFloat3(&vPos);
		CGameObject::SetPosition(d3dxvPosition);
	}
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll, bool isLocal)
{
	if (fPitch >= 360)
		fPitch = fPitch - 360;
	if (fYaw >= 360)
		fYaw = fYaw - 360;
	if (fRoll >= 360)
		fRoll = fRoll - 360;

	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), 
		XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));

	if (isLocal)
		m_mtxLocal = mtxRotate * m_mtxLocal;
	else
		m_mtxWorld = mtxRotate * m_mtxWorld;
}

void CGameObject::Rotate(XMFLOAT3 fAngle, bool isLocal)
{
	if (fAngle.x >= 360)
		fAngle.x = fAngle.x - 360;
	if (fAngle.y >= 360)
		fAngle.y = fAngle.y - 360;
	if (fAngle.z >= 360)
		fAngle.z = fAngle.z - 360;

	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fAngle.x),
		XMConvertToRadians(fAngle.y), XMConvertToRadians(fAngle.z));

	if (isLocal)
		m_mtxLocal = mtxRotate * m_mtxLocal;
	else
		m_mtxWorld = mtxRotate * m_mtxWorld;
}

void CGameObject::Rotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationAxis(*pd3dxvAxis, XMConvertToRadians(fAngle));

	if (isLocal)
		m_mtxLocal = mtxRotate * m_mtxLocal;
	else
		m_mtxWorld = mtxRotate * m_mtxWorld;
}

void CGameObject::SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch),
		XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));

	XMFLOAT4X4 mtx; XMStoreFloat4x4(&mtx, mtxRotate);
	XMFLOAT3 position = GetPosition(isLocal);
	
	mtx._41 = position.x;
	mtx._42 = position.y;
	mtx._43 = position.z;

	if (isLocal)
		m_mtxLocal = XMLoadFloat4x4(&mtx);
	else
		m_mtxWorld = XMLoadFloat4x4(&mtx);
}

void CGameObject::SetRotate(XMFLOAT3 fAngle, bool isLocal)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fAngle.x),
		XMConvertToRadians(fAngle.y), XMConvertToRadians(fAngle.z));

	XMFLOAT4X4 mtx; XMStoreFloat4x4(&mtx, mtxRotate);
	XMFLOAT3 position = GetPosition(isLocal);

	mtx._41 = position.x;
	mtx._42 = position.y;
	mtx._43 = position.z;

	if (isLocal)
		m_mtxLocal = XMLoadFloat4x4(&mtx);
	else
		m_mtxWorld = XMLoadFloat4x4(&mtx);
}

void CGameObject::SetRotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal)
{
	XMMATRIX mtxRotate;
	mtxRotate = XMMatrixRotationAxis(*pd3dxvAxis, XMConvertToRadians(fAngle));

	XMFLOAT4X4 mtx; XMStoreFloat4x4(&mtx, mtxRotate);
	XMFLOAT3 position = GetPosition(isLocal);

	mtx._41 = position.x;
	mtx._42 = position.y;
	mtx._43 = position.z;

	if (isLocal)
		m_mtxLocal = XMLoadFloat4x4(&mtx);
	else
		m_mtxWorld = XMLoadFloat4x4(&mtx);
}

XMVECTOR CGameObject::GetvPosition(bool isLocal) const
{
	XMFLOAT4X4 mtx;
	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else 
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	return XMVectorSet(mtx._41, mtx._42, mtx._43, 0.f);
}

XMFLOAT3 CGameObject::GetPosition(bool isLocal) const
{
	XMFLOAT4X4 mtx;
	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	return XMFLOAT3(mtx._41, mtx._42, mtx._43);
}

XMVECTOR CGameObject::GetvRight(bool isLocal) const
{
	XMFLOAT4X4 mtx;
	
	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	XMVECTOR d3dxvRight = XMVectorSet(mtx._11, mtx._12, mtx._13, 0.f);
	d3dxvRight = XMVector3NormalizeEst(d3dxvRight);

	return d3dxvRight;
}

XMVECTOR CGameObject::GetvUp(bool isLocal) const
{
	XMFLOAT4X4 mtx;

	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	XMVECTOR d3dxvUp = XMVectorSet(mtx._21, mtx._22, mtx._23, 0.f);
	d3dxvUp = XMVector3Normalize(d3dxvUp);

	return d3dxvUp;
}

XMVECTOR CGameObject::GetvLook(bool isLocal) const
{
	XMFLOAT4X4 mtx;

	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	XMVECTOR d3dxvLookAt = XMVectorSet(mtx._31, mtx._32, mtx._33, 0.f);
	d3dxvLookAt = XMVector3Normalize(d3dxvLookAt);

	return d3dxvLookAt;
}

XMFLOAT3 CGameObject::GetRight(bool isLocal) const
{
	XMFLOAT4X4 mtx;

	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	XMVECTOR d3dxvRight = XMVectorSet(mtx._11, mtx._12, mtx._13, 0.f);
	d3dxvRight = XMVector3NormalizeEst(d3dxvRight);

	XMFLOAT3 right; XMStoreFloat3(&right, d3dxvRight);
	return right;
}

XMFLOAT3 CGameObject::GetUp(bool isLocal) const
{
	XMFLOAT4X4 mtx;

	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	XMVECTOR d3dxvUp = XMVectorSet(mtx._21, mtx._22, mtx._23, 0.f);
	d3dxvUp = XMVector3Normalize(d3dxvUp);

	XMFLOAT3 up; XMStoreFloat3(&up, d3dxvUp);
	return up;
}

XMFLOAT3 CGameObject::GetLook(bool isLocal) const
{
	XMFLOAT4X4 mtx;

	if (isLocal)
		XMStoreFloat4x4(&mtx, m_mtxLocal);
	else
		XMStoreFloat4x4(&mtx, m_mtxWorld);

	XMVECTOR d3dxvLookAt = XMVectorSet(mtx._31, mtx._32, mtx._33, 0.f);
	d3dxvLookAt = XMVector3Normalize(d3dxvLookAt);

	XMFLOAT3 look; XMStoreFloat3(&look, d3dxvLookAt);
	return look;
}

BoundingBox CGameObject::GetBoundingBox(bool isLocal) const
{
	if (isLocal) 
		return m_bcMeshBoundingBox;
	else {
		BoundingBox bcBox = m_bcMeshBoundingBox;
		bcBox.Transform(bcBox, m_mtxWorld);
		return bcBox;
	}
}

BoundingOrientedBox CGameObject::GetBoundingOBox(bool isLocal) const
{
	if (isLocal)
		return m_bcMeshBoundingOBox;
	else {
		BoundingOrientedBox bcObox = m_bcMeshBoundingOBox;
		bcObox.Transform(bcObox, m_mtxWorld);
		return bcObox;
	}
}

BoundingSphere CGameObject::GetBoundingSphere(bool isLocal) const
{
	if (isLocal)
		return m_bsMeshBoundingSphere;
	else {
		BoundingSphere bsSphere = m_bsMeshBoundingSphere;
		bsSphere.Transform(bsSphere, m_mtxWorld);
		return bsSphere;
	}
}

void CGameObject::SetRight(XMFLOAT3 axis, bool isLocal)
{
	XMFLOAT4X4 mtx;

	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._11 = axis.x, mtx._12 = axis.y, mtx._13 = axis.z;

		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._11 = axis.x, mtx._12 = axis.y, mtx._13 = axis.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::SetUp(XMFLOAT3 axis, bool isLocal)
{
	XMFLOAT4X4 mtx;

	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._21 = axis.x, mtx._22 = axis.y, mtx._23 = axis.z;

		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._21 = axis.x, mtx._22 = axis.y, mtx._23 = axis.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::SetvRight(XMVECTOR axis, bool isLocal)
{
	XMFLOAT4X4 mtx;
	XMFLOAT3 right; XMStoreFloat3(&right, axis);

	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._11 = right.x, mtx._12 = right.y, mtx._13 = right.z;

		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._11 = right.x, mtx._12 = right.y, mtx._13 = right.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::SetvUp(XMVECTOR axis, bool isLocal)
{
	XMFLOAT4X4 mtx;
	XMFLOAT3 up; XMStoreFloat3(&up, axis);

	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._21 = up.x, mtx._22 = up.y, mtx._23 = up.z;

		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._21 = up.x, mtx._22 = up.y, mtx._23 = up.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}


void CGameObject::SetLook(XMFLOAT3 axis, bool isLocal)
{
	XMFLOAT4X4 mtx;
	
	if (isLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._31 = axis.x, mtx._32 = axis.y, mtx._33 = axis.z;

		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._31 = axis.x, mtx._32 = axis.y, mtx._33 = axis.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::SetvLook(XMVECTOR axis, bool bIsLocal)
{
	XMFLOAT4X4 mtx;
	XMFLOAT3 look; XMStoreFloat3(&look, axis);

	if (bIsLocal) {
		XMStoreFloat4x4(&mtx, m_mtxLocal);
		mtx._31 = look.x, mtx._32 = look.y, mtx._33 = look.z;
		m_mtxLocal = XMLoadFloat4x4(&mtx);
	}
	else {
		XMStoreFloat4x4(&mtx, m_mtxWorld);
		mtx._31 = look.x, mtx._32 = look.y, mtx._33 = look.z;
		m_mtxWorld = XMLoadFloat4x4(&mtx);
	}
}

void CGameObject::CreateConstantBuffers(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbWorldMatrix);

	d3dBufferDesc.ByteWidth = sizeof(XMVECTOR) * 4;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterialColors);

	DXUT_SetDebugName(m_pd3dcbWorldMatrix, "WorldMatrix");
	DXUT_SetDebugName(m_pd3dcbMaterialColors, "MaterialColors");
}

void CGameObject::ReleaseConstantBuffers()
{
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
	if (m_pd3dcbMaterialColors) m_pd3dcbMaterialColors->Release();
}

void CGameObject::UpdateConstantBuffer_WorldMtx(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxWorld)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	pcbWorldMatrix->m_mtxWorld = XMMatrixTranspose(*pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_CB_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}

void CGameObject::UpdateConstantBuffer_Material(ID3D11DeviceContext *pd3dDeviceContext, CMaterialColors *pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterialColors, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMFLOAT4 *pcbMaterial = (XMFLOAT4 *)d3dMappedResource.pData;
	memcpy(pcbMaterial, &pMaterial->m_d3dxcAmbient, sizeof(XMFLOAT4));
	memcpy(pcbMaterial + 1, &pMaterial->m_d3dxcDiffuse, sizeof(XMFLOAT4));
	memcpy(pcbMaterial + 2, &pMaterial->m_d3dxcSpecular, sizeof(XMFLOAT4));
	memcpy(pcbMaterial + 3, &pMaterial->m_d3dxcEmissive, sizeof(XMFLOAT4));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterialColors, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_MATERIAL, 1, &m_pd3dcbMaterialColors);
}

bool CGameObject::IsVisible(CCamera *pCamera)
{
	m_bIsVisible = (m_bActive) ? true : false;
#ifdef _WITH_FRUSTUM_CULLING_BY_OBJECT
	if (m_bActive) {
		BoundingBox bcBoundingCube;
		m_bcMeshBoundingBox.Transform(bcBoundingCube, m_mtxWorld);
		if (pCamera) m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
	}
#endif

	// 임시로 해놓은 상태.	 - 09. 17
	if (m_tagMesh == MeshTag::eStair)
		m_bIsVisible = true;
	if (m_tagMesh == MeshTag::eStair2)
		m_bIsVisible = true;

	return(m_bIsVisible);
}

bool CGameObject::IsCollision(CGameObject* pObject)
{
	cout << "안쓰고있다." << endl;
	return true;
}

void CGameObject::InitCollisionInfo()
{
	m_bIsCollision = false;
	m_infoCollision = CollisionInfo();
}

void CGameObject::Update(float fDeltaTime)
{
	if (m_pAxisObject) {
		if (GLOBAL_MGR->g_bShowWorldAxis)
			m_pAxisObject->Update(fDeltaTime);
	}

	if (!XMMatrixIsIdentity(m_mtxLocal))
		m_mtxWorld = XMMatrixMultiply(m_mtxLocal, m_mtxWorld);
}

void CGameObject::RenderMesh(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	for (auto& mesh : m_vecMeshContainer) {
		bool bIsVisible = true;
#ifdef _WITH_FRUSTUM_CULLING_BY_SUBMESH
		if (pCamera) {
#if _AABB_
			AABB bcBoundingCube = m_ppMeshes[i]->GetBoundingCube();
			bcBoundingCube.Update(&XMLoadFloat4x4(&m_d3dxmtxWorld));
			bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
#else
			BoundingBox bcBoundingCube = m_ppMeshes[i]->GetBoundingCube();
			bcBoundingCube.Transform(bcBoundingCube, XMLoadFloat4x4(&m_d3dxmtxWorld));
			m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
#endif
		}
#endif
		if (bIsVisible) 
			mesh->Render(pd3dDeviceContext);
	}
}

void CGameObject::OnPrepareRender()
{
}

void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender();
	
	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);
	if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pd3dDeviceContext);

	CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &m_mtxWorld);

	if (m_pd3dBlendState) pd3dDeviceContext->OMSetBlendState(m_pd3dBlendState, NULL, 0xffffffff);

	RenderMesh(pd3dDeviceContext, pCamera);

	if (m_pAxisObject) {
		if (GLOBAL_MGR->g_bShowWorldAxis)
			m_pAxisObject->Render(pd3dDeviceContext, pCamera);
	}

	if (m_pShader) m_pShader->OnPostRender(pd3dDeviceContext);
	if (m_pd3dBlendState) pd3dDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void CGameObject::BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pBoundingBoxMesh) {
		XMMATRIX mtxBoundingBoxWorld = m_mtxWorld;
		BoundingOrientedBox bcObbox; 
		m_bcMeshBoundingOBox.Transform(bcObbox, mtxBoundingBoxWorld);
	 
		mtxBoundingBoxWorld = XMMatrixRotationQuaternion(XMLoadFloat4(&bcObbox.Orientation)) *
			XMMatrixTranslation(bcObbox.Center.x, bcObbox.Center.y, bcObbox.Center.z);

		CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &mtxBoundingBoxWorld);

		m_pBoundingBoxMesh->Render(pd3dDeviceContext);
	}
}