#include "Globals.h"
#include "GameResource.h"
#include "ResourceManager.h"

GameResource::GameResource() :
mResourceId(-1)
{
	;
}

bool GameResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating)
{
	if(!isRecreating)
	{
		ResourceManager::GetInstance()->AddResource(this);
	}
	return true;
}

void GameResource::SetResourceId(int resourceId)
{
	mResourceId = resourceId;
}

int GameResource::GetResourceId()
{
	return mResourceId;
}