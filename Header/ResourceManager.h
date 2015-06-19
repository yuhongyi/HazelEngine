#pragma once
#include <vector>
#include <map>
using namespace std;

#include "Singleton.h"
#include "ImageResource.h"
#include "ShaderResource.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
	// ctor & dtor
	ResourceManager();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void ReleaseResource();

	// Methods
	void SetImageResourceCount(int imageResourceCount)
	{
		mImageResourceCount = imageResourceCount;
	}

	inline int GetRandomImageResourceId()
	{
		return rand() % mImageResourceCount;
	}


	void AddResource(GameResource* resource)
	{
		mResources.push_back(resource);
	}

	ArchivedGameResource* GetArchivedGameResource(const wstring& resourceName);
	bool LoadArchivedResource(LPDIRECT3DDEVICE9 d3dDevice);
	void UnloadArchivedResource();

private:
	vector<GameResource*> mResources;
	map<wstring, ArchivedGameResource*> mArchivedResources;
	int mImageResourceCount;
	int mNumResource;
};