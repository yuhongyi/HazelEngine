#pragma once
#include "Globals.h"
#include "string"

using namespace std;

class GameResource
{
public:
	// ctor & dtor
	GameResource();

	// Methods
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void ReleaseResource() = 0;
	void SetResourceId(int resourceId);
	int GetResourceId();

protected:
	wstring mFileSource;
};