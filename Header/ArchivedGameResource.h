#pragma once
#include "Globals.h"
#include "string"
#include "GameResource.h"


class ArchivedGameResource : public GameResource
{
public:
	// ctor & dtor
	//ArchivedGameResource();

	// GameResource
	//virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating = false);
	//virtual void ReleaseResource(bool isDeviceLost = false);
	//void SetResourceId(int resourceId);
	//int GetResourceId();

	// Methods
	virtual void LoadResource(LPDIRECT3DDEVICE9 d3dDevice, const wstring& source);
	const wstring& GetFileSource() { return mFileSource; }

protected:
	wstring mFileSource;
};