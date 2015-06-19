#include "Globals.h"
#include "VertexBufferResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)

VertexBufferResource::VertexBufferResource() :
mVB(nullptr)
{
}

bool VertexBufferResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	if (FAILED(d3dDevice->CreateVertexBuffer(mVertexSize * mNumVertices,
		0, mVertexFormat,
		D3DPOOL_DEFAULT, &mVB, nullptr)))
	{
		wprintf(L"Failed to create vertex buffer");
		return false;
	}

	assert((mVertexSize * mNumVertices) == mVertexData.size());

	VOID* pVertices;
	if (FAILED(mVB->Lock(0, mVertexSize * mNumVertices, (void**)&pVertices, 0)))
	{
		return false;
	}

	memcpy(pVertices, mVertexData.data(), mVertexSize * mNumVertices);
	mVB->Unlock();

	return GameResource::InitResource(d3dDevice);
}

void VertexBufferResource::ReleaseResource()
{
	SAFE_RELEASE(mVB);
}

void VertexBufferResource::LoadResource(LPDIRECT3DDEVICE9 d3dDevice, const wstring& source)
{
}

void VertexBufferResource::SetVertexBufferData(UINT vertexSize, UINT numVertices, DWORD vertexFormat, void* vertexData)
{
	mVertexSize = vertexSize;
	mNumVertices = numVertices;
	mVertexFormat = vertexFormat;

	mVertexData.resize(vertexSize * numVertices);
	memcpy(mVertexData.data(), vertexData, vertexSize * numVertices);
}