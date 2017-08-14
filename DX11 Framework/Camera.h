#pragma once

#define ASPECT_RATIO				(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

#define FIRST_PERSON_CAMERA			0x01
#define SPACESHIP_CAMERA			0x02
#define THIRD_PERSON_CAMERA			0x03

struct VS_CB_CAMERA
{
	XMFLOAT4X4						m_d3dxmtxView;
	XMFLOAT4X4						m_d3dxmtxProjection;
};

class CPlayer;

class CCamera
{
protected:
	XMFLOAT3						m_PrevPosition = XMFLOAT3(0, 0, 0);

	XMFLOAT3						m_d3dxvPosition;
	XMFLOAT3						m_d3dxvRight;
	XMFLOAT3						m_d3dxvUp;
	XMFLOAT3						m_d3dxvLook;

	CameraTag						m_tagCamera = CameraTag::eNone;

	XMFLOAT3						m_d3dxvOffset;
	float           				m_fTimeLag;

	XMFLOAT4X4						m_d3dxmtxView;
	XMFLOAT4X4						m_d3dxmtxProjection;


	float							m_fNearPlane = 0.0f;
	float							m_fFarPlane = 0.0f;
	D3D11_VIEWPORT					m_d3dViewport;

	CPlayer							*m_pPlayer;
	XMFLOAT4						m_pd3dxFrustumPlanes[6]; //World Coordinates          

	static ID3D11Buffer				*m_pd3dcbCamera;
	static ID3D11Buffer				*m_pd3dcbCameraPosition;

public:
	CCamera(CCamera *pCamera);
	virtual ~CCamera();

	void SetCameraTag(CameraTag tag) { m_tagCamera = tag; }
	CameraTag GetCameraTag() const { return m_tagCamera; }

	void GenerateViewMatrix();
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	static void CreateShaderVariables(ID3D11Device *pd3dDevice);
	static void ReleaseShaderVariables();
	static void UpdateConstantBuffer_ViewProjection(ID3D11DeviceContext *pd3dDeviceContext, XMMATRIX *pd3dxmtxView, XMMATRIX *pd3dxmtxProjection);
	static void UpdateConstantBuffer_CameraPos(ID3D11DeviceContext *pd3dDeviceContext, XMVECTOR*);

	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);

	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }
	CPlayer *GetPlayer() const { return(m_pPlayer); }

	void SetViewport(ID3D11DeviceContext *pd3dDeviceContext, DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetViewport(ID3D11DeviceContext *pd3dDeviceContext);
	D3D11_VIEWPORT GetViewport() const { return(m_d3dViewport); }

	XMMATRIX GetViewMatrix() const { return(XMLoadFloat4x4(&m_d3dxmtxView)); }
	XMMATRIX GetProjectionMatrix() const { return(XMLoadFloat4x4(&m_d3dxmtxProjection)); }
	ID3D11Buffer *GetCameraConstantBuffer() const { return(m_pd3dcbCamera); }

	void SetPosition(XMFLOAT3 vPosition) { m_d3dxvPosition = vPosition; }
	void SetPosition(XMVECTOR vPosition) { XMStoreFloat3(&m_d3dxvPosition, vPosition); }
	XMVECTOR GetvPosition() const { return(XMLoadFloat3(&m_d3dxvPosition)); }
	XMFLOAT3 GetPosition() const { return m_d3dxvPosition; }
	XMFLOAT3 GetRight() const { return m_d3dxvRight; }
	XMFLOAT3 GetUp() const { return m_d3dxvUp; }
	XMFLOAT3 GetLook() const { return m_d3dxvLook; }
	XMVECTOR GetvRight() const { return(XMLoadFloat3(&m_d3dxvRight)); }
	XMVECTOR GetvUp() const { return(XMLoadFloat3(&m_d3dxvUp)); }
	XMVECTOR GetvLook() const { return(XMLoadFloat3(&m_d3dxvLook)); }
	float GetNearPlane() const { return m_fNearPlane; }
	float GetFarPlane() const { return m_fFarPlane; }

	void SetOffset(XMFLOAT3 offset)
	{
		m_d3dxvOffset = offset;

		m_d3dxvPosition.x += offset.x;
		m_d3dxvPosition.y += offset.y;
		m_d3dxvPosition.z += offset.z;
	}

	void SetOffset(XMFLOAT3 direction, float distance) 
	{	
		m_d3dxvOffset.x = direction.x * distance;
		m_d3dxvOffset.y = direction.y * distance;
		m_d3dxvOffset.z = direction.z * distance;

		m_d3dxvPosition.x += m_d3dxvOffset.x;
		m_d3dxvPosition.y += m_d3dxvOffset.y;
		m_d3dxvPosition.z += m_d3dxvOffset.z;
	}

	XMFLOAT3 GetOffset() const { return m_d3dxvOffset; }

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() const { return(m_fTimeLag); }

	virtual void Move(const XMVECTOR& d3dxvShift)
	{ 
		m_PrevPosition = m_d3dxvPosition;
		XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
		v += d3dxvShift;
		XMStoreFloat3(&m_d3dxvPosition, v);
	}
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void Update(float fDeltaTime) { }
	virtual void SetLookAt(XMVECTOR& vLookAt);
	virtual void SetLookAt(XMVECTOR& d3dxvPosition, XMVECTOR& d3dxvLookAt, XMVECTOR& vd3dxvUp);

	void CalculateFrustumPlanes();
	bool IsInFrustum(XMVECTOR& xCenter, XMVECTOR& xExtern);
	bool IsInFrustum(BoundingBox *boundingbox);
	bool IsInFrustum(BoundingSphere *boundingSphere);
};