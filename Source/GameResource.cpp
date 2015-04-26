#include "stdafx.h"
#include "GameResource.h"

GameResource::GameResource() :
mResourceId(-1)
{
	;
}

bool GameResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	return true;
}

void GameResource::CleanUpResource()
{
	;
}

void GameResource::SetResourceId(int resourceId)
{
	mResourceId = resourceId;
}

int GameResource::GetResourceId()
{
	return mResourceId;
}