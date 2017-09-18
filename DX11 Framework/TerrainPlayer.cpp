#include "stdafx.h"
#include "TerrainPlayer.h"

CTerrainPlayer::CTerrainPlayer(CCharacterPlayer* pCharacter)
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
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eFirstPerson, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 1.0f, 0.5f));
		m_pCamera->GenerateProjectionMatrix(0.05f, 5000.0f, ASPECT_RATIO, 45.0f);
		break;

	case CameraTag::eFreeCam:
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eFreeCam, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;

	case CameraTag::eThirdPerson:
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eThirdPerson, nCurrentCameraTag);
//		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 1.5f, -8.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
	default:
		break; 
	}
}

/*
void CTerrainPlayer::OnApplyGravity(float fDeltaTime)
{
	m_fGravityTimeElapsed += fDeltaTime;
	m_d3dxvVelocity.y = m_d3dxvVelocity.y + m_fGravityAcceleration * m_fGravityTimeElapsed  * fDeltaTime;
	float groundHeight = 0.0f;
	if (m_bIsFloorCollision) {
		groundHeight = m_pCharacter->GetCollisionInfo().m_pHitObject->GetBoundingOBox().Center.y + m_pCharacter->GetCollisionInfo().m_pHitObject->GetBoundingOBox().Extents.y;
	//	cout << "땅 높이 : " << groundHeight << endl;
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
*/

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
