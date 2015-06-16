#include "Globals.h"
#include "GameResource.h"

GameResource::GameResource() :
mResourceId(-1)
{
	;
}

bool GameResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating)
{
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