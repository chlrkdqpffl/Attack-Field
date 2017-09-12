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

	XMFLOAT3						m_vPosition = XMFLOAT3(0, 0, 0);
	XMFLOAT3						m_vRight = XMFLOAT3(0, 0, 0);
	XMFLOAT3						m_vUp = XMFLOAT3(0, 0, 0);
	XMFLOAT3						m_vLook = XMFLOAT3(0, 0, 0);

	CameraTag						m_tagCamera = CameraTag::eNone;

	XMFLOAT3						m_vOffset = XMFLOAT3(0, 0, 0);
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

	void SetPosition(XMFLOAT3 vPosition) { m_vPosition = vPosition; }
	void SetPosition(XMVECTOR vPosition) { XMStoreFloat3(&m_vPosition, vPosition); }
	XMVECTOR GetvPosition() const { return(XMLoadFloat3(&m_vPosition)); }
	XMFLOAT3 GetPosition() const { return m_vPosition; }

	XMFLOAT3 GetRight() const { return m_vRight; }
	XMFLOAT3 GetUp() const { return m_vUp; }
	XMFLOAT3 GetLook() const { return m_vLook; }

	XMVECTOR GetvRight() const { return(XMLoadFloat3(&m_vRight)); }
	XMVECTOR GetvUp() const { return(XMLoadFloat3(&m_vUp)); }
	XMVECTOR GetvLook() const { return(XMLoadFloat3(&m_vLook)); }
	float GetNearPlane() const { return m_fNearPlane; }
	float GetFarPlane() const { return m_fFarPlane; }

	void SetOffset(XMFLOAT3 offset) { m_vOffset = offset; }
	XMFLOAT3 GetOffset() const { return m_vOffset; }

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() const { return(m_fTimeLag); }

	virtual void Move(const XMVECTOR& d3dxvShift)
	{ 
		m_PrevPosition = m_vPosition;
		XMVECTOR v = XMLoadFloat3(&m_vPosition);
		v += d3dxvShift;
		XMStoreFloat3(&m_vPosition, v);
	}
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void Update(float fDeltaTime);
	virtual void SetLookAt(XMVECTOR& vLookAt);
	virtual void SetLookAt(XMVECTOR& d3dxvPosition, XMVECTOR& d3dxvLookAt, XMVECTOR& vd3dxvUp);

	void CalculateFrustumPlanes();
	bool IsInFrustum(XMVECTOR& xCenter, XMVECTOR& xExtern);
	bool IsInFrustum(BoundingBox *boundingbox);
	bool IsInFrustum(BoundingSphere *boundingSphere);
};