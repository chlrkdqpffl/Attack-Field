#pragma once
#include "Object.h"
#include "NormalMapShader.h"
#include "NormalDisplaceMapShader.h"
#include "NormalMapCubeMesh.h"


class CNormalMapObject : public CGameObject
{
public:
	CNormalMapObject();
	virtual ~CNormalMapObject();

	virtual void CreateMaterial(ID3D11Device *pd3dDevice);
	virtual void CreateMesh(ID3D11Device *pd3dDevice);
	virtual void CreateShader(ID3D11Device *pd3dDevice);
};