#include "ResourceManager.h"
#include "GameConfig.h"

ResourceManager::ResourceManager() :
mImageResourceCount(0)
{
}

bool ResourceManager::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	for (const auto resource : mResources)
	{
		if (!resource->InitResource(d3dDevice))
		{
			return false;
		}
	}

	return true;
}

bool ResourceManager::LoadArchivedResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	mNumResource = 0;

	mImageResourceCount = gImageCount;
	for (int i = 0; i < gImageCount; i++)
	{
		ImageResource* image = new ImageResource();
		if (!image)
		{
			return false;
		}

		image->LoadResource(d3dDevice, gImageList[i]);
		mArchivedResources[gImageList[i]] = image;
	}

	for (int i = 0; i < gShaderCount; i++)
	{
		ShaderResource* shader = new ShaderResource();
		if (!shader)
		{
			return false;
		}

		shader->LoadResource(d3dDevice, gShaderList[i]);
		mArchivedResources[gShaderList[i]] = shader;
	}

	return true;
}

void ResourceManager::UnloadArchivedResource()
{
	for (auto resource : mArchivedResources)
	{
		delete resource.second;
	}

	mArchivedResources.empty();
}

void ResourceManager::ReleaseResource()
{
	for (const auto resource : mResources)
	{
		resource->ReleaseResource();
	}
}

ArchivedGameResource* ResourceManager::GetArchivedGameResource(const wstring& resourceName)
{
	return mArchivedResources.find(resourceName) != mArchivedResources.end() ? mArchivedResources[resourceName] : nullptr;
}