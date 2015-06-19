#include "Globals.h"
#include "GameResource.h"
#include "ResourceManager.h"


void ArchivedGameResource::LoadResource(LPDIRECT3DDEVICE9 d3dDevice, const wstring& source)
{
	mFileSource = source;
}