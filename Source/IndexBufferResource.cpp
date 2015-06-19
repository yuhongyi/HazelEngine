#include "Globals.h"
#include "IndexBufferResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)

IndexBufferResource::IndexBufferResource() :
mIB(nullptr)
{
}

bool IndexBufferResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	if (FAILED(d3dDevice->CreateIndexBuffer(mNumIndices * GetIndexSize(),
		0, mIndexFormat,
		D3DPOOL_DEFAULT, &mIB, nullptr)))
	{
		wprintf(L"Failed to create index buffer");
		return false;
	}

	assert((mNumIndices * GetIndexSize()) == mIndexData.size());

	VOID* pIndices;
	if (FAILED(mIB->Lock(0, mNumIndices * GetIndexSize(), (void**)&pIndices, 0)))
	{
		return false;
	}

	memcpy(pIndices, mIndexData.data(), mNumIndices * GetIndexSize());
	mIB->Unlock();

	return GameResource::InitResource(d3dDevice);
}

void IndexBufferResource::ReleaseResource()
{
	SAFE_RELEASE(mIB);
}

void IndexBufferResource::LoadResource(LPDIRECT3DDEVICE9 d3dDevice, const wstring& source)
{
}

void IndexBufferResource::SetIndexBufferData(UINT numIndices, D3DFORMAT indexFormat, void* indexData)
{
	mNumIndices = numIndices;
	mIndexFormat = indexFormat;

	mIndexData.resize(GetIndexSize() * numIndices);
	memcpy(mIndexData.data(), indexData, GetIndexSize() * numIndices);
}