#pragma once
#include "SingletonManager.h"
#include "GBuffer.h"

inline const XMFLOAT3 GammaToLinear(const XMFLOAT3& color)
{
	return XMFLOAT3(color.x * color.x, color.y * color.y, color.z * color.z);
}

#pragma pack(push,1)
struct CB_DIRECTIONAL
{
	XMFLOAT3 vAmbientLower;
	float pad;
	XMFLOAT3 vAmbientRange;
	float pad2;
	XMFLOAT3 vDirToLight;
	float pad3;
	XMFLOAT3 vDirectionalColor;
	float pad4;
};

struct CB_POINT_LIGHT_DOMAIN
{
	XMFLOAT4X4 WolrdViewProj;
};

struct CB_POINT_LIGHT_PIXEL
{
	XMFLOAT3 PointLightPos;
	float PointLightRangeRcp;
	XMFLOAT3 PointColor;
	float pad;
	XMFLOAT2 LightPerspectiveValues;
	float pad2[2];
};

struct CB_SPOT_LIGHT_DOMAIN
{
	XMFLOAT4X4 WolrdViewProj;
	float fSinAngle;
	float fCosAngle;
	float pad[2];
};

struct CB_SPOT_LIGHT_PIXEL
{
	XMFLOAT3 SpotLightPos;
	float SpotLightRangeRcp;
	XMFLOAT3 vDirToLight;
	float SpotCosOuterCone;
	XMFLOAT3 SpotColor;
	float SpotCosConeAttRange;
};


struct CB_CAPSULE_LIGHT_DOMAIN
{
	XMFLOAT4X4 WolrdViewProj;
	float HalfCapsuleLen;
	float CapsuleRange;
	float pad[2];
};

struct CB_CAPSULE_LIGHT_PIXEL
{
	XMFLOAT3 CapsuleLightPos;
	float CapsuleLightRangeRcp;
	XMFLOAT3 CapsuleDir;
	float CapsuleLen;
	XMFLOAT3 CapsuleColor;
	float pad;
};
#pragma pack(pop)

class CLightManager : public CSingletonManager<CLightManager>
{
public:

	CLightManager();
	~CLightManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	//	void Update();

	// Set the ambient values
	void SetAmbient(const XMFLOAT3& vAmbientLowerColor, const XMFLOAT3& vAmbientUpperColor)
	{
		m_vAmbientLowerColor = vAmbientLowerColor;
		m_vAmbientUpperColor = vAmbientUpperColor;
	}

	// Set the directional light values
	void SetDirectional(const XMFLOAT3& vDirectionalDir, const XMFLOAT3& vDirectionalColor)
	{
		XMVECTOR normal = XMLoadFloat3(&vDirectionalDir);
		XMStoreFloat3(&m_vDirectionalDir, XMVector3NormalizeEst(normal));
		m_vDirectionalColor = vDirectionalColor;
	}

	// Clear the lights from the previous frame
	void ClearLights() { m_arrLights.clear(); }

	// Add a single point light
	void AddPointLight(const XMFLOAT3& vPointPosition, float fPointRange, const XMFLOAT3& vPointColor)
	{
		LIGHT pointLight;

		pointLight.eLightType = TYPE_POINT;
		pointLight.vPosition = vPointPosition;
		pointLight.fRange = fPointRange;
		pointLight.vColor = vPointColor;
		pointLight.m_bsBoundingSphere = BoundingSphere(XMFLOAT3(vPointPosition), fPointRange);

		m_arrLights.push_back(pointLight);
	}

	void AddSpotLight(const XMFLOAT3& vSpotPosition, const XMFLOAT3& vSpotDirection, float fSpotRange,
		float fSpotOuterAngle, float fSpotInnerAngle, const XMFLOAT3& vSpotColor)
	{
		LIGHT spotLight;

		spotLight.eLightType = TYPE_SPOT;
		spotLight.vPosition = vSpotPosition;
		spotLight.vDirection = vSpotDirection;
		spotLight.fRange = fSpotRange;
		spotLight.fOuterAngle = D3DX_PI * fSpotOuterAngle / 180.0f;
		spotLight.fInnerAngle = D3DX_PI * fSpotInnerAngle / 180.0f;
		spotLight.vColor = vSpotColor;
		spotLight.m_bsBoundingSphere = BoundingSphere(XMFLOAT3(vSpotPosition), fSpotRange);

		m_arrLights.push_back(spotLight);
	}

