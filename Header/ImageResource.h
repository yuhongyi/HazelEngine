#pragma once
#include <d3d9.h>
#include "GameResource.h"

class ImageResource : public GameResource
{
public:
	// ctor & dtor
	ImageResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating = false);
	virtual void ReleaseResource(bool isDeviceLost = false);
	
	// Methods
	void LoadResource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source);
	inline LPDIRECT3DTEXTURE9 GetTexture() {return mTexture;}
private:
	LPDIRECT3DTEXTURE9 mTexture;
};