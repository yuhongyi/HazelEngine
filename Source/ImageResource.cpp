#include "Globals.h"
#include "ImageResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)

ImageResource::ImageResource() :
	mTexture(nullptr)
{
}

bool ImageResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	if (FAILED(D3DXCreateTextureFromFileEx(d3dDevice, mFileSource.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0, nullptr, nullptr, &mTexture)))
	{
		wprintf(L"Failed to load texture %s", mFileSource);
		return false;
	}

	return GameResource::InitResource(d3dDevice);
}

void ImageResource::ReleaseResource()
{
	SAFE_RELEASE(mTexture);
}