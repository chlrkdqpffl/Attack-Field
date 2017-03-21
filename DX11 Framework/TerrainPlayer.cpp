#include "stdafx.h"
#include "TerrainPlayer.h"

CTerrainPlayer::CTerrainPlayer(CCharacterObject* pCharacter)
	: CPlayer(pCharacter)
{
}

CTerrainPlayer::~CTerrainPlayer()
{
}

void CTerrainPlayer::ChangeCamera(ID3D11Device *pd3dDevice, CameraTag nNewCameraTag, float fTimeElapsed)
{
	CameraTag nCurrentCameraTag = (m_pCamera) ? m_pCamera->GetCameraTag() : CameraTag::eNone;
	if (nCurrentCameraTag == nNewCameraTag) return;
	switch (nNewCameraTag)
	{
	case CameraTag::eFirstPerson:
		SetFriction(250.0f);
		SetGravity(XMVectorSet(0.0f, -400.0f, 0.0f, 0.0f));

		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eFirstPerson, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMVectorSet(0.0f, 20.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
	case CameraTag::eSpaceShip:
		SetFriction(125.0f);
		SetGravity(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eSpaceShip, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
	case CameraTag::eThirdPerson:
		SetFriction(250.0f);
		SetGravity(XMVectorSet(0.0f, -250.0f, 0.0f, 0.0f));
		
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eThirdPerson, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffset(XMVectorSet(0.0f, 20.0f, -50.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
	default:
		break;
	}
	SetMaxVelocityXZ(1000.0f);
	Update(fTimeElapsed);
}

void CTerrainPlayer::OnPlayerUpdated(float fTimeElapsed)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pPlayerUpdatedContext;
	XMVECTOR d3dxvScale = pTerrain->GetScale();
	XMFLOAT3 d3dxvPlayerPosition;
	XMStoreFloat3(&d3dxvPlayerPosition, GetvPosition());
	int z = (int)(d3dxvPlayerPosition.z / XMVectorGetZ(d3dxvScale));
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(d3dxvPlayerPosition.x, d3dxvPlayerPosition.z, bReverseQuad) + 6.0f;
	if (d3dxvPlayerPosition.y < fHeight)
	{
		XMFLOAT3 d3dxvPlayerVelocity;
		XMStoreFloat3(&d3dxvPlayerVelocity, GetVelocity());
		d3dxvPlayerVelocity.y = 0.0f;
		SetVelocity(XMLoadFloat3(&d3dxvPlayerVelocity));
		d3dxvPlayerPosition.y = fHeight;
		SetPosition(XMLoadFloat3(&d3dxvPlayerPosition));
	}
}

void CTerrainPlayer::OnCameraUpdated(float fTimeElapsed)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pCameraUpdatedContext;
	XMFLOAT3 d3dxvScale;
	XMStoreFloat3(&d3dxvScale, pTerrain->GetScale());
	XMFLOAT3 d3dxvCameraPosition;
	XMStoreFloat3(&d3dxvCameraPosition, m_pCamera->GetPosition());
	int z = (int)(d3dxvCameraPosition.z / d3dxvScale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(d3dxvCameraPosition.x, d3dxvCameraPosition.z, bReverseQuad) + 5.0f;
	if (d3dxvCameraPosition.y <= fHeight)
	{
		d3dxvCameraPosition.y = fHeight;
		m_pCamera->SetPosition(XMLoadFloat3(&d3dxvCameraPosition));
		if (m_pCamera->GetCameraTag() == CameraTag::eThirdPerson)
		{
			CThirdPersonCamera *p3rdPersonCamera = (CThirdPersonCamera *)m_pCamera;
			p3rdPersonCamera->SetLookAt(GetvPosition());
		}
	}
}
