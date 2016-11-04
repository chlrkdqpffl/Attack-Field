#pragma once
#include "SingletonManager.h"
//#include <map>

enum Resource_TextrueTag;
enum Resource_MeshTag;

class CResourceManager : public CSingletonManager<CResourceManager>
{
//	multimap<Resource_TextrueTag, string> textureMap;
//	multimap<Resource_MeshTag, string> meshMap;

public:
	CResourceManager();
	virtual ~CResourceManager();

	virtual void InitializeManager();

//	void AddResourece(Resource_TextrueTag resourceTag, string source);
//	void AddResourece(Resource_MeshTag resourceTag, string source);
//	wstring FindResourceFromMap(Resource_TextrueTag resourceTag);
//	string FindResourceFromMap(Resource_MeshTag resourceTag);
};