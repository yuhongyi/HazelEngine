#pragma once
#include "Globals.h"
#include "GameResource.h"

class ShaderResource : public GameResource
{
public:
	// ctor & dtor
	ShaderResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating = false);
	virtual void ReleaseResource(bool isDeviceLost = false);

	// Methods
	void SetSource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source);
	ID3DXEffect* GetEffect();
private:
	ID3DXEffect* mEffect;
};