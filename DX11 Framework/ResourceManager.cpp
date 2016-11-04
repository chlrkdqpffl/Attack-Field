#include "stdafx.h"
#include "ResourceManager.h"

CResourceManager::CResourceManager()
{
}


CResourceManager::~CResourceManager()
{
}

void CResourceManager::InitializeManager()
{
	// ------------------------ Texture ---------------------------- //

	// Model

	// UI

	//  etc
	AddResourece(eTexture_Water,			"../Assets/Image/Terrain/water.jpg");
	AddResourece(eTexture_WaterDetail,		"../Assets/Image/Terrain/Water_Detail_Texture_0.dds");


	// ------------------------ Mesh ---------------------------- //

}

void CResourceManager::ReleseManager()
{
}


void CResourceManager::AddResourece(Resource_TextrueTag resourceTag, string source)
{
	textureMap.insert(make_pair(resourceTag, source));

	if (1 < textureMap.count(resourceTag)) {
		cout << "TextureTag No." << resourceTag << " : ���� �ؽ���" << endl;
	}
}

void CResourceManager::AddResourece(Resource_MeshTag resourceTag, string source)
{
	meshMap.insert(make_pair(resourceTag, source));

	if (1 < meshMap.count(resourceTag)) {
		cout << "MeshTag No." << resourceTag << " : ���� �޽�" << endl;
	}
}

/*
wstring CResourceManager::FindResourceFromMap(Resource_TextrueTag resourceTag)
{
	auto f = textureMap.find(resourceTag);

	string str = (*f).second.c_str();
	wstring wstr;
	wstr.assign(str.begin(), str.end());

	return wstr;
}
string CResourceManager::FindResourceFromMap(Resource_MeshTag resourceTag)
{
	auto f = meshMap.find(resourceTag);
	return (*f).second;
}*/

ID3D11ShaderResourceView* CResourceManager::FindResourceFromMap(Resource_TextrueTag resourceTag)
{
	auto findResource = textureMap.find(resourceTag);

	string str = (*findResource).second.c_str();
	wstring wstr;
	wstr.assign(str.begin(), str.end());
	
	HRESULT hResult;
	ID3D11ShaderResourceView *pd3dsrvTexture = NULL;
	D3DX11CreateShaderResourceViewFromFile(STATEOBJ_MGR->m_pd3dDevice.Get(), wstr.c_str(), NULL, NULL, &pd3dsrvTexture, &hResult);
	
#if defined(DEBUG) || defined(_DEBUG)
	if ((HRESULT)hResult >= 0)
		cout << "File Load Success!! <" << (*findResource).second << "> �ҷ����� ���� ! " << endl;
	else 
		cout << "File Load Error!! <" << (*findResource).second << "> �ҷ����� ���� ! - ���� �Ǵ� ��θ� Ȯ���ϼ���." << endl;
#endif

	return pd3dsrvTexture;
}