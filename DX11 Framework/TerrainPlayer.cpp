#include "stdafx.h"
#include "TerrainPlayer.h"

CTerrainPlayer::CTerrainPlayer(CCharacterObject* pCharacter)
	: CPlayer(pCharacter)
{
}

CTerrainPlayer::~CTerrainPlayer()
{
}

void CTerrainPlayer::ChangeCamera(ID3D11Device *pd3dDevice, CameraTag nNewCameraTag)
{
	CameraTag nCurrentCameraTag = (m_pCamera) ? m_pCamera->GetCameraTag() : CameraTag::eNone;
	if (nCurrentCameraTag == nNewCameraTag) return;

	switch (nNewCameraTag){
	case CameraTag::eFirstPerson:
		SetFriction(250.0f);
		m_fGravityAcceleration = -100.0f;

		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eFirstPerson, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetPosition(GetPosition());
		m_pCamera->SetOffset(m_d3dxvLook, 0.5f);	// 캐릭터 가슴보다 조금 앞
		m_pCamera->SetOffset(m_d3dxvUp, 0.95f);
		m_pCamera->GenerateProjectionMatrix(0.05f, 5000.0f, ASPECT_RATIO, 45.0f);

		break;
	case CameraTag::eSpaceShip:
		SetFriction(125.0f);
		m_fGravityAcceleration = 0.0f;

		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eSpaceShip, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
	case CameraTag::eThirdPerson:
		SetFriction(250.0f);
		m_fGravityAcceleration = -100.0f;

		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eThirdPerson, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetPosition(GetPosition());
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 2.0f, -10.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
		/*
	case CameraTag::eFreeCam:		// - 추후 구현하기, 
		SetFriction(0);
		SetGravity(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

		SetMaxVelocityY(0.0f);
//		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eFreeCam, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetPosition(XMFLOAT3(0, 0, 0));
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 2.0f, -10.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
		*/
	default:
		break; 
	}
	SetMaxVelocityXZ(500.0f);
}

void CTerrainPlayer::OnApplyGravity(float fDeltaTime)
{
	m_fGravityTimeElapsed += fDeltaTime;
	m_d3dxvVelocity.y = m_d3dxvVelocity.y + m_fGravityAcceleration * m_fGravityTimeElapsed  * fDeltaTime;
	float groundHeight = 0.0f;
	if (m_bIsFloorCollision) {
		groundHeight = m_pCharacter->GetCollisionInfo().m_pHitObject->GetBoundingOBox().Center.y + m_pCharacter->GetCollisionInfo().m_pHitObject->GetBoundingOBox().Extents.y;
		m_fGravityTimeElapsed = 0.0f;
		m_d3dxvVelocity.y = 0.0f;

		XMFLOAT3 pos = GetPosition();

		if (pos.y > groundHeight)
			pos.y = m_pCharacter->GetBoundingOBox(true).Center.y + m_pCharacter->GetBoundingOBox().Extents.y + groundHeight;
		else
			pos.y = m_pCharacter->GetBoundingOBox(true).Center.y + m_pCharacter->GetBoundingOBox().Extents.y;
		
		SetPosition(XMLoadFloat3(&pos));

		m_bIsFloorCollision = false;
	}

}

void CTerrainPlayer::OnCameraUpdated(float fDeltaTime)
{
	// 해당 함수는 Terrain이 존재할 경우 카메라가 땅아래에 있지 않도록 하는 함수로 
	// 이를 응용하여 카메라가 벽안으로 들어갈 경우 처리할 때 참조하도록 한다.

	/*
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pCameraUpdatedContext;
	XMFLOAT3 d3dxvScale;
	XMStoreFloat3(&d3dxvScale, pTerrain->GetScale());
	XMFLOAT3 d3dxvCameraPosition = m_pCamera->GetPosition();

	int z = (int)(d3dxvCameraPosition.z / d3dxvScale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(d3dxvCameraPosition.x, d3dxvCameraPosition.z, bReverseQuad) + 5.0f;
	if (d3dxvCameraPosition.y <= fHeight)
	{
		d3dxvCameraPosition.y = fHeight;
		m_pCamera->SetPosition(d3dxvCameraPosition);
		if (m_pCamera->GetCameraTag() == CameraTag::eThirdPerson)
		{
			CThirdPersonCamera *p3rdPersonCamera = (CThirdPersonCamera *)m_pCamera;
			p3rdPersonCamera->SetLookAt(GetvPosition());
		}
	}
	*/
}
