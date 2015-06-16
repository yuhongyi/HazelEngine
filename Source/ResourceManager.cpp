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

bool ResourceManager::InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating)
{
	mNumResource = 0;
	mResources.clear();

	mImageResourceCount = gImageCount;
	for(int i = 0; i < gImageCount; i++)
	{
		ImageResource* image = new ImageResource();
		image->LoadResource(d3dDevice, gImageList[i]);
		if(!image->InitResource(d3dDevice))
		{
			return false;
		}
	}
		
	for(int i = 0; i < gShaderCount; i++)
	{
		ShaderResource* shader = new ShaderResource();
		shader->SetSource(d3dDevice, gShaderList[i]);
		if(!shader->InitResource(d3dDevice, isRecreating))
		{
			return false;
		}
	}

	return true;
}

void ResourceManager::ReleaseResource(bool isDeviceLost)
{
	for(auto imageIter = mResources.cbegin();
		imageIter != mResources.cend();
		++imageIter)
	{
		imageIter->second->ReleaseResource(isDeviceLost);
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