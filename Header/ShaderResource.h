#pragma once
#include "GameResource.h"

class ShaderResource : public GameResource
{
public:
	// ctor & dtor
	ShaderResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void CleanUpResource();

	// Methods
	void SetSource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source);
	ID3DXEffect* GetEffect();
private:
	TCHAR mFileSource[MAX_PATH + 1];
	ID3DXEffect* mEffect;
};