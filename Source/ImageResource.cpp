#include "Globals.h"
#include "ImageResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)

ImageResource::ImageResource() :
	mTexture(nullptr)
{
	mFileSource[0] = '/0';
}

bool ImageResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating)
{
	if (FAILED(D3DXCreateTextureFromFileEx(d3dDevice, mFileSource, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0, nullptr, nullptr, &mTexture)))
	{
		wprintf(L"Failed to load texture %s", mFileSource);
		return false;
	}

	return GameResource::InitResource(d3dDevice, isRecreating);
}

void ImageResource::ReleaseResource(bool isDeviceLost)
{
	SAFE_RELEASE(mTexture);
}

void ImageResource::LoadResource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source)
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
}