#include "Globals.h"
#include "GameResource.h"
#include "ResourceManager.h"

GameResource::GameResource()
{
	ResourceManager::GetInstance()->AddResource(this);
}

bool GameResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	return true;
}