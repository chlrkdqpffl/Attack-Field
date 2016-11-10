#pragma once
#include "Object.h"
#include "NormalMapShader.h"
#include "NormalMapCubeMesh.h"

class CNormalMapObject : public CGameObject
{
public:
	CNormalMapObject();
	virtual ~CNormalMapObject();

	void CreateMaterial(ID3D11Device *pd3dDevice);			// 후에 가상함수로 구현하기
	void CreateMesh(ID3D11Device *pd3dDevice);
	void CreateShader(ID3D11Device *pd3dDevice);
};

