#pragma once
#include "AimMesh.h"
#include "C2DLineShader.h"

class CAimObject
{
protected:
	bool			m_bIsActive = true;
	UINT			m_nAimSize = 0;
	XMFLOAT4		m_Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	UINT			m_nAimLength = 0;
	POINT			m_ptAimPos = POINT{ 0, 0 };

	CAimMesh*		m_pLeftLine = nullptr;
	CAimMesh*		m_pRightLine = nullptr;
	CAimMesh*		m_pTopLine = nullptr;
	CAimMesh*		m_pBottomLine = nullptr;
	CAimMesh*		m_pCenterPoint = nullptr;
	C2DLineShader*	m_pShader = nullptr;

public:
	CAimObject(UINT AimLength, XMFLOAT4 color);
	virtual ~CAimObject();

	virtual void Initialize(ID3D11Device* pDevice, POINT aimPosition, UINT width);
	virtual void SetAimSize(UINT size);

	void SetActive(bool set) { m_bIsActive = set; }
	void Render(ID3D11DeviceContext* pDeviceContext);
};