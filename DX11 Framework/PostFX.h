#pragma once

#define DXUTERR_MEDIANOTFOUND           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0903)

namespace Radar
{
	#ifndef SAFE_DELETE
		#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
	#endif
	#ifndef SAFE_DELETE_ARRAY
		#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
	#endif
	#ifndef SAFE_RELEASE
		#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
	#endif

	#define V_RETURN(X) X

	/*
	inline void DXUT_SetDebugName(IDXGIObject* pObj, const CHAR* pstrName)
	{
		if (pObj)
			pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
	}

	inline void DXUT_SetDebugName(ID3D10Device* pObj, const CHAR* pstrName)
	{
		if (pObj)
			pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
	}

	inline void DXUT_SetDebugName(ID3D10DeviceChild* pObj, const CHAR* pstrName)
	{
		if (pObj)
			pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
	}

	inline void DXUT_SetDebugName(ID3D11Device* pObj, const CHAR* pstrName)
	{
		if (pObj)
			pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
	}

	inline void DXUT_SetDebugName(ID3D11DeviceChild* pObj, const CHAR* pstrName)
	{
		if (pObj)
			pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
	}

	*/
}
using namespace Radar;

class CTextureToScreenRectMesh;

class CPostFX : public CShader
{
public:

	CPostFX();
	~CPostFX();

	HRESULT Init(ID3D11Device *pd3dDevice, UINT width, UINT height);
	void Deinit();

	// Entry point for post processing
	void PostProcessing
	(
		  ID3D11DeviceContext*		pd3dImmediateContext
		, ID3D11ShaderResourceView* pd3dsrvLDR
		, ID3D11RenderTargetView*	pd3dRenderTargetView
	);

	void Read_CSO_Shader(ID3D11Device *pd3dDevice);

	void SetParameters(const float& fMiddleGrey, const float& fWhite, const float& fAdaptation, const float& fBloomThreshold, const float& fBloomScale) { m_fMiddleGrey = fMiddleGrey; m_fWhite = fWhite; m_fAdaptation = fAdaptation; m_fBloomThreshold = fBloomThreshold; m_fBloomScale = fBloomScale; }

private:

	HRESULT WINAPI DXUTFindDXSDKMediaFileCch(WCHAR* strDestPath, int cchDest,
		LPCWSTR strFilename)
	{
		bool bFound;
		WCHAR strSearchFor[MAX_PATH];

		if (NULL == strFilename || strFilename[0] == 0 || NULL == strDestPath || cchDest < 10)
			return E_INVALIDARG;

		// Get the exe name, and exe path
		WCHAR strExePath[MAX_PATH] =
		{
			0
		};
		WCHAR strExeName[MAX_PATH] =
		{
			0
		};
		WCHAR* strLastSlash = NULL;
		GetModuleFileName(NULL, strExePath, MAX_PATH);
		strExePath[MAX_PATH - 1] = 0;
		strLastSlash = wcsrchr(strExePath, TEXT('\\'));
		if (strLastSlash)
		{
			wcscpy_s(strExeName, MAX_PATH, &strLastSlash[1]);

			// Chop the exe name from the exe path
			*strLastSlash = 0;

			// Chop the .exe from the exe name
			strLastSlash = wcsrchr(strExeName, TEXT('.'));
			if (strLastSlash)
				*strLastSlash = 0;
		}

		// Typical directories:
		//      .\
		    //      ..\
    //      ..\..\
    //      %EXE_DIR%\
    //      %EXE_DIR%\..\
    //      %EXE_DIR%\..\..\
    //      %EXE_DIR%\..\%EXE_NAME%
//      %EXE_DIR%\..\..\%EXE_NAME%

// Typical directory search
		bFound = DXUTFindMediaSearchTypicalDirs(strDestPath, cchDest, strFilename, strExePath, strExeName);
		if (bFound)
			return S_OK;

		// Typical directory search again, but also look in a subdir called "\media\" 
		swprintf_s(strSearchFor, MAX_PATH, L"media\\%s", strFilename);
		bFound = DXUTFindMediaSearchTypicalDirs(strDestPath, cchDest, strSearchFor, strExePath, strExeName);
		if (bFound)
			return S_OK;

		WCHAR strLeafName[MAX_PATH] =
		{
			0
		};

		// Search all parent directories starting at .\ and using strFilename as the leaf name
		wcscpy_s(strLeafName, MAX_PATH, strFilename);
		bFound = DXUTFindMediaSearchParentDirs(strDestPath, cchDest, L".", strLeafName);
		if (bFound)
			return S_OK;

		// Search all parent directories starting at the exe's dir and using strFilename as the leaf name
		bFound = DXUTFindMediaSearchParentDirs(strDestPath, cchDest, strExePath, strLeafName);
		if (bFound)
			return S_OK;

		// Search all parent directories starting at .\ and using "media\strFilename" as the leaf name
		swprintf_s(strLeafName, MAX_PATH, L"media\\%s", strFilename);
		bFound = DXUTFindMediaSearchParentDirs(strDestPath, cchDest, L".", strLeafName);
		if (bFound)
			return S_OK;

		// Search all parent directories starting at the exe's dir and using "media\strFilename" as the leaf name
		bFound = DXUTFindMediaSearchParentDirs(strDestPath, cchDest, strExePath, strLeafName);
		if (bFound)
			return S_OK;

		// On failure, return the file as the path but also return an error code
		wcscpy_s(strDestPath, cchDest, strFilename);

		return DXUTERR_MEDIANOTFOUND;
	}

