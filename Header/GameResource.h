#pragma once
#include "Globals.h"
class GameResource
{
public:
	// ctor & dtor
	GameResource();

	// Methods
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void CleanUpResource();
	void SetResourceId(int resourceId);
	int GetResourceId();
	
private:
	int mResourceId;
};