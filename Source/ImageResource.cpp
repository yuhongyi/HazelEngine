#include "stdafx.h"
#include "ImageResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)

ImageResource::ImageResource() :
	mTexture(NULL)
{
	mFileSource[0] = '/0';
}

bool ImageResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	return true;
}

void ImageResource::CleanUpResource()
{
	SAFE_RELEASE(mTexture);
}

void ImageResource::SetSource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source)
{
	if(wcslen(source) < MAX_PATH)
	{
		wcscpy(&mFileSource[0], source);
	}

	// Load Texture
	if(wcslen(mFileSource) == 0)
	{
		return;
	}

	if(FAILED(D3DXCreateTextureFromFile(d3dDevice, mFileSource, &mTexture)))
	{
		wprintf(L"Failed to load texture %s", mFileSource);
	}
}