	bool DXUTFindMediaSearchTypicalDirs(WCHAR* strSearchPath, int cchSearch, LPCWSTR strLeaf,
		WCHAR* strExePath, WCHAR* strExeName)
	{
		// Typical directories:
		//      .\
		    //      ..\
    //      ..\..\
    //      %EXE_DIR%\
    //      %EXE_DIR%\..\
    //      %EXE_DIR%\..\..\
    //      %EXE_DIR%\..\%EXE_NAME%
//      %EXE_DIR%\..\..\%EXE_NAME%
//      DXSDK media path

// Search in .\  
		wcscpy_s(strSearchPath, cchSearch, strLeaf);
		if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
			return true;

		// Search in ..\  
		swprintf_s(strSearchPath, cchSearch, L"..\\%s", strLeaf);
		if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
			return true;

		// Search in ..\..\ 
		swprintf_s(strSearchPath, cchSearch, L"..\\..\\%s", strLeaf);
		if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
			return true;

		// Search in ..\..\ 
		swprintf_s(strSearchPath, cchSearch, L"..\\..\\%s", strLeaf);
		if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
			return true;

		// Search in the %EXE_DIR%\ 
		swprintf_s(strSearchPath, cchSearch, L"%s\\%s", strExePath, strLeaf);
		if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
			return true;

		// Search in the %EXE_DIR%\..\ 
		swprintf_s(strSearchPath, cchSearch, L"%s\\..\\%s", strExePath, strLeaf);
		if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
			return true;

		// Search in the %EXE_DIR%\..\..\ 
		swprintf_s(strSearchPath, cchSearch, L"%s\\..\\..\\%s", strExePath, strLeaf);
		if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
			return true;

		// Search in "%EXE_DIR%\..\%EXE_NAME%\".  This matches the DirectX SDK layout
		swprintf_s(strSearchPath, cchSearch, L"%s\\..\\%s\\%s", strExePath, strExeName, strLeaf);
		if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
			return true;

		// Search in "%EXE_DIR%\..\..\%EXE_NAME%\".  This matches the DirectX SDK layout
		swprintf_s(strSearchPath, cchSearch, L"%s\\..\\..\\%s\\%s", strExePath, strExeName, strLeaf);
		if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
			return true;

		// Search in media search dir 
		WCHAR* s_strSearchPath = DXUTMediaSearchPath();
		if (s_strSearchPath[0] != 0)
		{
			swprintf_s(strSearchPath, cchSearch, L"%s%s", s_strSearchPath, strLeaf);
			if (GetFileAttributes(strSearchPath) != 0xFFFFFFFF)
				return true;
		}

		return false;
	}

	bool DXUTFindMediaSearchParentDirs(WCHAR* strSearchPath, int cchSearch, WCHAR* strStartAt,
		WCHAR* strLeafName)
	{
		WCHAR strFullPath[MAX_PATH] =
		{
			0
		};
		WCHAR strFullFileName[MAX_PATH] =
		{
			0
		};
		WCHAR strSearch[MAX_PATH] =
		{
			0
		};
		WCHAR* strFilePart = NULL;

		GetFullPathName(strStartAt, MAX_PATH, strFullPath, &strFilePart);
		if (strFilePart == NULL)
			return false;

		while (strFilePart != NULL && *strFilePart != '\0')
		{
			swprintf_s(strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName);
			if (GetFileAttributes(strFullFileName) != 0xFFFFFFFF)
			{
				wcscpy_s(strSearchPath, cchSearch, strFullFileName);
				return true;
			}

			swprintf_s(strSearch, MAX_PATH, L"%s\\..", strFullPath);
			GetFullPathName(strSearch, MAX_PATH, strFullPath, &strFilePart);
		}

		return false;
	}

