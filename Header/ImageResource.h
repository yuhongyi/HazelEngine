#pragma once
#include <d3d9.h>
#include "ArchivedGameResource.h"

class ImageResource : public ArchivedGameResource
{
public:
	// ctor & dtor
	ImageResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice) override;
	virtual void ReleaseResource() override;
	
	// Methods
	inline LPDIRECT3DTEXTURE9 GetTexture() {return mTexture;}
private:
	LPDIRECT3DTEXTURE9 mTexture;
};