	void AddCapsuleLight(const XMFLOAT3& vCapsulePosition, const XMFLOAT3& vCapsuleDirection, float fCapsuleRange,
		float fCapsuleLength, const XMFLOAT3& vCapsuleColor)
	{
		LIGHT capsuleLight;

		capsuleLight.eLightType = TYPE_CAPSULE;
		capsuleLight.vPosition = vCapsulePosition;
		capsuleLight.vDirection = vCapsuleDirection;
		capsuleLight.fRange = fCapsuleRange;
		capsuleLight.fLength = fCapsuleLength;
		capsuleLight.vColor = vCapsuleColor;
		capsuleLight.m_bsBoundingSphere = BoundingSphere(XMFLOAT3(vCapsulePosition), fCapsuleRange);

		m_arrLights.push_back(capsuleLight);
	}

	void DoLighting(ID3D11DeviceContext* pd3dImmediateContext, CCamera *pCamera);

	void DrawLightVolume(ID3D11DeviceContext* pd3dImmediateContext);
	void SetGBuffer(CGBuffer* pGBuffer) { m_pGBuffer = pGBuffer; }
private:

	typedef enum
	{
		TYPE_POINT = 0,
		TYPE_SPOT,
		TYPE_CAPSULE
	} LIGHT_TYPE;

	// Light storage
	typedef struct
	{
		LIGHT_TYPE eLightType;
		XMFLOAT3 vPosition;
		XMFLOAT3 vDirection;
		float fRange;
		float fLength;
		float fOuterAngle;
		float fInnerAngle;
		XMFLOAT3 vColor;

		BoundingSphere m_bsBoundingSphere;
	} LIGHT;

	// Do the directional light calculation
	void DirectionalLight(ID3D11DeviceContext* pd3dImmediateContext);

	void PointLight(ID3D11DeviceContext* pd3dImmediateContext, const XMFLOAT3& vPos, float fRange, const XMFLOAT3& vColor, bool bWireframe);
	void SpotLight(ID3D11DeviceContext* pd3dImmediateContext, const XMFLOAT3& vPos, const XMFLOAT3& vDir, float fRange, float fInnerAngle, float fOuterAngle, const XMFLOAT3& vColor, bool bWireframe);
	void CapsuleLight(ID3D11DeviceContext* pd3dImmediateContext, const XMFLOAT3& vPos, const XMFLOAT3& vDir, float fRange, float fLen, const XMFLOAT3& vColor, bool bWireframe);

	// Directional light
	ID3D11VertexShader* m_pDirLightVertexShader;
	ID3D11PixelShader* m_pDirLightPixelShader;
	ID3D11Buffer* m_pDirLightCB;

	// Point light
	ID3D11VertexShader* m_pPointLightVertexShader;
	ID3D11HullShader* m_pPointLightHullShader;
	ID3D11DomainShader* m_pPointLightDomainShader;
	ID3D11PixelShader* m_pPointLightPixelShader;
	ID3D11Buffer* m_pPointLightDomainCB;
	ID3D11Buffer* m_pPointLightPixelCB;

	// Spot light
	ID3D11VertexShader* m_pSpotLightVertexShader;
	ID3D11HullShader* m_pSpotLightHullShader;
	ID3D11DomainShader* m_pSpotLightDomainShader;
	ID3D11PixelShader* m_pSpotLightPixelShader;
	ID3D11Buffer* m_pSpotLightDomainCB;
	ID3D11Buffer* m_pSpotLightPixelCB;

	// Capsule light
	ID3D11VertexShader* m_pCapsuleLightVertexShader;
	ID3D11HullShader* m_pCapsuleLightHullShader;
	ID3D11DomainShader* m_pCapsuleLightDomainShader;
	ID3D11PixelShader* m_pCapsuleLightPixelShader;
	ID3D11Buffer* m_pCapsuleLightDomainCB;
	ID3D11Buffer* m_pCapsuleLightPixelCB;

	// Light volume debug shader
	ID3D11PixelShader* m_pDebugLightPixelShader;

	// Depth state with no writes and stencil test on
	ID3D11DepthStencilState* m_pNoDepthWriteLessStencilMaskState;
	ID3D11DepthStencilState* m_pNoDepthWriteGreatherStencilMaskState;

	// Additive blend state to accumulate light influence
	ID3D11BlendState* m_pAdditiveBlendState;

	// Front face culling for lights volume
	ID3D11RasterizerState* m_pNoDepthClipFrontRS;

	// Wireframe render state for light volume debugging
	ID3D11RasterizerState* m_pWireframeRS;

	// Ambient light information
	XMFLOAT3 m_vAmbientLowerColor = XMFLOAT3(0, 0, 0);
	XMFLOAT3 m_vAmbientUpperColor = XMFLOAT3(0, 0, 0);

	// Directional light information
	XMFLOAT3 m_vDirectionalDir = XMFLOAT3(0, 0, 0);
	XMFLOAT3 m_vDirectionalColor = XMFLOAT3(0, 0, 0);

	// Linked list with the active lights
	std::vector<LIGHT> m_arrLights;
	CGBuffer* m_pGBuffer = nullptr;
};