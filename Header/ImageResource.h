#pragma once
#include "GameResource.h"

class ImageResource : public GameResource
{
public:
	// ctor & dtor
	ImageResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void CleanUpResource();
	
	// Methods
	void SetSource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source);
	inline LPDIRECT3DTEXTURE9 GetTexture() {return mTexture;}
private:
	TCHAR mFileSource[MAX_PATH + 1];
	LPDIRECT3DTEXTURE9 mTexture;
};