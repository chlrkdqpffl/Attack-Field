#pragma once
#include "Mesh.h"


class CModelMesh_FBX : public CMeshTexturedIlluminated
{
protected:
	vector<XMFLOAT3>	posVector;
	vector<XMFLOAT3>	normalVector;
	vector<XMFLOAT3>	tangentVector;
	vector<XMFLOAT3>	indexVector;
	vector<XMFLOAT2>	uvVector;

	string				m_fileName;
	float				m_fModelSize;

	XMFLOAT3			*m_pNormals		= nullptr;
	XMFLOAT2			*m_pTexCoords	= nullptr;
	XMFLOAT3			*m_pTangents	= nullptr;

	ID3D11Buffer		*m_pd3dTangentBuffer = nullptr;
	bool				m_bTangent = false;
public:
	CModelMesh_FBX(ID3D11Device *pd3dDevice, const string&, bool isTangent = false, float size = 1.0f);
	virtual ~CModelMesh_FBX();

	virtual void Initialize(ID3D11Device *pd3dDevice);
	virtual bool LoadFBXfromFile(const string& fileName);
	
	float GetModelSize() const { return m_fModelSize; }
};