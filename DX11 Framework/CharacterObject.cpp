#include "stdafx.h"
#include "CharacterObject.h"

CCharacterObject::CCharacterObject()
{
	for (int i = 0; i < ePartsCount; ++i)
		m_pPartsBoundingBoxMesh[i] = nullptr;
}

CCharacterObject::~CCharacterObject()
{
	SafeDelete(m_pWeapon);

	for (int i = 0; i < ePartsCount; ++i)
		SafeDelete(m_pPartsBoundingBoxMesh[i]);
}

void CCharacterObject::CreateObjectData(ID3D11Device *pd3dDevice)
{
	CSkinnedObject::CreateObjectData(pd3dDevice);
	CreateWeapon(pd3dDevice);
}

BoundingOrientedBox CCharacterObject::GetPartsBoundingOBox(UINT index) const
{
	BoundingOrientedBox bcObox = m_bcPartsBoundingOBox[index];
	bcObox.Transform(bcObox, m_mtxPartsBoundingWorld[index]);
	return bcObox;
}

void CCharacterObject::Firing()
{
	SetAnimation(AnimationData::CharacterAnim::eStandingFire);

	XMStoreFloat3(&m_f3FiringDirection, CGameObject::GetLook());


	m_pWeapon->Firing(XMLoadFloat3(&m_f3FiringDirection));
}

void CCharacterObject::Running()
{
	SetAnimation(AnimationData::CharacterAnim::eRun);
//	m_pPlayer->GetCamera()->Move(GetLook() * 1);			// ���� ����
}

void CCharacterObject::OnCollisionCheck()
{	
	BoundingOrientedBox bcObox = GetBoundingOBox();

	CollisionInfo info;
	/*
	for (auto targetObject : COLLISION_MGR->m_vecStaticMeshContainer) {
		if (COLLISION_MGR->CheckCollision(info, this, targetObject))
			cout << "�浹 "<< endl;
	}
	*/

	XMVECTOR velocity = m_pPlayer->GetVelocity();

	if(m_pPlayer->IsMoving()){
		for (auto staticObject : COLLISION_MGR->m_vecStaticMeshContainer) {

			XMMATRIX d3dxmtxInverse;
			d3dxmtxInverse = XMMatrixInverse(NULL, staticObject->m_mtxWorld);

			XMVECTOR rayPosition;
			rayPosition = XMVector3TransformCoord(GetvPosition(), d3dxmtxInverse);
			
			XMVECTOR rayDirection;
			rayDirection = XMVector3TransformCoord(velocity, d3dxmtxInverse);
			rayDirection = XMVector3Normalize(rayDirection);

			int count = staticObject->GetMesh()->CheckRayIntersection(&rayPosition, &rayDirection, &info);

			if (count) {
				if (info.m_fDistance < 5) {
					cout << "�浹ü ���� : " << endl;
					cout << "�Ÿ� : " << info.m_fDistance << endl;
//					cout << "���� : "; ShowXMFloat3(info.m_f3HitNormal);
//					cout << "�޽� �±� : " << static_cast<int>(staticObject->GetMeshTag()) << endl;

					XMVECTOR slidingVec = velocity - XMVector3Dot(velocity, XMLoadFloat3(&info.m_f3HitNormal)) * XMLoadFloat3(&info.m_f3HitNormal);
//					cout << "�����̵� ���� : "; ShowXMVector(slidingVec);

					// ��ġ �����ڵ� �߰��ϱ�
					m_pPlayer->SetVelocity(slidingVec);
					// �븻���� �������� distance ��ŭ �о��

		//			XMVECTOR offsetPos = XMLoadFloat3(&info.m_f3HitNormal) * info.m_fDistance;
		//			m_pPlayer->Move(offsetPos);
				}
			}
		}
	}
		
	
	// �Ʒ� �������� �浹 - ����� ���̿� ���缭 ��ġ�� �״�� set
	// -> ���� ������ distance�� �̿��Ͽ� ���̸� ���ϰ� �߷°��� �̿��Ͽ� �����ϵ��� �����ϱ�
	if (COLLISION_MGR->RayCastCollision(m_infoCollision, XMLoadFloat3(&bcObox.Center), -1 * GetUp())) {
		if (m_infoCollision.m_fDistance <= bcObox.Extents.y) {
			m_pPlayer->SetFloorCollision(true);
		}
	}
	
		

	// �浹 �����ϱ� ���� ��ŷ �ڵ� ���� �ٽ� �����غ���
	// ���� ������ �߸��� ��
} 

