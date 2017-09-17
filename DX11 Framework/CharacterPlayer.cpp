#include "stdafx.h"
#include "CharacterPlayer.h"


CCharacterPlayer::CCharacterPlayer(TeamType team)
	: CCharacterObject(team)
{
}

CCharacterPlayer::~CCharacterPlayer()
{
}

void CCharacterPlayer::Revival()
{
	CCharacterObject::Revival();

	m_pWeapon->Reloading();

	XMFLOAT3 redTeamStartPosition = XMFLOAT3(65, 2.4f, 12);
	XMFLOAT3 blueTeamStartPosition = XMFLOAT3(270, 2.4f, 230);

	if (m_tagTeam == TeamType::eRedTeam)
		m_pPlayer->SetPosition(redTeamStartPosition);
	else
		m_pPlayer->SetPosition(blueTeamStartPosition);
}

void CCharacterPlayer::RotateFiringPos()
{
	XMVECTOR firingDirection = XMLoadFloat3(&m_f3FiringDirection);
	XMMATRIX mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetvRight(), XMConvertToRadians(m_fPitch));
	firingDirection = XMVector3TransformNormal(m_pPlayer->GetvLook(), mtxRotate);
	XMStoreFloat3(&m_f3FiringDirection, firingDirection);
	
	CCharacterObject::RotateFiringPos();
}

void CCharacterPlayer::Update(float fDeltaTime)
{
	m_pPlayer->Update(fDeltaTime);
	CCharacterObject::Update(fDeltaTime);
}

void CCharacterPlayer::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CameraTag playerCameraTag = m_pPlayer->GetCamera()->GetCameraTag();
	m_pPlayer->UpdateShaderVariables(pd3dDeviceContext);

	// 자유 시점시 플레이어 렌더링 X
	if (CameraTag::eFreeCam == playerCameraTag)
		return;

	m_pUpperController->UpdateConstantBuffer(pd3dDeviceContext);
	m_pLowerController->UpdateConstantBuffer(pd3dDeviceContext);

	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);
	if (m_pMaterial) m_pMaterial->UpdateShaderVariable(pd3dDeviceContext);

	CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &XMLoadFloat4x4(&m_pPlayer->m_mtxWorld));
	

	if (CameraTag::eFirstPerson == playerCameraTag)
		m_vecMeshContainer[1]->Render(pd3dDeviceContext);		// 팔 메쉬
	else
		m_vecMeshContainer[0]->Render(pd3dDeviceContext);		// 전체 메쉬

	if (m_pShader) m_pShader->OnPostRender(pd3dDeviceContext);

	m_pWeapon->Render(pd3dDeviceContext, pCamera);
}