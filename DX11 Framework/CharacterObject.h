#pragma once
#include "SkinnedObject.h"
#include "RifleGunWeapon.h"
#include "SniperRifle.h"
#include "StateMachine.h"

struct DamagedInfo
{
	bool			m_bIsDamage = false;
	XMFLOAT3		m_f3DamagedPosition = XMFLOAT3(0, 0, 0);

	DamagedInfo() { m_bIsDamage = false; m_f3DamagedPosition = XMFLOAT3(0, 0, 0); }
	DamagedInfo(bool isDamage, XMFLOAT3 pos) { m_bIsDamage = isDamage, m_f3DamagedPosition = pos; }
	DamagedInfo(bool isDamage, XMVECTOR pos) { m_bIsDamage = isDamage, XMStoreFloat3(&m_f3DamagedPosition, pos); }
};

class CCharacterObject : public CSkinnedObject
{
public:
	CCharacterObject(TeamTag team);
	virtual ~CCharacterObject();

	static UINT			g_nCharacterCount;
protected:
	CStateMachine<CCharacterObject>*   m_pStateUpper = nullptr;
	CStateMachine<CCharacterObject>*   m_pStateLower = nullptr;

	CWeapon*            m_pWeapon[static_cast<UINT>(WeaponTag::eMaxWeaponCount)];
	UINT				m_nSelectWeapon = 0;
	UINT				m_nNextReplacementWeaponNumber = 0;

	XMFLOAT3            m_f3FiringDirection = XMFLOAT3(0, 0, 1);
	float               m_fPitch = 0.0f;
	float               m_fYaw = 0.0f;

	float               m_fPrevPitch = 0.0f;
	float               m_fPrevYaw = 0.0f;

	XMFLOAT3            m_f3RelativeVelocity = XMFLOAT3(0, 0, 0);

	UINT				m_nCharacterID = 0;

	// ----- State Variable ----- //
	bool				m_bIsFire = false;
	bool				m_bIsJump = false;
	bool				m_bIsCrouch = false;
	bool				m_bIsReload = false;
	bool				m_bIsRun = false;
	bool				m_bIsOccupy = false;
	bool				m_bIsDeathHead = false;
	bool				m_bIsHeadHit = false;
	bool				m_bTempIsRun = false;   // 임시로 달리기 속력 맞추려고 넣은 변수 이므로 사용 금지 - 추후 수정
	DWORD               m_dwWalkSoundWatingTime = 0;
	bool				m_bIsDeadly = false;
	bool				m_bIsDeadlyAttack = false;
	bool				m_bIsReplaceWeapon = false;

	// ----- Game System Variable ----- //
	DWORD				m_dwDeathStartTime = 0;
	DWORD				m_dwOccupyStartTime = 0;
	UINT				m_nServerID = 0;
	UINT				m_nLife = 0;
	UINT				m_nArmorPoint = 0;

	// ----- Parts Collision Variable ----- // 
	BoundingOrientedBox		m_bcPartsBoundingOBox[static_cast<int>(ChracterBoundingBoxParts::ePartsCount)];
	XMMATRIX				m_mtxPartsBoundingWorld[static_cast<int>(ChracterBoundingBoxParts::ePartsCount)];
	CBoundingBoxMesh		*m_pPartsBoundingBoxMesh[static_cast<int>(ChracterBoundingBoxParts::ePartsCount)];

protected:
	virtual void CreateWeapon(ID3D11Device *pd3dDevice) = 0;
	virtual void CreateStateMachine();

public:
	virtual void CreateObjectData(ID3D11Device *pd3dDevice) override;