void CCharacterObject::SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal)	//�̺κ� ���� ��ħ.
{
	CGameObject::SetRotate(0, fYaw, fRoll, isLocal);
	//XMMATRIX mtxRotate;
	//mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch),
	//	XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));

	//XMFLOAT4X4 mtx; XMStoreFloat4x4(&mtx, mtxRotate);
	//XMFLOAT3 position = GetPosition();

	//mtx._41 = position.x;
	//mtx._42 = position.y;
	//mtx._43 = position.z;

	//this->m_mtxWorld = XMLoadFloat4x4(&mtx);
//	Object.SetRotate(fPitch, fYaw, fRoll, isLocal);

	m_fPitch = fPitch;
}

void CCharacterObject::SetRotate(XMFLOAT3 fAngle, bool isLocal)
{
	//Object.SetRotate(0, fAngle.y, fAngle.z, isLocal);
	m_fPitch = fAngle.x;
}

void CCharacterObject::SetRotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal)
{
	XMFLOAT3 axis; XMStoreFloat3(&axis, *pd3dxvAxis);

	Object.SetRotate(0, axis.y, axis.z, isLocal);

	m_fPitch = axis.x;
}

void CCharacterObject::RotateFiringPos()
{
	if (m_pPlayer) {
		m_f3FiringDirection = m_pPlayer->GetLook();		// ������ �ʿ��� �κ���
		m_fPitch = m_pPlayer->GetPitch();
	}

	m_fPitch = clamp(m_fPitch, -40.0f, 50.0f);		// �� ������ ĳ���͸��� �ٸ����� ����� ���⿡�� clamp�ϵ��� ����
	GetSkinnedMesh()->SetPitch(m_fPitch);
	// ȸ�� �����ϰ� �Ѿ� �޽��� �� ��׷������� �׷��� ����� �س���
	 // XMMATRIX mtxRotate = XMMatrixRotationAxis(XMVectorSet(1.0f, 0, 0, 0), XMConvertToRadians(m_fPitch)
}

void CCharacterObject::Update(float fTimeElapsed)
{
	RotateFiringPos();
	if (m_pPlayer) {
		m_pPlayer->UpdateKeyInput(fTimeElapsed);
		OnCollisionCheck();
		m_pPlayer->Update(fTimeElapsed);
	}

	CGameObject::Update(fTimeElapsed);
	CSkinnedObject::Update(fTimeElapsed);
	m_pWeapon->Update(fTimeElapsed);
}

void CCharacterObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	if(m_pPlayer)
		m_pPlayer->UpdateShaderVariables(pd3dDeviceContext);
		CSkinnedObject::Render(pd3dDeviceContext, pCamera);

	m_pWeapon->Render(pd3dDeviceContext, pCamera);
}

void CCharacterObject::BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	m_mtxPartsBoundingWorld[BoundingBoxParts::eBody] = GetSkinnedMesh()->GetFinalBoneMtx(2) * m_mtxWorld;		// ���� �ε��� 2
	m_mtxPartsBoundingWorld[BoundingBoxParts::eHead] = GetSkinnedMesh()->GetFinalBoneMtx(5) * m_mtxWorld;		// �Ӹ� �ε��� 5

	for (int i = 0; i < ePartsCount; ++i) {
		if (m_pPartsBoundingBoxMesh[i]) {
			BoundingOrientedBox bcObbox;
			XMMATRIX mtxBoundingBoxWorld = m_mtxPartsBoundingWorld[i];

			m_bcPartsBoundingOBox[i].Transform(bcObbox, mtxBoundingBoxWorld);
			
			mtxBoundingBoxWorld = XMMatrixRotationQuaternion(XMLoadFloat4(&bcObbox.Orientation)) * 
				XMMatrixTranslation(bcObbox.Center.x, bcObbox.Center.y, bcObbox.Center.z);
			
			CGameObject::UpdateConstantBuffer_WorldMtx(pd3dDeviceContext, &mtxBoundingBoxWorld);

			m_pPartsBoundingBoxMesh[i]->Render(pd3dDeviceContext);
		}
	}
}