	WCHAR* DXUTMediaSearchPath()
	{
		static WCHAR s_strMediaSearchPath[MAX_PATH] =
		{
			0
		};
		return s_strMediaSearchPath;
	}

	// Down scale the full size HDR image
	void DownScale(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pLDRSRV);

	// Extract the bloom values from the downscaled image
	void Bloom(ID3D11DeviceContext* pd3dImmediateContext);

	// Apply a gaussian blur to the input and store it in the output
	void Blur(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pInput, ID3D11UnorderedAccessView* pOutput);

	// Final pass that composites all the post processing calculations
	void FinalPass(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pLDRSRV);

	// Downscaled scene texture
	ID3D11Texture2D* m_pDownScaleRT;
	ID3D11ShaderResourceView* m_pDownScaleSRV;
	ID3D11UnorderedAccessView* m_pDownScaleUAV;

	// Temporary texture
	ID3D11Texture2D* m_pTempRT[2];
	ID3D11ShaderResourceView* m_pTempSRV[2];
	ID3D11UnorderedAccessView* m_pTempUAV[2];

	// Bloom texture
	ID3D11Texture2D* m_pBloomRT;
	ID3D11ShaderResourceView* m_pBloomSRV;
	ID3D11UnorderedAccessView* m_pBloomUAV;

	// 1D intermediate storage for the down scale operation
	ID3D11Buffer* m_pDownScale1DBuffer;
	ID3D11UnorderedAccessView* m_pDownScale1DUAV;
	ID3D11ShaderResourceView* m_pDownScale1DSRV;

	// Average luminance
	ID3D11Buffer* m_pAvgLumBuffer;
	ID3D11UnorderedAccessView* m_pAvgLumUAV;
	ID3D11ShaderResourceView* m_pAvgLumSRV;

	// Previous average luminance for adaptation
	ID3D11Buffer* m_pPrevAvgLumBuffer;
	ID3D11UnorderedAccessView* m_pPrevAvgLumUAV;
	ID3D11ShaderResourceView* m_pPrevAvgLumSRV;

	UINT m_nWidth;
	UINT m_nHeight;
	UINT m_nDownScaleGroups;
	float m_fMiddleGrey;
	float m_fWhite;
	float m_fAdaptation;
	float m_fBloomThreshold;
	float m_fBloomScale;

	typedef struct
	{
		UINT nWidth;
		UINT nHeight;
		UINT nTotalPixels;
		UINT nGroupSize;
		float fAdaptation;
		float fBloomThreshold;
		UINT pad[2];
	} TDownScaleCB;
	ID3D11Buffer* m_pDownScaleCB;

	typedef struct
	{
		float fMiddleGrey;
		float fLumWhiteSqr;
		float fBloomScale;
		UINT pad;
	} TFinalPassCB;
	ID3D11Buffer* m_pFinalPassCB;

	typedef struct
	{
		UINT numApproxPasses;
		float fHalfBoxFilterWidth;			// w/2
		float fFracHalfBoxFilterWidth;		// frac(w/2+0.5)
		float fInvFracHalfBoxFilterWidth;	// 1-frac(w/2+0.5)
		float fRcpBoxFilterWidth;			// 1/w
		UINT pad[3];
	} TBlurCB;
	ID3D11Buffer* m_pBlurCB;

	// Shaders
	ID3D11ComputeShader* m_pDownScaleFirstPassCS;
	ID3D11ComputeShader* m_pDownScaleSecondPassCS;
	ID3D11ComputeShader* m_pBloomRevealCS;
	ID3D11ComputeShader* m_HorizontalBlurCS;
	ID3D11ComputeShader* m_VerticalBlurCS;
	ID3D11VertexShader* m_pFullScreenQuadVS;
	ID3D11PixelShader* m_pFinalPassPS;

	// Direct3D 11 resources
	ID3D11SamplerState*         m_pSampLinear = NULL;
	ID3D11SamplerState*         m_pSampPoint = NULL;


	CTextureToScreenRectMesh* m_pTextScreenMesh;
};