	virtual void Update(float fDeltaTime) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera) override;
	virtual void BoundingBoxRender(ID3D11DeviceContext *pd3dDeviceContext) override;

	virtual void SetRotate(float fPitch, float fYaw, float fRoll, bool isLocal = false) override;
	virtual void SetRotate(XMFLOAT3 fAngle, bool isLocal = false) override;
	virtual void SetRotate(XMVECTOR *pd3dxvAxis, float fAngle, bool isLocal = false) override;

	virtual void RotateFiringPos();

	// ----- Game System Function ----- //
	virtual void Revival();
	void Reloading();
	void Firing();
	void Walking();
	void Running();
	void ReplaceWeapon(WeaponTag weapon);
	void DamagedCharacter(UINT damage);

	// ----- Get, Setter ----- // 

	BoundingOrientedBox GetPartsBoundingOBox(UINT index) const;
	CStateMachine<CCharacterObject>* GetFSM(AnimationData::Parts parts) const {
		if (parts == AnimationData::Parts::UpperBody)
			return m_pStateUpper;
		else
			return m_pStateLower;
	}
	void SetRelativevVelocity(XMVECTOR velocity) { XMStoreFloat3(&m_f3RelativeVelocity, velocity); }
	void SetRelativeVelocity(XMFLOAT3 velocity) { m_f3RelativeVelocity = velocity; }
	XMFLOAT3 GetRelativeVelocity() const { return m_f3RelativeVelocity; }
	
	void SetYaw(float yaw) { m_fYaw = yaw; }
	float GetYaw() const { return m_fYaw; }
	void SetPitch(float pitch) { m_fPitch = pitch; }
	void AddPitch(float pitch) { m_fPitch += pitch; }
	float GetPitch() const { return m_fPitch; }

	float GetminusPitch() { return m_fPrevPitch - m_fPitch; }
	float GetminusYaw() { return m_fPrevYaw - m_fYaw; }

	void SetPrevPitch(float pitch) { m_fPrevPitch = pitch; }
	void SetPrevYaw(float Yaw) { m_fPrevYaw = Yaw; }

	XMFLOAT3 GetFireDirection() const { return m_f3FiringDirection; }
	void SetFireDirection(XMFLOAT3 GetFireDirection) { m_f3FiringDirection = GetFireDirection; }

	CSpriteImageObject* GetMuzzleSprite() const { return m_pWeapon[m_nSelectWeapon]->GetMuzzleSprite(); }

	// --- 파츠별 바운딩 박스 월드 매트릭스 적용 함수 --- //
	void SetPartsWorldMtx();

	// ----- State Function ----- //
	bool IsMoving() const {	return (m_f3RelativeVelocity.x != 0 || m_f3RelativeVelocity.y != 0 || m_f3RelativeVelocity.z != 0);	}
	void SetIsRun(bool set) { m_bIsRun = set; }
	bool GetIsRun() const { return  m_bIsRun; }
	void SetIsCrouch(bool set) { m_bIsCrouch = set; }
	bool GetIsCrouch() const { return  m_bIsCrouch; }
	
	void SetIsTempRun(bool set) { m_bTempIsRun = set; }
	bool GetIsTempRun() const { return m_bTempIsRun; }
	void SetIsFire(bool bIsFire) { 
		m_bIsFire = bIsFire;
		if (false == bIsFire)
			m_pWeapon[m_nSelectWeapon]->SetFireBulletCount(0);
	}
	bool GetIsFire() const { return  m_bIsFire; }
	void SetIsReload(bool bIsReload) { 
		m_bIsReload = bIsReload;
		if (bIsReload)
			SetIsFire(false);
	}
	bool GetIsReload() const { return m_bIsReload; }
	
	void SetIsJump(bool set) { m_bIsJump = set; }
	bool GetIsJump() const { return m_bIsJump; }

	void SetOccupy(bool occupy) {
		static bool bIsOccupyStart = false;

		if (occupy) {
			if (!bIsOccupyStart) {
				m_bIsOccupy = true;
				bIsOccupyStart = true;
				m_dwOccupyStartTime = GetTickCount();
			}
		}
		else {
			m_bIsOccupy = false;
			bIsOccupyStart = false;
		}
	}
	bool GetIsOccupy() const { return m_bIsOccupy; }
	
	void SetDeath() {
		m_dwDeathStartTime = GetTickCount();
		m_nLife = 0;
		m_bIsOccupy = false;
	}
	bool GetAlive() const { return m_nLife; }

	bool GetIsDeath() const { return (m_nLife <= 0); }
	void SetIsDeathHead(bool set) { m_bIsDeathHead = set; }
	bool GetIsDeathHead() const { return m_bIsDeathHead; }
	void SetIsHeadHit(bool set) { m_bIsHeadHit = set; }
	bool GetIsHeadHit() const { return m_bIsHeadHit; }
	void SetIsDeadly(bool set) { m_bIsDeadly = set; }
	bool GetIsDeadly() const { return m_bIsDeadly; }
	void SetIsDeadlyAttack(bool set) { m_bIsDeadlyAttack = set; }
	bool GetIsDeadlyAttack() const { return m_bIsDeadlyAttack; }
	void SetIsReplaceWeapon(bool set) { m_bIsReplaceWeapon = set; }
	bool GetIsReplaceWeapon() const { return m_bIsReplaceWeapon; }
	
	void SetTeamTag(TeamTag Team) { m_tagTeam = Team; }
	TeamTag GetTagTeam() { return m_tagTeam; }
	UINT GetCharacterID() const { return m_nCharacterID; }

	// ----- Game System Function ----- //
	void SetWeaponNumber(UINT num) { m_nSelectWeapon = num; }
	UINT GetNextWeaponNumber() const { return m_nNextReplacementWeaponNumber; }

	void SetLife(UINT life) { m_nLife = life; if (m_nLife <= 0) SetDeath();	} //피가 0 이하이면 셋 데스 함수를 호출.
	UINT GetLife() const { return m_nLife; }
	void SetArmorPoint(UINT armorPoint) { m_nArmorPoint = armorPoint; }
	UINT GetArmorPoint() const { return m_nArmorPoint; }

	void SetServerID(UINT id) { m_nServerID = id; }
	UINT GetServerID() const { return m_nServerID; }
	UINT GetWeaponBulletCount() const { return m_pWeapon[m_nSelectWeapon]->GetBulletCount(); }
	UINT GetWeaponMaxBulletCount() const { return m_pWeapon[m_nSelectWeapon]->GetMaxBulletCount(); }
	void SetWeaponBulletMax() { m_pWeapon[m_nSelectWeapon]->Reloading(); }
	DWORD GetDeathTime() const { return m_dwDeathStartTime; }
	DWORD GetOccupyTime() const { return m_dwOccupyStartTime; }

	CWeapon* GetWeapon() const { return m_pWeapon[m_nSelectWeapon]; }
};