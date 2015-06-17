#include "Globals.h"
#include "IndexBufferResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)

IndexBufferResource::IndexBufferResource() :
mIB(nullptr)
{
	mFileSource[0] = '/0';
}

bool IndexBufferResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating)
{
	if (FAILED(d3dDevice->CreateIndexBuffer(mBufferSize * GetIndexSize(),
		0, mBufferFormat,
		D3DPOOL_DEFAULT, &mIB, nullptr)))
	{
		wprintf(L"Failed to create index buffer");
		return false;
	}

	return GameResource::InitResource(d3dDevice, isRecreating);
}

void IndexBufferResource::ReleaseResource(bool isDeviceLost)
{
	SAFE_RELEASE(mIB);
}

void IndexBufferResource::LoadResource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source)
{
}