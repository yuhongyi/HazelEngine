#pragma once
#include <map>
using namespace std;
#include "ImageResource.h"
#include "SoundResource.h"
#include "ShaderResource.h"

class ResourceManager : public Singleton<ResourceManager>, public GameResource
{
public:
	// ctor & dtor
	ResourceManager();
	~ResourceManager();
	
	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void CleanUpResource();

	// Methods
	void SetImageResourceCount(int imageResourceCount)
	{
		mImageResourceCount = imageResourceCount;
	}

	inline int GetRandomImageResourceId()
	{
		return rand() % mImageResourceCount;
	}

	GameResource* GetResourceById(int id);
private:
	map<int, GameResource*> mResources;
	int mImageResourceCount;
};