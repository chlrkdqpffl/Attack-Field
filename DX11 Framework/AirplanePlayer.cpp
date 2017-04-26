#include "stdafx.h"
#include "AirplanePlayer.h"


CAirplanePlayer::CAirplanePlayer(CCharacterObject* pCharacter)
	: CPlayer(pCharacter)
{
}

CAirplanePlayer::~CAirplanePlayer()
{
}

void CAirplanePlayer::ChangeCamera(ID3D11Device *pd3dDevice, CameraTag nNewCameraTag, float fTimeElapsed)
{
	CameraTag nCurrentCameraTag = (m_pCamera) ? m_pCamera->GetCameraTag() : CameraTag::eNone;
	if (nCurrentCameraTag == nNewCameraTag) return;
	switch (nNewCameraTag)
	{
	case CameraTag::eFirstPerson:
		SetFriction(200.0f);
		SetGravity(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(125.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eFirstPerson, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
		break;
	case CameraTag::eSpaceShip:
		SetFriction(125.0f);
		SetGravity(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(400.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eSpaceShip, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
		break;
	case CameraTag::eThirdPerson:
		SetFriction(250.0f);
		SetGravity(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(125.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, CameraTag::eThirdPerson, nCurrentCameraTag);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 25.0f, -35.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
		break;
	default:
		break;
	}
	Update(fTimeElapsed);
}

void CAirplanePlayer::OnPlayerUpdated(float fTimeElapsed)
{
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pPlayerUpdatedContext;
	XMVECTOR d3dxvScale = pTerrain->GetScale();
	XMFLOAT3 d3dxvPlayerPosition = GetPosition();

	int z = (int)(d3dxvPlayerPosition.z / XMVectorGetZ(d3dxvScale));
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(d3dxvPlayerPosition.x, d3dxvPlayerPosition.z, bReverseQuad) + 30.0f;
	if (d3dxvPlayerPosition.y < fHeight)
	{
		XMVECTOR d3dxvPlayerVelocity = GetVelocity();
		d3dxvPlayerVelocity = XMVectorSetY(d3dxvPlayerVelocity, 0.0f);
		SetVelocity(d3dxvPlayerVelocity);
		d3dxvPlayerPosition.y = fHeight;
		SetPosition(XMLoadFloat3(&d3dxvPlayerPosition));
	}
}