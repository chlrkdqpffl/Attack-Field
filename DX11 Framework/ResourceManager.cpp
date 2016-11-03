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

	// Title Scene Texture
//	AddResourece(eTexture_TitleBackGround, "../Assets/Image/UI/BackGround2.jpg");

	// Loading Scene Texture

	// Space Scene Texture

	// Model


	// UI & etc


	// ------------------------ Mesh ---------------------------- //

}

void CResourceManager::ReleseManager()
{
}

/*
void CResourceManager::AddResourece(Resource_TextrueTag resourceTag, string source)
{
	textureMap.insert(make_pair(resourceTag, source));

	if (1 < textureMap.count(resourceTag)) {
		cout << "TextureTag No." << resourceTag << " : 다중 텍스쳐" << endl;
	}
}

void CResourceManager::AddResourece(Resource_MeshTag resourceTag, string source)
{
	meshMap.insert(make_pair(resourceTag, source));

	if (1 < meshMap.count(resourceTag)) {
		cout << "MeshTag No." << resourceTag << " : 다중 메쉬" << endl;
	}
}

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
}
*/