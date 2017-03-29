#pragma once

#include "Mesh.h"

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
	XMFLOAT3						m_d3dxvPosition;
	XMFLOAT3						m_d3dxvRight;
	XMFLOAT3						m_d3dxvUp;
	XMFLOAT3						m_d3dxvLook;

	float           				m_fPitch;
	float           				m_fRoll;
	float           				m_fYaw;

	CameraTag						m_tagCamera = CameraTag::eNone;

	XMFLOAT3						m_d3dxvOffset;
	float           				m_fTimeLag;

	XMFLOAT4X4						m_d3dxmtxView;
	XMFLOAT4X4						m_d3dxmtxProjection;

	D3D11_VIEWPORT					m_d3dViewport;

	static ID3D11Buffer				*m_pd3dcbCamera;
	static ID3D11Buffer				*m_pd3dcbCameraPosition;

	CPlayer							*m_pPlayer;

	XMFLOAT4						m_pd3dxFrustumPlanes[6]; //World Coordinates          

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

	void SetPosition(XMVECTOR d3dxvPosition) { XMStoreFloat3(&m_d3dxvPosition, d3dxvPosition); }
	XMVECTOR GetvPosition() const { return(XMLoadFloat3(&m_d3dxvPosition)); }
	XMFLOAT3 GetPosition() const { return m_d3dxvPosition; }
	XMVECTOR GetRightVector() const { return(XMLoadFloat3(&m_d3dxvRight)); }
	XMVECTOR GetUpVector() const { return(XMLoadFloat3(&m_d3dxvUp)); }
	XMVECTOR GetLookVector() const { return(XMLoadFloat3(&m_d3dxvLook)); }

	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }
	float GetYaw() const { return(m_fYaw); }

	void SetOffset(XMVECTOR d3dxvOffset) 
	{
		XMStoreFloat3(&m_d3dxvOffset, d3dxvOffset);
		XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
		v += d3dxvOffset;
		XMStoreFloat3(&m_d3dxvPosition, v);
	}
	XMVECTOR GetOffset() const { return(XMLoadFloat3(&m_d3dxvOffset)); }

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() const { return(m_fTimeLag); }

	virtual void Move(const XMVECTOR& d3dxvShift)
	{ 
		XMVECTOR v = XMLoadFloat3(&m_d3dxvPosition);
		v += d3dxvShift;
		XMStoreFloat3(&m_d3dxvPosition, v);
	}
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void Update(XMVECTOR& d3dxvLookAt, float fTimeElapsed) { }
	virtual void SetLookAt(XMVECTOR& vLookAt) { }
	virtual void SetLookAt(XMVECTOR& d3dxvPosition, XMVECTOR& d3dxvLookAt, XMVECTOR& vd3dxvUp);

	void CalculateFrustumPlanes();

	bool IsInFrustum(XMVECTOR& xCenter, XMVECTOR& xExtern);
	bool IsInFrustum(BoundingBox *boundingbox);
};