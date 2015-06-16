#pragma once
#include "Globals.h"
class GameResource
{
public:
	// ctor & dtor
	GameResource();

	// Methods
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating = false);
	virtual void ReleaseResource(bool isDeviceLost = false) = 0;
	void SetResourceId(int resourceId);
	int GetResourceId();

protected:
	TCHAR mFileSource[MAX_PATH + 1];
	
private:
	int mResourceId;
};