#pragma once

struct CB_PARTICLEINFO
{
	XMFLOAT3 m_d3dxvEmitPosition;
	float m_fGameTime;

	XMFLOAT3 m_d3dxvEmitDirection;
	float m_fTimeStep;
};

struct CParticle
{
	XMFLOAT3 m_f3Position = XMFLOAT3(0, 0, 0);
	XMFLOAT3 m_f3Velocity = XMFLOAT3(0, 0, 0);
	XMFLOAT2 m_f2Size = XMFLOAT2(0, 0);
	float m_fAge = 0.0f;
	UINT m_nType = 0;
};

class CParticleSystem
{
private:
	UINT m_nMaxParticles			= 0;			
	bool m_bInitializeParticle		= true;		
	float m_fGameTime				= 0.0f;				
	float m_fTimeStep				= 0.0f;				
	float m_fAge					= 0.0f;

	XMFLOAT3 m_f3EmitPosition		= XMFLOAT3(0, 0, 0);
	XMFLOAT3 m_f3EmitDirection		= XMFLOAT3(0, 1, 0);
	XMFLOAT3 m_f3CameraPosition		= XMFLOAT3(0, 0, 0);
	
	ID3D11Buffer *m_pd3dInitialVertexBuffer	= nullptr;
	ID3D11Buffer *m_pd3dStreamOutVertexBuffer = nullptr;
	ID3D11Buffer *m_pd3dDrawVertexBuffer = nullptr;

	ID3D11Buffer *m_pCBParticleInfo = nullptr;

	ID3D11InputLayout		*m_pd3dVertexLayout = nullptr;
	ID3D11VertexShader		*m_pd3dVertexShader = nullptr;
	ID3D11GeometryShader	*m_pd3dGeometryShader = nullptr;
	ID3D11PixelShader		*m_pd3dPixelShader = nullptr;
	ID3D11VertexShader		*m_pd3dSOVertexShader = nullptr;
	ID3D11GeometryShader	*m_pd3dSOGeometryShader = nullptr;
	
	ID3D11BlendState		*m_pd3dBlendState = nullptr;

	ID3D11ShaderResourceView *m_pd3dsrvRandomTexture = nullptr;
	ID3D11ShaderResourceView *m_pd3dsrvTextureArray = nullptr;

	void CreateBuffer(ID3D11Device *pd3dDevice);

public:
	CParticleSystem();
	~CParticleSystem();

	void Initialize(ID3D11Device *pd3dDevice, ID3D11ShaderResourceView *pd3dsrvTexArray, ID3D11ShaderResourceView *pd3dsrvRandomTexture, UINT nMaxParticles, ID3D11BlendState* blendState);

	void Update(float fDeltaTime);
	void Render(ID3D11DeviceContext *pd3dDeviceContext);

	void ParticleRestart() { m_bInitializeParticle = true; m_fAge = 0.0f; }

	void UpdateConstantBuffer(ID3D11DeviceContext *pd3dDeviceContext);
	void CreateShader(ID3D11Device *pd3dDevice, const wstring& wstring);
	void CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, const wstring& wstring, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dSOGeometryShader);
	
	ID3D11ShaderResourceView* CreateRandomTexture1DSRV(ID3D11Device *pd3dDevice);
	ID3D11ShaderResourceView* CreateTexture2DArraySRV(ID3D11Device *pd3dDevice, _TCHAR(*ppstrFilePaths)[128], UINT nTextures);

	// Setter
	void SetEmitPosition(XMFLOAT3 pos) { m_f3EmitPosition = pos; }
	void SetEmitvPosition(XMVECTOR pos) { XMStoreFloat3(&m_f3EmitPosition,pos); }
};