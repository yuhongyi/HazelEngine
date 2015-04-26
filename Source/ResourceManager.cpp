#include "ResourceManager.h"
#include "GameConfig.h"

ResourceManager::ResourceManager() :
mImageResourceCount(0)
{
}

ResourceManager::~ResourceManager()
{
	;
}

bool ResourceManager::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	int resource_index = 0;


	mImageResourceCount = gImageCount;
	for(int i = 0; i < gImageCount; i++)
	{
		ImageResource* image = new ImageResource();
		image->SetSource(d3dDevice, gImageList[i]);
		if(!image->InitResource(d3dDevice))
		{
			return false;
		}
		mResources.insert(pair<int, GameResource*>(resource_index++, (GameResource*)image));
	}
		
	for(int i = 0; i < gShaderCount; i++)
	{
		ShaderResource* shader = new ShaderResource();
		shader->SetSource(d3dDevice, gShaderList[i]);
		if(!shader->InitResource(d3dDevice))
		{
			return false;
		}
		mResources.insert(pair<int, GameResource*>(resource_index++, (GameResource*)shader));
	}

	return true;
}

void ResourceManager::CleanUpResource()
{
	for(auto imageIter = mResources.cbegin();
		imageIter != mResources.cend();
		++imageIter)
	{
		imageIter->second->CleanUpResource();
		delete imageIter->second;
	}

	mResources.clear();
}

GameResource* ResourceManager::GetResourceById(int id)
{
	map<int, GameResource*>::const_iterator resourceIter = mResources.find(id);
	if(resourceIter != mResources.cend())
	{
		return resourceIter->second;
	}

	return NULL;
}